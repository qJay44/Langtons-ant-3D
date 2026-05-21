#include <GLFW/glfw3.h>
#include <cglm/mat4.h>
#include <cglm/struct/mat4.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
  #include <direct.h>
  #define CHDIR(p) _chdir(p);
#else
  #include <unistd.h>
  #define CHDIR(p) chdir(p);
#endif


#include "cglm/types-struct.h"

#include "inputs.h"
#include "engine/mesh/mesh.h"
#include "engine/camera.h"
#include "engine/mesh/meshInstanced.h"
#include "ant/ant.h"
#include "ant/grid.h"

struct Context ctx;

int main() {
  // Change cwd to where "src" directory located (since launching the executable always from the directory where its located)
  CHDIR("../../..");
  srand(time(NULL));

  const int initWidth = 1600;
  const int initHeight = 900;

  // GLFW init
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Window init
  GLFWwindow* window = glfwCreateWindow(initWidth, initHeight, "MyProgram", NULL, NULL);
  if (!window) {
    printf("Failed to create GFLW window\n");
    glfwTerminate();
    return EXIT_FAILURE;
  }
  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPos(window, initWidth * 0.5f, initHeight * 0.5f);
  glfwSetCursorPosCallback(window, cursorPosCallback);

  ctx.window = window;

  // GLAD init
  int version = gladLoadGL((GLADloadfunc)glfwGetProcAddress);
  if (!version) {
    printf("Failed to initialize GLAD\n");
    return EXIT_FAILURE;
  }

  glViewport(0, 0, initWidth, initHeight);

  vec3 lightColor = {1.f  , 1.f,   1.f  };
  vec3 bgColor    = {0.07f, 0.13f, 0.17f};

  shadersFolder = "res/shaders";
  Shader cubeShader = shaderCreate("cube.vert", "cube.frag", NULL);
  Shader voxelShader = shaderCreate("voxel.vert", "voxel.frag", NULL);
  shaderSetUniform3f(&cubeShader, "u_lightColor", lightColor);
  shaderSetUniform3f(&voxelShader, "u_lightColor", lightColor);

  Camera camera = cameraCreateDefault();
  activeCamera = &camera;

  Mesh baseCube = meshCreateCubePN();
  MeshInstanced antVoxels = meshInstancedCreateCubePN();
  Ant ant = antCreateDefault();

  double titleTimer = glfwGetTime();
  double prevTime = titleTimer;
  double currTime = prevTime;
  double dt;

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  // Keep it simple, because no windows resize callbacks
  const vec2s winCenter = getWinCenter();

  // Render loop
  while (!glfwWindowShouldClose(window)) {
    // ----- Updates --------------------------------------------- //

    glfwSetCursorPos(window, winCenter.x, winCenter.y);

    currTime = glfwGetTime();
    dt = currTime - prevTime;
    prevTime = currTime;

    ctx.time += (float)dt;

    inputsMoveCamera(activeCamera, dt);
    cameraUpdate(activeCamera);

    antUpdate(&ant);
    gridUpdateMeshBuffers(&antVoxels);

    // ----- Draw ------------------------------------------------ //

    glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    meshDraw(&baseCube, activeCamera, &cubeShader);
    meshInstancedDraw(&antVoxels, activeCamera, &voxelShader);

    // ----------------------------------------------------------- //

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Maybe call clears here
  glfwTerminate();

  printf("Done\n");

  return 0;
}


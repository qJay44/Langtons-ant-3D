#include <cglm/mat4.h>
#include <cglm/struct/mat4.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <direct.h>
#define CHDIR(p) _chdir(p);
#else
#include <unistd.h>
#define CHDIR(p) chdir(p);
#endif

#include "inputs.h"
#include "sun.h"

#include "ant/ant.h"
#include "ant/grid.h"
#include "cglm/types-struct.h"
#include "engine/camera.h"
#include "engine/mesh/meshInstanced.h"
#include "engine/text/font.h"
#include "engine/text/text.h"

struct Context ctx = {0};

int main() {
  // Change cwd to where "src" directory located (since launching the executable always from the directory where its located)
  CHDIR("../../..");

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
    fprintf(stderr, "❌Failed to create GFLW window\n");
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, cursorPosCallback);
  glfwSetKeyCallback(window, keyCallback);

  // GLAD init
  int version = gladLoadGL((GLADloadfunc)glfwGetProcAddress);
  if (!version) {
    fprintf(stderr, "❌Failed to initialize GLAD\n");
    return EXIT_FAILURE;
  }

  glViewport(0, 0, initWidth, initHeight);

  ctx.window = window;
  ctx.wireframeMode = false;
  ctx.movesPerFrame = 1;

  // Keep it simple, because no windows resize callbacks
  const vec2s winCenter = getWinCenter();
  glfwSetCursorPos(window, winCenter.x, winCenter.y);

  Sun sun = sunCreateDefault();

  shadersFolder = "res/shaders";
  Shader sunShader = shaderCreate("sun.vert", "sun.frag", NULL);
  Shader voxelShader = shaderCreate("voxel.vert", "voxel.frag", NULL);
  Shader textShader = shaderCreate("text.vert", "text.frag", NULL);
  sunSetUniforms(&sun, &voxelShader);

  Camera camera = cameraCreateDefault();
  activeCamera = &camera;

  Ant ant = antCreateDefault();
  gridInitMeshFromOBJ("res/obj/BeveledCube.obj");

  Font font = fontCreate("res/fonts/Minecraft.otf", 22, 0);
  Text textFps = textCreate(&font, 1.f, "60");
  textSetPosRelative(&textFps, (vec2s){{0.975f, 0.97f}});

  Text textSteps = textCreate(&font, 1.f, "0");
  textSetPosRelative(&textSteps, (vec2s){{0.005f, 0.97f}});

  Text textStepsPerFrame = textCreate(&font, 1.f, "0");
  textSetPosUnderOther(&textStepsPerFrame, &textSteps, (vec2s){{0.f, -20.f}});

  double titleTimer = glfwGetTime();
  double prevTime = titleTimer;
  double currTime = prevTime + 1.f;
  float dt = currTime;
  float fpsTimer = 0.f;

  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  // Render loop
  while (!glfwWindowShouldClose(window)) {
    // ----- Updates --------------------------------------------- //

    glfwSetCursorPos(window, winCenter.x, winCenter.y);

    currTime = glfwGetTime();
    dt = (float)(currTime - prevTime);
    prevTime = currTime;
    fpsTimer += dt;

    inputsMoveCamera(activeCamera, dt);
    cameraUpdate(activeCamera);

    for (int i = 0; i < ctx.movesPerFrame; i++) {
      antUpdate(&ant);
    }

    gridUpdateMesh();

    if (fpsTimer > 0.1f){
      int fps = (int)(1.f / fmaxf(dt, 0.0001f));
      textSetTexti(&textFps, fps);
      fpsTimer = 0.f;
    }

    textSetTexti(&textSteps, ant.steps);
    textSetTextFmt(&textStepsPerFrame, "x%d", ctx.movesPerFrame);

    // ----- Draw ------------------------------------------------ //

    glClearColor(0.07f, 0.13f, 0.17f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    sunDraw(&sun, activeCamera, &sunShader);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    meshInstancedDraw(&gridMesh, activeCamera, &voxelShader);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    textDraw(&textFps, activeCamera, &textShader);
    textDraw(&textSteps, activeCamera, &textShader);
    textDraw(&textStepsPerFrame, activeCamera, &textShader);

    // ----------------------------------------------------------- //

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Maybe call clears here
  glfwTerminate();

  printf("Done\n");

  return 0;
}


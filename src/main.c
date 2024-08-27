#include <cglm/mat4.h>
#include <cglm/struct/mat4.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#include "cglm/types-struct.h"
#include "cglm/affine-pre.h"

#include "camera.h"
#include "inputs.h"
#include "mesh/ant/ant.h"
#include "mesh/ant/grid.h"
#include "mesh/mesh.h"
#include "mesh/shader.h"
#include "mesh/ant/ant.h"

struct State _gState = {
  .nearPlane = 0.1f,
  .farPlane = 100.f,
  .winWidth = 1200,
  .winHeight = 720,
  .time = 0.f,
};

// Called when the window resized
void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
  glfwSetCursorPos(window, width * 0.5f, height * 0.5f);
  _gState.winWidth = width;
  _gState.winHeight = height;
}

int main() {
  // Change cwd to where "src" directory located (since launching the executable always from the directory where its located)
  SetCurrentDirectory("../../../src");
  srand(time(NULL));

  // GLFW init
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Window init
  GLFWwindow* window = glfwCreateWindow(_gState.winWidth, _gState.winHeight, "LearnOpenGL", NULL, NULL);
  if (!window) {
    printf("Failed to create GFLW window\n");
    glfwTerminate();
    return EXIT_FAILURE;
  }
  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  glfwSetCursorPos(window, _gState.winWidth * 0.5f, _gState.winHeight * 0.5f);

  // GLAD init
  int version = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  if (!version) {
    printf("Failed to initialize GLAD\n");
    return EXIT_FAILURE;
  }

  glViewport(0, 0, _gState.winWidth, _gState.winHeight);
  glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

  vec4s lightColor = (vec4s){1.f, 1.f, 1.f, 1.f};
  vec3s lightPos = (vec3s){0.5f, 0.5f, 0.5f};
  vec3s bgColor = (vec3s){0.07f, 0.13f, 0.17f};

  Shader mainShader = shaderCreate("shaders/main.vert", "shaders/main.frag", "shaders/main.geom");
  Shader boundaryShader = shaderCreate("shaders/main.vert", "shaders/boundary.frag", "shaders/boundary.geom");
  shaderUniformVec4(&mainShader, "lightColor", lightColor.raw);

  vec3s center = {20.f, 20.f, 20.f};
  Mesh baseCube = meshCreateCube(CUBE_SIZE, center, (vec3s){1.f, 1.f, 1.f}, 1.f);
  Ant ant = antCreate((vec3s)center, baseCube);
  Grid grid = gridCreate(1);

  Mesh boundaryCube = meshCreateCube(CUBE_SIZE * GRID_DIM_SIZE, center, (vec3s){1.f, 0.f, 0.f}, 1.f);

  double titleTimer = glfwGetTime();
  double prevTime = titleTimer;
  double currTime = prevTime;
  double dt;
  bool wasUnfocused = false;

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  glFrontFace(GL_CW);

  // Render loop
  while (!glfwWindowShouldClose(window)) {
    static double mouseX, mouseY;

    if (glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
      if (!wasUnfocused)
        glfwGetCursorPos(window, &mouseX, &mouseY);
      wasUnfocused = false;
    } else {
      wasUnfocused = true;
    }

    glfwSetCursorPos(window, _gState.winWidth * 0.5f, _gState.winHeight * 0.5f);

    currTime = glfwGetTime();
    dt = currTime - prevTime;
    prevTime = currTime;
    _gState.time = currTime;

    // Update window title every 0.3 seconds
    if (glfwGetTime() - titleTimer >= 0.3) {
      char title[256];
      u16 fps = 1. / dt;
      sprintf(title, "FPS: %d / %f ms", fps, dt);
      glfwSetWindowTitle(window, title);
      titleTimer = currTime;
    }

    glClearColor(bgColor.x, bgColor.y, bgColor.z, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    processInput(window);

    cameraMove(mouseX, mouseY);
    cameraUpdate(dt);
    antUpdate(&grid, &ant);

    glDisable(GL_CULL_FACE);

    // Draw the base cube
    /* shaderUniformVec3(&mainShader, "colorUni", (vec3){1.f, 1.f, 1.f}); */
    /* meshDraw(&baseCube, &mainShader); */

    // Draw ant
    shaderUniformVec3(&mainShader, "colorUni", (vec3){1.f, 1.f, 1.f});
    meshDraw(&ant.mesh, &mainShader);

    for (u32 i = 0; i < grid.idx; i++) {
      enum BlockColor bc = grid.cells[i].color;
      vec3 color = {
        (bc >> 16) / 255.f,
        ((bc >> 8) & 0xff) / 255.f,
        (bc & 0xff) / 255.f
      };
      shaderUniformVec3(&mainShader, "colorUni", color);
      meshDrawTranslated(&baseCube, &mainShader, grid.cells[i].translateVal);
    }

    // Draw boundaries
    meshDraw(&boundaryCube, &boundaryShader);

    glEnable(GL_CULL_FACE);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  printf("Done\n");

  return 0;
}


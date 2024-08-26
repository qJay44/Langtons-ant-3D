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
  shaderUniformVec4(&mainShader, "lightColor", lightColor.raw);

  Mesh baseCube = meshCreateCube(CUBE_SIZE, (vec3s){20.f, 20.f, 20.f}, (vec3s){1.f, 1.f, 1.f}, 1.f);

  Ant ant = antCreate((vec3s){20.f, 20.f, 20.f}, CUBE_SIZE * 2.f, baseCube);
  Grid grid = gridCreate(1);

  double titleTimer = glfwGetTime();
  double prevTime = titleTimer;
  double currTime = prevTime;
  double dt;

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  glFrontFace(GL_CW);

  // Render loop
  while (!glfwWindowShouldClose(window)) {
    static double mouseX, mouseY;

    glfwGetCursorPos(window, &mouseX, &mouseY);
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
      antUpdate(&grid, &ant);
    }

    glClearColor(bgColor.x, bgColor.y, bgColor.z, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    processInput(window);

    cameraMove(mouseX, mouseY);
    cameraUpdate(dt);

    glDisable(GL_CULL_FACE);

    /* shaderUniformVec3(&mainShader, "colorUni", (vec3){1.f, 1.f, 1.f}); */
    /* meshDraw(&baseCube, &mainShader); */

    for (u32 i = 0; i < grid.idx; i++) {
      enum BlockColor bc = grid.cells[i].color;
      vec3 color = {
        (bc >> 16) / 255.f,
        ((bc >> 8) & 0xff) / 255.f,
        (bc & 0xff) / 255.f
      };
      Mesh m = meshTranslateCopy(&baseCube, grid.cells[i].translateVal);
      shaderUniformVec3(&mainShader, "colorUni", color);
      meshDraw(&m, &mainShader);
    }

    shaderUniformVec3(&mainShader, "colorUni", (vec3){1.f, 1.f, 1.f});
    meshDraw(&ant.me, &mainShader);

    glEnable(GL_CULL_FACE);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  printf("Done\n");

  return 0;
}


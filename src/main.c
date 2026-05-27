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

#include "environment.h"
#include "inputs.h"
#include "sun.h"

#include "ant/ant.h"
#include "ant/grid.h"
#include "cglm/types-struct.h"
#include "engine/camera.h"
#include "engine/text/font.h"
#include "engine/text/text.h"
#include "engine/shader.h"
#include "engine/texture/textureCubemap.h"

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

  // ----- Shaders --------------------------------------------- //

  shadersFolder = "res/shaders";
  Shader voxelShader = shaderCreate("voxel.vert", "voxel.frag", NULL);
  Shader textShader = shaderCreate("text.vert", "text.frag", NULL);

  // ----- Text ------------------------------------------------ //

  Font font = fontCreate("res/fonts/Minecraft.otf", 22, 0);
  Text textFps = textCreate(&font, "60");
  textSetPosRelative(&textFps, (vec2s){{0.975f, 0.97f}});

  Text textSteps = textCreate(&font, "0");
  textSetPosRelative(&textSteps, (vec2s){{0.005f, 0.97f}});

  Text textInstances = textCreate(&font, "0");
  textSetPosUnderOther(&textInstances, &textSteps, (vec2s){{0.f, -20.f}});

  Text textStepsPerFrame = textCreate(&font, "0");
  textSetPosUnderOther(&textStepsPerFrame, &textInstances, (vec2s){{0.f, -20.f}});

  // ----- Other ----------------------------------------------- //

  gridInit(128u);

  Camera camera = cameraCreateDefault();
  camera.speed *= 2.f;
  camera.position.x = grid.size * 0.5f;
  camera.position.y = grid.size * 0.5f;
  camera.position.z = grid.size * 0.5f;
  activeCamera = &camera;

  Ant ant = antCreateDefault();
  activeAnt = &ant;

  Environment environment = envCreateDefault("res/tex/cubemaps/Cubemap_Sky_01-512x512.png");
  sunSetUniforms(&environment.sun, &voxelShader);

  // ----- Pre loop -------------------------------------------- //

  double titleTimer = glfwGetTime();
  double prevTime = titleTimer;
  double currTime = prevTime + 1.f;
  float dt = currTime;
  float fpsTimer = 0.f;

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

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

    // Update fps text every 0.1 seconds
    if (fpsTimer > 0.1f){
      int fps = (int)(1.f / fmaxf(dt, 0.0001f));
      textSetTexti(&textFps, fps);
      fpsTimer = 0.f;
    }

    textSetTextFmt(&textSteps, "Steps: %d", ant.steps);
    textSetTextFmt(&textInstances, "Voxels: %zu", gridGetVoxelsCount());
    textSetTextFmt(&textStepsPerFrame, "Speed: x%d", ctx.movesPerFrame);

    // ----- Draw to HDR buffer ---------------------------------- //

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    textureCubemap_bind(environment.skybox, 1);
    gridDraw(activeCamera, &voxelShader);

    textDraw(&textFps, activeCamera, &textShader);
    textDraw(&textSteps, activeCamera, &textShader);
    textDraw(&textStepsPerFrame, activeCamera, &textShader);
    textDraw(&textInstances, activeCamera, &textShader);

    // ----------------------------------------------------------- //

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Maybe call clears here
  glfwTerminate();

  printf("Done\n");

  return 0;
}


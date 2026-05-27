#include "engine/fbo.h"
#include "engine/mesh/mesh.h"
#include "engine/shader.h"
#include "engine/texture/TextureDescriptor.h"
#include "engine/texture/texture2D.h"
#include "engine/texture/textureCubemap.h"
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

struct Context ctx = {0};
Ant* activeAnt = NULL;

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

  Shader extractShader = shaderCreate("bloom/uv.vert", "bloom/extract.frag", NULL);
  Shader blurShader = shaderCreate("bloom/uv.vert", "bloom/blur.frag", NULL);
  Shader bloomShader = shaderCreate("bloom/uv.vert", "bloom/bloom.frag", NULL);

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

  // ----- Framebuffers ---------------------------------------- //

  // ===== HDR FBO ============================================= //

  TextureDescriptor hdrTexDesc = texture2D_defaultDesc;
  hdrTexDesc.internalFormat = GL_RGBA16F;
  hdrTexDesc.format = GL_RGBA;
  hdrTexDesc.type = GL_HALF_FLOAT;
  hdrTexDesc.minFilter = GL_LINEAR;
  hdrTexDesc.magFilter = GL_LINEAR;

  Texture2D texScreenHDR = texture2D_createEmpty(&hdrTexDesc, initWidth, initHeight);

  FBO fboScreenHDR = {0};
  fboGen(&fboScreenHDR, 1);
  fboAttach2D(&fboScreenHDR, GL_COLOR_ATTACHMENT0, texScreenHDR);

  // ===== Blur FBO ============================================ //

  TextureDescriptor blurTexDesc = texture2D_defaultDesc;
  blurTexDesc.internalFormat = GL_RGBA16F;
  blurTexDesc.format = GL_RGBA;
  blurTexDesc.type = GL_HALF_FLOAT;
  blurTexDesc.minFilter = GL_LINEAR;
  blurTexDesc.magFilter = GL_LINEAR;

  Texture2D texBlurH = texture2D_createEmpty(&blurTexDesc, initWidth / 2, initHeight / 2);
  Texture2D texBlurV = texture2D_createEmpty(&blurTexDesc, initWidth / 2, initHeight / 2);

  FBO fboScreenBlur = {0};
  fboGen(&fboScreenBlur, 1);
  fboAttach2D(&fboScreenBlur, GL_COLOR_ATTACHMENT0, texBlurH);
  fboAttach2D(&fboScreenBlur, GL_COLOR_ATTACHMENT1, texBlurV);

  // =========================================================== //

  // ----- Pre loop -------------------------------------------- //

  double titleTimer = glfwGetTime();
  double prevTime = titleTimer;
  double currTime = prevTime + 1.f;
  float dt = currTime;
  float fpsTimer = 0.f;
  size_t blurAmount = 4;

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

    fboBind(&fboScreenHDR);
    glViewport(0, 0, initWidth, initHeight);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    textureCubemap_bind(environment.skybox, 1);
    gridDraw(activeCamera, &voxelShader);

    // ----- Draw to Blur buffer (first pass) -------------------- //

    fboBind(&fboScreenBlur);
    glViewport(0, 0, initWidth / 2, initHeight / 2);
    glDrawBuffer(GL_COLOR_ATTACHMENT1); // Write to vertical
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    texture2D_bind(texScreenHDR, 0);
    meshDrawScreen(activeCamera, &extractShader);

    // ----- Draw to Blur buffer --------------------------------- //

    for (size_t i = 0; i < blurAmount; i++) {
      fboBind(&fboScreenBlur);
      glDrawBuffer(GL_COLOR_ATTACHMENT0); // Write to horizontal

      texture2D_bind(texBlurV, 0); // Read from vertical
      shaderSetUniform1ui(&blurShader, "u_horizontal", 1u);
      meshDrawScreen(activeCamera, &blurShader);

      glDrawBuffer(GL_COLOR_ATTACHMENT1); // Write to vertical

      texture2D_bind(texBlurH, 0); // Read from horizontal
      shaderSetUniform1ui(&blurShader, "u_horizontal", 0u);
      meshDrawScreen(activeCamera, &blurShader);
    }

    // ----- Draw to the main buffer ----------------------------- //

    fboUnbind();
    glViewport(0, 0, initWidth, initHeight);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    texture2D_bind(texBlurH, 0);
    texture2D_bind(texScreenHDR, 1);
    meshDrawScreen(activeCamera, &bloomShader);

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


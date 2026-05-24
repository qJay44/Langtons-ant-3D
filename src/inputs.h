#ifndef INPUTS_H
#define INPUTS_H

#include "engine/camera.h"
#include "Context.h"

static inline void inputsMoveCamera(Camera* camera, float dt) {
  if (glfwGetKey(ctx.window, GLFW_KEY_Q) == GLFW_PRESS)
    glfwSetWindowShouldClose(ctx.window, GLFW_TRUE);

  if (glfwGetKey(ctx.window, GLFW_KEY_W) == GLFW_PRESS) cameraMoveForward(camera, dt);
  if (glfwGetKey(ctx.window, GLFW_KEY_A) == GLFW_PRESS) cameraMoveLeft(camera, dt);
  if (glfwGetKey(ctx.window, GLFW_KEY_S) == GLFW_PRESS) cameraMoveBack(camera, dt);
  if (glfwGetKey(ctx.window, GLFW_KEY_D) == GLFW_PRESS) cameraMoveRight(camera, dt);

  if (glfwGetKey(ctx.window, GLFW_KEY_SPACE) == GLFW_PRESS) cameraMoveUp(camera, dt);
  if (glfwGetKey(ctx.window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) cameraMoveDown(camera, dt);

  if (glfwGetKey(ctx.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camera->speedMult = 5.f;
  else if (glfwGetKey(ctx.window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) camera->speedMult = 1.f;
}

static inline void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  (void)window;
  (void)scancode;
  (void)mods;

  switch (key) {
    case GLFW_KEY_1:
      if (action == GLFW_PRESS) {
        ctx.wireframeMode = !ctx.wireframeMode;
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE + !ctx.wireframeMode);
      }
      break;
    case GLFW_KEY_2:
      if (action == GLFW_PRESS)
        ctx.movesPerFrame >>= 1;
      break;
    case GLFW_KEY_3:
      if (action == GLFW_PRESS) {
        ctx.movesPerFrame <<= 1;
        if (ctx.movesPerFrame < 1)
          ctx.movesPerFrame = 1;
      }
      break;
  }

  ctx.movesPerFrame = ctx.movesPerFrame > 1023 ? 1024 : ctx.movesPerFrame;
  ctx.movesPerFrame = ctx.movesPerFrame < 0 ? 0 : ctx.movesPerFrame;
}

static inline void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
  (void)window; // Handling unused varible warning
  cameraRotate(activeCamera, (vec2s){{(float)xpos, (float)ypos}});
}

#endif


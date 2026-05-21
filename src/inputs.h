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

static inline void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
  (void)window; // Handling unused varible warning
  cameraRotate(activeCamera, (vec2s){{(float)xpos, (float)ypos}});
}

#endif


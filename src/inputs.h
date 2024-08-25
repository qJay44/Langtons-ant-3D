#ifndef INPUTS_H
#define INPUTS_H

#include "camera.h"

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraMoveForward();
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraMoveLeft();
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraMoveBack();
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraMoveRight();

  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) cameraMoveUp();
  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) cameraMoveDown();

  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) cameraSetIncreasedSpeed();
  else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) cameraSetNormalSpeed();
}

#endif


#ifndef CONTEXT_H
#define CONTEXT_H

#include <assert.h>
#include <cglm/types-struct.h>
#include <cglm/struct/vec2.h>

extern struct Context {
  GLFWwindow* window;
  bool wireframeMode;
} ctx;

static inline ivec2s getWinSize() {
  assert(ctx.window);
  ivec2s res;
  glfwGetWindowSize(ctx.window, &res.x, &res.y);
  return res;
}

static inline vec2s getWinSizef() {
  assert(ctx.window);
  ivec2s ws = getWinSize();
  vec2s res = {0};
  res.x = (float)ws.x;
  res.y = (float)ws.y;

  return res;
}

static inline vec2s getWinCenter() {
  ivec2s ws = getWinSize();
  vec2s winSize = {0};
  winSize.x = (float)ws.x;
  winSize.y = (float)ws.y;

  return glms_vec2_scale(winSize, 0.5f);
}

static inline vec2s getMousePos() {
  assert(ctx.window);
  double x, y;
  vec2s res;

  glfwGetCursorPos(ctx.window, &x, &y);
  res.x = (float)x;
  res.y = (float)y;

  return res;
}

#endif


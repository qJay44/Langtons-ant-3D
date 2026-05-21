#ifndef CAMERA_H
#define CAMERA_H

#include "cglm/types-struct.h"

typedef struct {
  vec3s position, up, orientation;
  float yaw, pitch, speed, speedMult, sensitivity, fov, near, far;

  mat4s proj;
  mat4s view;
} Camera;

extern Camera* activeCamera;

Camera cameraCreateDefault();

void cameraUpdate(Camera* self);
void cameraMoveForward(Camera* self, float dt);
void cameraMoveBack(Camera* self, float dt);
void cameraMoveLeft(Camera* self, float dt);
void cameraMoveRight(Camera* self, float dt);
void cameraMoveUp(Camera* self, float dt);
void cameraMoveDown(Camera* self, float dt);
void cameraRotate(Camera* self, vec2s mousePos);

#endif


#ifndef CAMERA_H
#define CAMERA_H

#include "cglm/types-struct.h"

typedef struct {
  vec3s position, orientation, up;
  float speed, sensitivity, fov;
  mat4s mat;
} Camera;

extern Camera _gCamera;

void cameraUpdate(float dt);
void cameraMoveForward();
void cameraMoveBack();
void cameraMoveLeft();
void cameraMoveRight();
void cameraMoveUp();
void cameraMoveDown();
void cameraSetIncreasedSpeed();
void cameraSetNormalSpeed();
void cameraMove(double x, double y);

#endif


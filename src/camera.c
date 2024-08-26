#include "camera.h"

#include "cglm/struct/cam.h"
#include "cglm/struct/mat4.h"

Camera _gCamera = {
  .position = {19.f, 21.f, 22.f},
  .orientation = {0.5f, -0.3f, -1.f},
  .up = {0.f, 1.f, 0.f},
  .speed = 100.f,
  .sensitivity = 100.f,
  .fov = 45.f,
};

void cameraUpdate(float dt) {
  mat4s view = GLMS_MAT4_IDENTITY_INIT;
  mat4s proj = GLMS_MAT4_IDENTITY_INIT;

  float aspectRatio = (float)_gState.winWidth / _gState.winHeight;
  _gCamera.speed *= dt;

  vec3s lookPos = glms_vec3_add(_gCamera.position, _gCamera.orientation);

  view = glms_lookat(_gCamera.position, lookPos, _gCamera.up);
  proj = glms_perspective(glm_rad(_gCamera.fov), aspectRatio, _gState.nearPlane, _gState.farPlane);

  _gCamera.mat = glms_mat4_mul(proj, view);
}

void cameraMoveForward() {
  vec3s quotient = glms_vec3_scale(_gCamera.orientation, _gCamera.speed);
  _gCamera.position = glms_vec3_add(_gCamera.position, quotient);
}

void cameraMoveBack() {
  vec3s quotient = glms_vec3_scale(_gCamera.orientation, -_gCamera.speed);
  _gCamera.position = glms_vec3_add(_gCamera.position, quotient);
}

void cameraMoveLeft() {
  vec3s normCross = glms_normalize(glms_cross(_gCamera.orientation, _gCamera.up));
  vec3s quotient = glms_vec3_scale(normCross, -_gCamera.speed);
  _gCamera.position = glms_vec3_add(_gCamera.position, quotient);
}

void cameraMoveRight() {
  vec3s normCross = glms_normalize(glms_cross(_gCamera.orientation, _gCamera.up));
  vec3s quotient = glms_vec3_scale(normCross, _gCamera.speed);
  _gCamera.position = glms_vec3_add(_gCamera.position, quotient);
}

void cameraMoveUp() {
  vec3s quotient = glms_vec3_scale(_gCamera.up, _gCamera.speed);
  _gCamera.position = glms_vec3_add(_gCamera.position, quotient);
}

void cameraMoveDown() {
  vec3s quotient = glms_vec3_scale(_gCamera.up, -_gCamera.speed);
  _gCamera.position = glms_vec3_add(_gCamera.position, quotient);
}

void cameraSetIncreasedSpeed() {
  _gCamera.speed = 4.f;
}

void cameraSetNormalSpeed() {
  _gCamera.speed = 3.f;
}

void cameraMove(double x, double y) {
  // Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
  // and then "transforms" them into degrees
  float rotX = _gCamera.sensitivity * (y - _gState.winHeight * 0.5f) / _gState.winHeight;
  float rotY = _gCamera.sensitivity * (x - _gState.winWidth * 0.5f) / _gState.winWidth;

  // Calculates upcoming vertical change in the Orientation
  vec3s newOrientation = glms_vec3_rotate(_gCamera.orientation, glm_rad(-rotX), glms_normalize(glms_cross(_gCamera.orientation, _gCamera.up)));
  vec3s upOpposite = glms_vec3_scale(_gCamera.up, -1.f);

  // Decides whether or not the next vertical Orientation is legal or not
  if (!(glms_vec3_angle(newOrientation, _gCamera.up) <= glm_rad(5.f) || glms_vec3_angle(newOrientation, upOpposite) <= glm_rad(5.f)))
    _gCamera.orientation = newOrientation;

  // Rotates the Orientation left and right
  _gCamera.orientation = glms_vec3_rotate(_gCamera.orientation, glm_rad(-rotY), _gCamera.up);
}


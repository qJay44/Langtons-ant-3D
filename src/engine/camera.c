#include "camera.h"

#include "cglm/struct/cam.h"
#include "cglm/struct/mat4.h"
#include "cglm/struct/vec2.h"
#include "cglm/struct/vec3.h"
#include "cglm/util.h"
#include "Context.h"
#include "utils.h"

Camera* activeCamera = NULL;

static void updateOrientation(Camera* self) {
  self->orientation = getDirectionYawPitchRad(self->yaw, self->pitch);
}

Camera cameraCreateDefault() {
  Camera camera;
  camera.position = (vec3s){{10.f, 10.f, 10.f}};
  camera.up = (vec3s){{0.f, 1.f, 0.f}};
  camera.yaw = glm_rad(-120.f);
  camera.pitch = glm_rad(-30.f);
  camera.speed = 10.f;
  camera.sensitivity = 2.f;
  camera.fov = glm_rad(90.f);
  camera.near = 0.1f;
  camera.far = 1000.f;

  updateOrientation(&camera);

  return camera;
}

void cameraUpdate(Camera* self) {
  updateOrientation(self);
  ivec2s winSize = getWinSize();

  float aspectRatio = (float)winSize.x / winSize.y;

  vec3s lookPos = glms_vec3_add(self->position, self->orientation);

  self->proj = glms_perspective(self->fov, aspectRatio, self->near, self->far);
  self->view = glms_lookat(self->position, lookPos, self->up);
  self->pv = glms_mat4_mul(self->proj, self->view);
  self->pvInv = glms_mat4_inv(self->pv);
}

void cameraMoveForward(Camera* self, float dt) {
  float move = self->speed * self->speedMult * dt;
  vec3s quotient = glms_vec3_scale(self->orientation, move);
  self->position = glms_vec3_add(self->position, quotient);
}

void cameraMoveBack(Camera* self, float dt) {
  float move = self->speed * self->speedMult * dt;
  vec3s quotient = glms_vec3_scale(self->orientation, -move);
  self->position = glms_vec3_add(self->position, quotient);
}

void cameraMoveLeft(Camera* self, float dt) {
  float move = self->speed * self->speedMult * dt;
  vec3s normCross = glms_normalize(glms_cross(self->orientation, self->up));
  vec3s quotient = glms_vec3_scale(normCross, -move);
  self->position = glms_vec3_add(self->position, quotient);
}

void cameraMoveRight(Camera* self, float dt) {
  float move = self->speed * self->speedMult * dt;
  vec3s normCross = glms_normalize(glms_cross(self->orientation, self->up));
  vec3s quotient = glms_vec3_scale(normCross, move);
  self->position = glms_vec3_add(self->position, quotient);
}

void cameraMoveUp(Camera* self, float dt) {
  float move = self->speed * self->speedMult * dt;
  vec3s quotient = glms_vec3_scale(self->up, move);
  self->position = glms_vec3_add(self->position, quotient);
}

void cameraMoveDown(Camera* self, float dt) {
  float move = self->speed * self->speedMult * dt;
  vec3s quotient = glms_vec3_scale(self->up, -move);
  self->position = glms_vec3_add(self->position, quotient);
}

void cameraRotate(Camera* self, vec2s mousePos) {
  vec2s winCenter = getWinCenter();
  vec2s distFromCenter = glms_vec2_sub(mousePos, winCenter);
  vec2s rad = glms_vec2_div(distFromCenter, winCenter);
  rad = glms_vec2_scale(rad, self->sensitivity);

  self->yaw += rad.x;
  self->pitch = glm_clamp(self->pitch - rad.y, -PI_2 + 0.1f, PI_2 - 0.1f);
}

void cameraSetUniforms(const Camera* self, Shader* shader) {
  shaderSetUniform1f   (shader, "u_camNear"   , self->near);
  shaderSetUniform1f   (shader, "u_camFar"    , self->far);
  shaderSetUniform1f   (shader, "u_camFov"    , self->fov);
  shaderSetUniform3f   (shader, "u_camPos"    , self->position.raw);
  shaderSetUniform3f   (shader, "u_camUp"     , self->up.raw);
  shaderSetUniformMat4f(shader, "u_camProj"   , self->proj.raw);
  shaderSetUniformMat4f(shader, "u_camView"   , self->view.raw);
  shaderSetUniformMat4f(shader, "u_camPV"     , self->pv.raw);
  shaderSetUniformMat4f(shader, "u_camInvPV"  , self->pvInv.raw);
}


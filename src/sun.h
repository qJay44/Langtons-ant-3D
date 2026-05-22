#ifndef SUN_H
#define SUN_H

#include "cglm/types-struct.h"
#include "engine/camera.h"
#include "engine/mesh/mesh.h"
#include "engine/shader.h"
#include "utils.h"

typedef struct {
  float yaw, pitch; // Radians
  float focus;
  float intensity;
  vec3s color;

  vec3s lightDir; // From source
} Sun;

static inline void sunUpdate(Sun* self) {
  self->lightDir = getDirectionYawPitchRad(self->yaw, self->pitch);
}

static inline Sun sunCreateDefault() {
  Sun sun;
  sun.yaw = 0.f;
  sun.pitch = PI_4;
  sun.focus = 800.f;
  sun.intensity = 2.f;
  sun.color = (vec3s){{1.f, 1.f, 1.f}};

  sunUpdate(&sun);

  return sun;
}

static inline void sunSetUniforms(Sun* self, Shader* shader) {
  shaderSetUniform3f(shader, "u_lightDir", self->lightDir.raw);
  shaderSetUniform3f(shader, "u_lightColor", self->color.raw);
  shaderSetUniform1f(shader, "u_sunFocus", self->focus);
  shaderSetUniform1f(shader, "u_sunIntensity", self->intensity);
}

static inline void sunDraw(Sun* self, const Camera* cam, Shader* shader) {
  sunSetUniforms(self, shader);

  glDepthMask(GL_FALSE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);

  meshDrawScreen(cam, shader);

  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);
}

#endif


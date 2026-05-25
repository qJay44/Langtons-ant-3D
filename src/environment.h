#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "cglm/util.h"
#include "engine/texture/textureCubemap.h"
#include "sun.h"

typedef struct {
  Sun sun;
  TextureCubemap skybox;
} Environment;

static inline Environment envCreateDefault(const char* skyboxImagePath) {
  Environment env;
  env.sun = sunCreateDefault();
  env.sun.pitch = glm_rad(20.f);
  env.skybox = textureCubemap_createFromImagep(&textureCubemap_defaultDesc, skyboxImagePath);

  sunUpdate(&env.sun);

  return env;
}

static inline void envDraw(Environment* self, const Camera* cam, Shader* shader) {
  sunSetUniforms(&self->sun, shader);

  glDepthMask(GL_FALSE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);

  textureCubemap_bind(self->skybox, 0);
  meshDrawScreen(cam, shader);

  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);
}

#endif


#include "texture3D.h"

#include <stdio.h>
#include <stdlib.h>

Texture3D texture3D_createEmpty(const TextureDescriptor* desc, GLsizei width, GLsizei height, GLsizei depth) {
  Texture3D id = {0};

  if (desc->target != GL_TEXTURE_3D) {
    fprintf(stderr,"❌[Texture3D::Texture3D] Got wrong target [%d]", desc->target);
    exit(EXIT_FAILURE);
  }

  glGenTextures(1, &id);
  texture3D_bind(id, 0);
  glTexParameteri(desc->target, GL_TEXTURE_MIN_FILTER, desc->minFilter);
  glTexParameteri(desc->target, GL_TEXTURE_MAG_FILTER, desc->magFilter);
  glTexParameteri(desc->target, GL_TEXTURE_WRAP_S, desc->wrapS);
  glTexParameteri(desc->target, GL_TEXTURE_WRAP_T, desc->wrapT);
  glTexParameteri(desc->target, GL_TEXTURE_WRAP_R, desc->wrapR);
  glTexImage3D(desc->target, 0, desc->internalFormat, width, height, depth, 0, desc->format, desc->type, NULL);

  if (desc->genMipMap)
    glGenerateMipmap(desc->target);

  texture3D_unbind();

  return id;
}

void texture3D_bind(Texture3D self, u32 unit) {
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(GL_TEXTURE_3D, self);
}

void texture3D_unbind() {
  glBindTexture(GL_TEXTURE_3D, 0);
}

void texture3D_clear(Texture3D* self) {
  if (*self) glDeleteTextures(1, self);
  *self = 0;
}


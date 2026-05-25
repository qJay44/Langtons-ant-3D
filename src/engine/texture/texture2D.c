#include "texture2D.h"

#include <stdio.h>
#include <stdlib.h>

Texture2D texture2D_createFromMemory(const TextureDescriptor* desc, const image2D* img) {
  Texture2D id = {0};

  if (desc->target != GL_TEXTURE_2D) {
    fprintf(stderr,"❌[Texture2D::Texture2D] Got wrong target [%d]", desc->target);
    exit(EXIT_FAILURE);
  }

  glGenTextures(1, &id);
  texture2D_bind(id, 0);
  glTexParameteri(desc->target, GL_TEXTURE_MIN_FILTER, desc->minFilter);
  glTexParameteri(desc->target, GL_TEXTURE_MAG_FILTER, desc->magFilter);
  glTexParameteri(desc->target, GL_TEXTURE_WRAP_S, desc->wrapS);
  glTexParameteri(desc->target, GL_TEXTURE_WRAP_T, desc->wrapT);
  glTexImage2D(desc->target, 0, desc->internalFormat, img->width, img->height, 0, desc->format, desc->type, img->pixels);

  if (desc->genMipMap)
    glGenerateMipmap(desc->target);

  texture2D_unbind();

  return id;
}

Texture2D texture2D_createEmpty(const TextureDescriptor* desc, int width, int height) {
  image2D dummyImg = {0};
  dummyImg.width = width;
  dummyImg.height = height;

  return texture2D_createFromMemory(desc, &dummyImg);
}

void texture2D_bind(Texture2D self, u32 unit) {
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(GL_TEXTURE_2D, self);
}

void texture2D_unbind() {
  glBindTexture(GL_TEXTURE_2D, 0);
}

void texture2D_clear(Texture2D* self) {
  if (*self) glDeleteTextures(1, self);
  *self = 0;
}


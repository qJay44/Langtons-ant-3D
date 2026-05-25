#include "textureCubemap.h"
#include "image2D.h"

#include <stdio.h>
#include <stdlib.h>

static TextureCubemap onInit(const TextureDescriptor* desc) {
  TextureCubemap id = {0};
  glGenTextures(1, &id);
  textureCubemap_bind(id, 0);
  glTexParameteri(desc->target, GL_TEXTURE_MIN_FILTER, desc->minFilter);
  glTexParameteri(desc->target, GL_TEXTURE_MAG_FILTER, desc->magFilter);
  glTexParameteri(desc->target, GL_TEXTURE_WRAP_S, desc->wrapS);
  glTexParameteri(desc->target, GL_TEXTURE_WRAP_T, desc->wrapT);
  glTexParameteri(desc->target, GL_TEXTURE_WRAP_R, desc->wrapR);

  return id;
}

TextureCubemap textureCubemap_createFromImage(const TextureDescriptor* desc, const image2D* img) {
  if (desc->target != GL_TEXTURE_CUBE_MAP) {
    fprintf(stderr,"❌[TextureCubemap::TextureCubemap] Got wrong target [%d]", desc->target);
    exit(EXIT_FAILURE);
  }

  TextureCubemap id = onInit(desc);

  //     +Y
  //  -X +Z +X -Z
  //     -Y
  static const int skipCols[6] = {2, 0, 1, 1, 1, 3};
  static const int skipRows[6] = {1, 1, 0, 2, 1, 1};

  int faceSize = img->width / 4;
  glPixelStorei(GL_UNPACK_ROW_LENGTH, img->width);

  for (int i = 0; i < 6; i++) {
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, faceSize * skipCols[i]);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, faceSize * skipRows[i]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, desc->internalFormat, faceSize, faceSize, 0, desc->format, desc->type, img->pixels);
  }

  if (desc->genMipMap)
    glGenerateMipmap(desc->target);

  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
  glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
  textureCubemap_unbind();

  return id;
}

TextureCubemap textureCubemap_createFromImagep(const TextureDescriptor* desc, const char* imgPath) {
  return textureCubemap_createFromImagepf(desc, imgPath, false);
}

TextureCubemap textureCubemap_createFromImagepf(const TextureDescriptor* desc, const char* imgPath, bool flipVertically) {
  image2D img = image2D_load(imgPath, flipVertically);
  return textureCubemap_createFromImage(desc, &img);
}

void textureCubemap_bind(TextureCubemap self, u32 unit) {
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(GL_TEXTURE_CUBE_MAP, self);
}

void textureCubemap_unbind() {
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void textureCubemap_clear(TextureCubemap* self) {
  if (*self) glDeleteTextures(1, self);
  *self = 0;
}


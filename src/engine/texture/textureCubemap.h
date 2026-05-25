#ifndef TEXTURE_CUBEMAP_H
#define TEXTURE_CUBEMAP_H

#include "TextureDescriptor.h"
#include "image2D.h"

typedef GLuint TextureCubemap;

// [img] only needs width, height and pixels
TextureCubemap textureCubemap_createFromImage(const TextureDescriptor* desc, const image2D* img);
TextureCubemap textureCubemap_createFromImagep(const TextureDescriptor* desc, const char* imgPath);
TextureCubemap textureCubemap_createFromImagepf(const TextureDescriptor* desc, const char* imgPath, bool flipVertically);

void textureCubemap_bind(TextureCubemap self, u32 unit);
void textureCubemap_unbind();
void textureCubemap_clear(TextureCubemap* self);

#endif


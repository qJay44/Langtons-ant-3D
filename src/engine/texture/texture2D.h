#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include "TextureDescriptor.h"
#include "image2D.h"

typedef GLuint Texture2D;

// [img] only needs width, height and pixels
Texture2D texture2D_createFromMemory(const TextureDescriptor* desc, const image2D* img);
Texture2D texture2D_createEmpty(const TextureDescriptor* desc, int width, int height);

void texture2D_bind(Texture2D self, u32 unit);
void texture2D_unbind();
void texture2D_clear(Texture2D* self);

#endif


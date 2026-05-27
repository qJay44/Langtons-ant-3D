#ifndef TEXTURE_3D_H
#define TEXTURE_3D_H

#include "TextureDescriptor.h"

typedef GLuint Texture3D;

[[nodiscard]] Texture3D texture3D_createEmpty(const TextureDescriptor* desc, GLsizei width, GLsizei height, GLsizei depth);

void texture3D_bind(Texture3D self, u32 unit);
void texture3D_unbind();
void texture3D_clear(Texture3D* self);

#endif


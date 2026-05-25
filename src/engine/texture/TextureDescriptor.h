#ifndef TEXTURE_DESCRIPTOR_H
#define TEXTURE_DESCRIPTOR_H

// NOTE: Use GL_NEAREST for the integer samplers (isampler, usampler, etc.)
typedef struct {
  GLenum target;
  GLenum internalFormat;
  GLenum format;
  GLenum type;
  GLenum minFilter;
  GLenum magFilter;
  GLenum wrapS;
  GLenum wrapT;
  GLenum wrapR;
  bool genMipMap;
} TextureDescriptor;

static const TextureDescriptor texture2D_defaultDesc = {
  .target         = GL_TEXTURE_2D,
  .internalFormat = GL_RGB8,
  .format         = GL_RGB,
  .type           = GL_UNSIGNED_BYTE,
  .minFilter      = GL_LINEAR,
  .magFilter      = GL_LINEAR,
  .wrapS          = GL_CLAMP_TO_EDGE,
  .wrapT          = GL_CLAMP_TO_EDGE,
  .wrapR          = GL_CLAMP_TO_EDGE,
  .genMipMap      = false
};

static const TextureDescriptor textureCubemap_defaultDesc = {
  .target         = GL_TEXTURE_CUBE_MAP,
  .internalFormat = GL_RGB8,
  .format         = GL_RGB,
  .type           = GL_UNSIGNED_BYTE,
  .minFilter      = GL_LINEAR,
  .magFilter      = GL_LINEAR,
  .wrapS          = GL_CLAMP_TO_EDGE,
  .wrapT          = GL_CLAMP_TO_EDGE,
  .wrapR          = GL_CLAMP_TO_EDGE,
  .genMipMap      = false
};

#endif


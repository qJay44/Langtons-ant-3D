#ifndef FBO_H
#define FBO_H

#include "texture/texture2D.h"
typedef struct {
  GLuint id;
  GLsizei size;
} FBO;

static inline void fboGen(FBO* self, GLsizei size) {
  glGenFramebuffers(size, &self->id);
  self->size = size;
}

static inline void fboBind(const FBO* self) {
  glBindFramebuffer(GL_FRAMEBUFFER, self->id);
}

static inline void fboUnbind() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

static inline void fboAttach2D(const FBO* self, GLenum attachment, Texture2D tex) {
  fboBind(self);
  glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, tex, 0);
  fboUnbind();
}

static inline void fboClear(FBO* self) {
  if (self->id) glDeleteFramebuffers(self->size, &self->id);
  self->id = 0;
}


#endif // FBO_H


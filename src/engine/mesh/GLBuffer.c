#include "GLBuffer.h"

void GLBuffer_gen(GLBuffer* self, GLenum target, GLsizei size) {
  glGenBuffers(size, &self->id);
  self->target = target;
  self->size = size;
}

void GLBuffer_bind(const GLBuffer* self) {
  glBindBuffer(self->target, self->id);
}

void GLBuffer_allocate(const GLBuffer* self, const void* data, GLsizeiptr dataSize, GLenum usage) {
  GLBuffer_bind(self);
  glBufferData(self->target, dataSize, data, usage);
  GLBuffer_unbind(self);
}

void GLBuffer_update(const GLBuffer* self, const void* data, GLsizeiptr dataSize, GLintptr offset) {
  GLBuffer_bind(self);
  glBufferSubData(self->target, offset, dataSize, data);
  GLBuffer_unbind(self);
}

void GLBuffer_unbind(const GLBuffer* self) {
  glBindBuffer(self->target, 0);
}

void GLBuffer_clear(GLBuffer* self) {
  if (self->id) glDeleteBuffers(self->size, &self->id);
  self->id = 0;
  self->size = 0;
}


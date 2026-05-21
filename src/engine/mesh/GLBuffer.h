#ifndef GL_BUFFER_H
#define GL_BUFFER_H

// VBO - GL_ARRAY_BUFFER
// EBO - GL_ELEMENT_ARRAY_BUFFER
// UBO - GL_UNIFORM_BUFFER
// PBO - GL_PIXEL_PACK_BUFFER
// FBO - X
// RBO - X

typedef struct {
  GLenum target;
  GLuint id;
  GLsizei size;
} GLBuffer;

void GLBuffer_gen(GLBuffer* self, GLenum target, GLsizei size);
void GLBuffer_bind(const GLBuffer* self);
void GLBuffer_allocate(const GLBuffer* self, const void* data, GLsizeiptr dataSize, GLenum usage);
void GLBuffer_unbind(const GLBuffer* self);
void GLBuffer_clear(GLBuffer* self);


#endif

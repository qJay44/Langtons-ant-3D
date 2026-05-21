#ifndef VAO_H
#define VAO_H

// Vertex Array Object
typedef struct {
  GLuint id;
  GLsizei size;
} VAO;

static inline void vaoGen(VAO* self, GLsizei size) {
  glGenVertexArrays(size, &self->id);
  self->size = size;
}

static inline void vaoBind(const VAO* self) {
  glBindVertexArray(self->id);
}

static inline void vaoLinkAttrib(GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, const void* offset) {
  glEnableVertexAttribArray(layout);
  glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
}

static inline void vaoUnbind(void) {
  glBindVertexArray(0);
}

static inline void vaoClear(VAO* self) {
  if (self->id) glDeleteVertexArrays(self->size, &self->id);
  self->id = 0;
  self->size = 0;
}

static inline const VAO* vaoEmpty() {
  static VAO vao = (VAO){.id = 0, .size = 0};
  if (vao.id == 0)
    vaoGen(&vao, 1);

  return &vao;
}

#endif


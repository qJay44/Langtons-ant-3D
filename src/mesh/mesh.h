#ifndef MESH_H
#define MESH_H

#include "cglm/types-struct.h"

#include "shader.h"
#include "vao.h"
#include "vbo.h"
#include "ebo.h"

#define MESH_VERTEX_ATTRIBUTES 11 // 3 (position) + 3 (normal) + 3 (color) + 2 (texture)

typedef struct {
  float* vertices;
  size_t vertSize;
  GLuint* indices;
  size_t indSize;

  struct VAO vao;
  struct VBO vbo;
  struct EBO ebo;

  mat4s mat;
} Mesh;

[[nodiscard]] Mesh meshCreate(float* vertices, size_t vertSize, GLuint* indices, size_t indSize);
[[nodiscard]] Mesh meshCreateCube(float size, vec3s pos, vec3s color, float scale);

void meshScale(Mesh* self, float scale);
void meshTranslate(Mesh* self, vec3s v);
void meshTranslateX(Mesh* self, float x);
void meshTranslateY(Mesh* self, float y);
void meshTranslateZ(Mesh* self, float z);
void meshDraw(Mesh* self, const Shader* shader);
void meshDelete(Mesh* self);

#endif


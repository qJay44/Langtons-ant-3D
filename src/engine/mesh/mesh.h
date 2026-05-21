#ifndef MESH_H
#define MESH_H

#include "vao.h"
#include "GLBuffer.h"
#include "Transformable.h"
#include "../shader.h"
#include "../camera.h"

typedef struct {
  size_t vertices;
  size_t indices;

  VAO vao;
  GLBuffer vbo;
  GLBuffer ebo;

  Transformable mats;
} Mesh;

[[nodiscard]] Mesh meshCreatePN(float* vertices, size_t vertSize, GLuint* indices, size_t indSize);
[[nodiscard]] Mesh meshCreateCubePN();

void meshDraw(Mesh* self, const Camera* cam, Shader* shader);
void meshDrawScreen(const Camera* cam, Shader* shader);
void meshClear(Mesh* self);

#endif


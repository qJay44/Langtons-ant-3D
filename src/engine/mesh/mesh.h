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

typedef struct {
  float* vertices;
  GLuint* indices;
  size_t vertSize;
  size_t indSize;
} MeshData;

void meshLoadObjPN(const char* filepath, MeshData* outData);

[[nodiscard]] Mesh meshCreatePN(const MeshData* data);

void meshDraw(Mesh* self, const Camera* cam, Shader* shader);
void meshDrawScreen(const Camera* cam, Shader* shader);
void meshClear(Mesh* self);

#endif


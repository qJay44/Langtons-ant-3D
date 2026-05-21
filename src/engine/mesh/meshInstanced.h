#ifndef MESH_INSTANCED_H
#define MESH_INSTANCED_H

#include "mesh.h"
#include "vao.h"
#include "GLBuffer.h"
#include "../shader.h"
#include "../camera.h"

typedef struct {
  VAO vao;
  GLBuffer vbo;
  GLBuffer ebo;
  GLBuffer vboInstanced;

  u32 indices;
  u32 instanceCount;
} MeshInstanced;

typedef struct {
  vec3s pos;
  int state;
} InstanceVertex;

[[nodiscard]] MeshInstanced meshInstancedCreatePN(const MeshData* data);

void meshInstancedDraw(MeshInstanced* self, const Camera* cam, Shader* shader);
void meshInstancedClear(MeshInstanced* self);

#endif


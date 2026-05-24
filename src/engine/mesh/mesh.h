#ifndef MESH_H
#define MESH_H

#include "vao.h"
#include "GLBuffer.h"
#include "Transformable.h"
#include "../shader.h"
#include "../camera.h"
#include "vertex.h"

#define MESH_LOAD_OBJ_ATTRIB_POSITION (1 << 0)
#define MESH_LOAD_OBJ_ATTRIB_TEXTURE  (1 << 1)
#define MESH_LOAD_OBJ_ATTRIB_NORMAL   (1 << 2)

#define MESH_LOAD_OBJ_ATTRIBS_PT (MESH_LOAD_OBJ_ATTRIB_POSITION | MESH_LOAD_OBJ_ATTRIB_TEXTURE)
#define MESH_LOAD_OBJ_ATTRIBS_PN (MESH_LOAD_OBJ_ATTRIB_POSITION | MESH_LOAD_OBJ_ATTRIB_NORMAL )

#define MESH_LOAD_OBJ_ATTRIBS_PTN (MESH_LOAD_OBJ_ATTRIBS_PT | MESH_LOAD_OBJ_ATTRIB_NORMAL)

typedef struct {
  size_t count;

  VAO vao;
  GLBuffer vbo;
  GLBuffer ebo;

  Transformable mats;
} MeshElements;

typedef struct {
  size_t count;

  VAO vao;
  GLBuffer vbo;

  Transformable mats;
} MeshArrays;

typedef struct {
  float* vertices;
  GLuint* indices;
  size_t verticesSize;
  size_t indicesSize;
  VertexLayout layout;
} MeshData;

// [outData.vertices] and [outData.indices] have to be freed
void meshLoadObj(const char* filepath, MeshData* outData, u32 attribFlags);

[[nodiscard]] MeshElements meshCreateElements(const MeshData* data);
[[nodiscard]] MeshArrays meshCreateArrays(const MeshData* data, GLenum usage);

void meshUpdateBufferVBO(MeshArrays* self, const MeshData* data, GLintptr offset);
void meshDrawElements(MeshElements* self, const Camera* cam, Shader* shader);
void meshDrawArrays(MeshArrays* self, const Camera* cam, Shader* shader);
void meshDrawScreen(const Camera* cam, Shader* shader);
void meshClear(MeshElements* self);

#endif


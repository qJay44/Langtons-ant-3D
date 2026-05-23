#ifndef GRID_H
#define GRID_H

#include "../engine/mesh/vertex.h"
#include "../engine/mesh/meshInstanced.h"

typedef struct {
  VertexPN* vertices;
  size_t verticesSize;
  GLuint* indices;
  size_t indicesSize;
} GridMeshData;

void gridInitMesh(const GridMeshData* data);
void gridInitMeshFromOBJ(const char* filepath);

int gridGetVoxel(ivec3s coord);
void gridSetVoxel(ivec3s coord, int state);
void gridUpdateMesh();

extern MeshInstanced gridMesh;

#endif


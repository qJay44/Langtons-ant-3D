#include "grid.h"

#include <assert.h>
#include <stb/stb_ds.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "cglm/types-struct.h"
#include "../engine/mesh/mesh.h"

typedef struct {
  int x, y, z;
} VoxelKey;

typedef struct {
  VoxelKey key;
  int value;
} VoxelMap;

typedef struct {
  vec3s pos;
  int state;
} VoxelInstanceAttributes;

static VoxelMap* grid = NULL;
MeshInstanced gridMesh = {0};

void gridInitMesh(const GridMeshData* data) {
  if (gridMesh.indices != 0) {
    fprintf(stderr, "[❌gridInit] Grid mesh already initialized");
    exit(EXIT_FAILURE);
  }

  gridMesh.indices = data->indicesSize / sizeof(data->indices[0]);

  vaoGen(&gridMesh.vao, 1);
  GLBuffer_gen(&gridMesh.vbo, GL_ARRAY_BUFFER, 1);
  GLBuffer_gen(&gridMesh.vboInstanced, GL_ARRAY_BUFFER, 1);
  GLBuffer_gen(&gridMesh.ebo, GL_ELEMENT_ARRAY_BUFFER, 1);

  vaoBind(&gridMesh.vao);

  GLBuffer_allocate(&gridMesh.vbo, data->vertices, data->verticesSize, GL_STATIC_DRAW);
  GLBuffer_allocate(&gridMesh.ebo, data->indices, data->indicesSize, GL_STATIC_DRAW);

  GLBuffer_bind(&gridMesh.vbo);
  GLBuffer_bind(&gridMesh.ebo);

  size_t typeSize = sizeof(float);
  size_t stride = sizeof(data->vertices[0]);
  vaoLinkAttrib(0, 3, GL_FLOAT, stride, (void*)(0 * typeSize));
  vaoLinkAttrib(1, 3, GL_FLOAT, stride, (void*)(3 * typeSize));

  GLBuffer_bind(&gridMesh.vboInstanced);
  vaoLinkAttrib(2, 3, GL_FLOAT, sizeof(VoxelInstanceAttributes), (void*)0);
  glEnableVertexAttribArray(3);
  glVertexAttribIPointer(3, 1, GL_INT, sizeof(VoxelInstanceAttributes), (void*)(sizeof(vec3s))); // float matches int size

  glVertexAttribDivisor(2, 1);
  glVertexAttribDivisor(3, 1);

  vaoUnbind();
  GLBuffer_unbind(&gridMesh.vbo);
}

void gridInitMeshFromOBJ(const char* filepath) {
  MeshData data;
  meshLoadObjPN(filepath, &data);

  GridMeshData gridData;
  gridData.vertices = (VertexPN*)data.vertices;
  gridData.verticesSize = data.verticesSize;
  gridData.indices = data.indices;
  gridData.indicesSize = data.indicesSize;

  gridInitMesh(&gridData);

  free(data.vertices);
  free(data.indices);
}

int gridGetVoxel(ivec3s coord) {
  VoxelKey key = {.x = coord.x, .y = coord.y, .z = coord.z};
  VoxelMap* entry = hmgetp(grid, key);

  return entry ? entry->value : 0;
}

void gridSetVoxel(ivec3s coord, int state) {
  VoxelKey key = { .x = coord.x, .y = coord.y, .z = coord.z };

  // hmput inserts or overwrites seamlessly
  hmput(grid, key, state);
}

void gridUpdateMesh() {
  assert(gridMesh.indices != 0);

  ptrdiff_t activeVoxels = hmlen(grid);

  if (activeVoxels == 0) {
    gridMesh.instanceCount = 0;
    return;
  }

  VoxelInstanceAttributes* instanceVertices = malloc(activeVoxels * sizeof(VoxelInstanceAttributes));
  gridMesh.instanceCount = 0;

  for (ptrdiff_t i = 0; i < activeVoxels; i++) {
    if (grid[i].value != 0) {
      instanceVertices[gridMesh.instanceCount].pos.x = (float)grid[i].key.x;
      instanceVertices[gridMesh.instanceCount].pos.y = (float)grid[i].key.y;
      instanceVertices[gridMesh.instanceCount].pos.z = (float)grid[i].key.z;
      instanceVertices[gridMesh.instanceCount].state = grid[i].value;
      gridMesh.instanceCount++;
    }
  }

  if (gridMesh.instanceCount > 0)
    GLBuffer_allocate(&gridMesh.vboInstanced, instanceVertices, gridMesh.instanceCount * sizeof(VoxelInstanceAttributes), GL_DYNAMIC_DRAW);

  free(instanceVertices);
}


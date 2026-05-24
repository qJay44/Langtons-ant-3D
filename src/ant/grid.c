#include "grid.h"

#include <assert.h>
#include <stb/stb_ds.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "cglm/types-struct.h"
#include "../engine/mesh/mesh.h"

#define MAX_VOXELS 100000u

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

static VoxelInstanceAttributes* staticInstanceVertices = NULL;

void gridInitMesh(const MeshData* data) {
  if (staticInstanceVertices) {
    fprintf(stderr, "[❌gridInit] Grid mesh already initialized");
    exit(EXIT_FAILURE);
  }

  staticInstanceVertices = malloc(MAX_VOXELS * sizeof(VoxelInstanceAttributes));

  MeshElements mesh = meshCreateElements(data);

  gridMesh.vao = mesh.vao;
  gridMesh.vbo = mesh.vbo;
  gridMesh.ebo = mesh.ebo;
  gridMesh.indices = data->indicesSize / sizeof(data->indices[0]);

  vaoBind(&gridMesh.vao);
  GLBuffer_gen(&gridMesh.vboInstanced, GL_ARRAY_BUFFER, 1);
  GLBuffer_bind(&gridMesh.vboInstanced);
  vaoLinkAttrib(3, 3, GL_FLOAT, sizeof(VoxelInstanceAttributes), (void*)0);
  glEnableVertexAttribArray(4);
  glVertexAttribIPointer(4, 1, GL_INT, sizeof(VoxelInstanceAttributes), (void*)(sizeof(vec3s))); // float matches int size

  glVertexAttribDivisor(3, 1);
  glVertexAttribDivisor(4, 1);

  vaoUnbind();
  GLBuffer_unbind(&gridMesh.vbo);
}

void gridInitMeshFromOBJ(const char* filepath, u32 attribFlags) {
  MeshData data = {0};
  data.layout = PTN_LAYOUT;

  meshLoadObj(filepath, &data, attribFlags);
  gridInitMesh(&data);

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
  gridMesh.instanceCount = 0;

  if (activeVoxels == 0)
    return;

  if (activeVoxels > MAX_VOXELS) {
    fprintf(stderr, "❌Exceeding max voxels [%zu]\n", activeVoxels);
    exit(EXIT_FAILURE);
  }

  for (ptrdiff_t i = 0; i < activeVoxels; i++) {
    staticInstanceVertices[gridMesh.instanceCount].pos.x = (float)grid[i].key.x;
    staticInstanceVertices[gridMesh.instanceCount].pos.y = (float)grid[i].key.y;
    staticInstanceVertices[gridMesh.instanceCount].pos.z = (float)grid[i].key.z;
    staticInstanceVertices[gridMesh.instanceCount].state = grid[i].value;
    gridMesh.instanceCount++;
  }

  if (gridMesh.instanceCount > 0)
    GLBuffer_allocate(&gridMesh.vboInstanced, staticInstanceVertices, gridMesh.instanceCount * sizeof(VoxelInstanceAttributes), GL_DYNAMIC_DRAW);
}

void gridClear() {
  if (staticInstanceVertices) free(staticInstanceVertices);
  staticInstanceVertices = NULL;
}


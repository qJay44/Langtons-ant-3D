#ifndef GRID_H
#define GRID_H

#include "../engine/mesh/meshInstanced.h"

typedef struct {
  int x, y, z;
} VoxelKey;

typedef struct {
  VoxelKey key;
  int value;
} VoxelMap;

int gridGetVoxel(ivec3s coord);
void gridSetVoxel(ivec3s coord, int state);
void gridUpdateMeshBuffers(MeshInstanced* mesh);

extern VoxelMap* grid;

#endif

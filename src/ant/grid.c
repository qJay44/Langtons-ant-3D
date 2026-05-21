#include "grid.h"
#include "cglm/types-struct.h"

#include <stb/stb_ds.h>
#include <stddef.h>
#include <stdlib.h>

VoxelMap* grid = NULL;

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

void gridUpdateMeshBuffers(MeshInstanced* mesh) {
  ptrdiff_t activeVoxels = hmlen(grid);

  if (activeVoxels == 0) {
    mesh->instanceCount = 0;
    return;
  }

  InstanceVertex* instanceVertices = malloc(activeVoxels * sizeof(InstanceVertex));
  mesh->instanceCount = 0;

  for (ptrdiff_t i = 0; i < activeVoxels; i++) {
    if (grid[i].value != 0) {
      instanceVertices[mesh->instanceCount].pos.x = (float)grid[i].key.x;
      instanceVertices[mesh->instanceCount].pos.y = (float)grid[i].key.y;
      instanceVertices[mesh->instanceCount].pos.z = (float)grid[i].key.z;
      instanceVertices[mesh->instanceCount].state = grid[i].value;
      mesh->instanceCount++;
    }
  }

  if (mesh->instanceCount > 0)
    GLBuffer_allocate(&mesh->vboInstanced, instanceVertices, mesh->instanceCount * sizeof(InstanceVertex), GL_DYNAMIC_DRAW);

  free(instanceVertices);
}


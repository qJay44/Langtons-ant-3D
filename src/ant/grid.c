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

typedef struct VoxelMap{
  VoxelKey key;
  u8 value;
} VoxelMap;

typedef struct {
  vec3s pos;
  u8 state;
} VoxelInstanceAttributes;

Grid grid = {0};

void gridInit(GLsizei size) {
  if (grid.tex || grid.size || grid.map) {
    fprintf(stderr, "[❌gridInit] Grid already initialized");
    exit(EXIT_FAILURE);
  }

  TextureDescriptor texDesc = {
    .target         = GL_TEXTURE_3D,
    .internalFormat = GL_R8UI,
    .format         = GL_RED_INTEGER,
    .type           = GL_UNSIGNED_BYTE,
    .minFilter      = GL_NEAREST,
    .magFilter      = GL_NEAREST,
    .wrapS          = GL_CLAMP_TO_EDGE,
    .wrapT          = GL_CLAMP_TO_EDGE,
    .wrapR          = GL_CLAMP_TO_EDGE,
    .genMipMap      = false
  };

  grid.tex = texture3D_createEmpty(&texDesc, size, size, size);
  grid.size = size;
  grid.map = NULL;
}

u8 gridGetVoxel(ivec3s coord) {
  VoxelKey key = {.x = coord.x, .y = coord.y, .z = coord.z};
  VoxelMap* entry = hmgetp(grid.map, key);

  return entry ? entry->value : 0u;
}

size_t gridGetVoxelsCount() {
  return hmlen(grid.map);
}

void gridSetVoxel(ivec3s coord, u8 state) {
  VoxelKey key = { .x = coord.x, .y = coord.y, .z = coord.z };

  // hmput inserts or overwrites seamlessly
  hmput(grid.map, key, state);

  texture3D_bind(grid.tex, 0);
  glTexSubImage3D(GL_TEXTURE_3D, 0, coord.x, coord.y, coord.z, 1, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_BYTE, &state);
}

void gridDraw(const Camera* cam, Shader* shader) {
  texture3D_bind(grid.tex, 0);
  shaderSetUniform1ui(shader, "u_gridSize", grid.size);
  meshDrawScreen(cam, shader);
}

void gridClearStates() {
  if (grid.map) hmfree(grid.map);
  grid.map = NULL;
  glClearTexImage(grid.tex, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, (void*)(0));
}


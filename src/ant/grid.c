#include "Context.h"
#include "grid.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../engine/mesh/mesh.h"
#include "cglm/types-struct.h"

typedef struct {
  vec3s pos;
  u8 state;
} VoxelInstanceAttributes;

Grid grid = {0};

void gridInit(GLsizei dsize) {
  if (grid.tex || grid.dsize || grid.data) {
    fprintf(stderr, "[gridInit]❌ Grid already initialized");
    exit(EXIT_FAILURE);
  }

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

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

  size_t totalCount = dsize * dsize * dsize;

  grid.data = calloc(totalCount, sizeof(u8));
  grid.tex = texture3D_createEmpty(&texDesc, dsize, dsize, dsize);
  grid.dsize = dsize;

  printf("Grid array size: [%.2f] MB\n", (totalCount * sizeof(u8)) / (1024.f * 1024.f));

  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

u8 gridGetVoxel(ivec3s coord) {
  assert(grid.data && coord.x > 0 && coord.y > 0 coord.z > 0);
  return grid.data[coord.x + (coord.y + coord.z * grid.dsize) * grid.dsize];
}

void gridSetVoxel(ivec3s coord, u8 state) {
  assert(grid.data && coord.x > 0 && coord.y > 0 coord.z > 0);
  grid.data[coord.x + (coord.y + coord.z * grid.dsize) * grid.dsize] = state;
}

void gridUpdateTexture() {
  texture3D_bind(grid.tex, 0);
  glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, grid.dsize, grid.dsize, grid.dsize, GL_RED_INTEGER, GL_UNSIGNED_BYTE, grid.data);
}

void gridDraw(const Camera* cam, Shader* shader) {
  texture3D_bind(grid.tex, 0);
  shaderSetUniform1ui(shader, "u_gridSize", grid.dsize);
  meshDrawScreen(cam, shader);
}

void gridClearStates() {
  ctx.activeVoxels = 0;
  memset(grid.data, 0, sizeof(u8) * grid.dsize * grid.dsize * grid.dsize);
  glClearTexImage(grid.tex, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, (void*)(0));
}


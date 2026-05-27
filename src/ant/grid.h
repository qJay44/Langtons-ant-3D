#ifndef GRID_H
#define GRID_H

#include "../engine/texture/texture3D.h"
#include "../engine/camera.h"
#include "../engine/shader.h"
#include "cglm/types-struct.h"

void gridInit(GLsizei size);

u8 gridGetVoxel(ivec3s coord);
size_t gridGetVoxelsCount();

void gridSetVoxel(ivec3s coord, u8 state);
void gridDraw(const Camera* cam, Shader* shader);
void gridClearStates();

typedef struct {
  Texture3D tex;
  GLsizei size;
  struct VoxelMap* map;
} Grid;

extern Grid grid;

#endif


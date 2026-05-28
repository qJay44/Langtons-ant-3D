#ifndef GRID_H
#define GRID_H

#include "../engine/texture/texture3D.h"
#include "../engine/camera.h"
#include "../engine/shader.h"
#include "cglm/types-struct.h"

#define GRID_MAX_STATES 10u

void gridInit(GLsizei size);

u8 gridGetVoxel(ivec3s coord);

void gridSetVoxel(ivec3s coord, u8 state);
void gridUpdateTexture();
void gridDraw(const Camera* cam, Shader* shader);
void gridClearStates();

typedef struct {
  u8* data;
  Texture3D tex;
  GLsizei dsize; // Size for each dimension
} Grid;

extern Grid grid;

#endif


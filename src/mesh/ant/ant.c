#include "ant.h"
#include "grid.h"

Ant antCreate(vec3s pos, float stepSize, Mesh representation) {
  Ant ant = {
    .pos = pos,
    .dir = UP,
    .stepSize = stepSize,
    .me = representation,
    .steps = 0,
  };

  return ant;
}

void antPlace(Ant* self, enum BlockColor* color) {
  switch (*color) {
    case BLOCK_COLOR1:
      antRotateLeft(self);
      *color = BLOCK_COLOR2;
      break;
    case BLOCK_COLOR2:
      antRotateRight(self);
      *color = BLOCK_COLOR3;
      break;
    case BLOCK_COLOR3:
      antRotateFront(self);
      *color = BLOCK_COLOR4;
      break;
    case BLOCK_COLOR4:
      antRotateRear(self);
      *color = BLOCK_COLOR5;
      break;
    case BLOCK_COLOR5:
      antRotateFront(self);
      *color = BLOCK_COLOR1;
      break;
  }
}


// NOTE: Not including the direction whicn ant currently facing

void antRotateLeft(Ant* self) {
  switch (self->dir) {
    case FORWARD:
      self->dir = LEFT;
      break;
    case LEFT:
      self->dir = BACK;
      break;
    case BACK:
      self->dir = RIGHT;
      break;
    case RIGHT:
      self->dir = FORWARD;
      break;
    case UP:
      self->dir = LEFT;
      break;
    case DOWN:
      self->dir = RIGHT;
      break;
  }
}

void antRotateRight(Ant* self) {
  switch (self->dir) {
    case FORWARD:
      self->dir = RIGHT;
      break;
    case LEFT:
      self->dir = FORWARD;
      break;
    case BACK:
      self->dir = LEFT;
      break;
    case RIGHT:
      self->dir = BACK;
      break;
    case UP:
      self->dir = RIGHT;
      break;
    case DOWN:
      self->dir = LEFT;
      break;
  }
}

void antRotateFront(Ant* self) {
  switch (self->dir) {
    case UP:
      self->dir = FORWARD;
      break;
    case DOWN:
      self->dir = BACK;
      break;
    default:
     self->dir = DOWN;
  }
}

void antRotateRear(Ant* self) {
  switch (self->dir) {
    case UP:
      self->dir = BACK;
      break;
    case DOWN:
      self->dir = FORWARD;
      break;
    default:
     self->dir = UP;
  }
}

void antMove(Ant* self) {
  switch (self->dir) {
    case FORWARD:
      self->pos.x += self->stepSize;
      meshTranslateX(&self->me, self->stepSize);
      break;
    case BACK:
      self->pos.x -= self->stepSize;
      meshTranslateX(&self->me, -self->stepSize);
      break;
    case LEFT:
      self->pos.z -= self->stepSize;
      meshTranslateZ(&self->me, -self->stepSize);
      break;
    case RIGHT:
      self->pos.z += self->stepSize;
      meshTranslateZ(&self->me, self->stepSize);
      break;
    case UP:
      self->pos.y += self->stepSize;
      meshTranslateY(&self->me, self->stepSize);
      break;
    case DOWN:
      self->pos.y -= self->stepSize;
      meshTranslateY(&self->me, -self->stepSize);
      break;
  }
  self->steps++;
}

void antUpdate(Grid* grid, Ant *self) {
  u32 x = MIN(MAX((u32)self->pos.x * 1.f / CUBE_SIZE, 0), GRID_DIM_SIZE);
  u32 y = MIN(MAX((u32)self->pos.y * 1.f / CUBE_SIZE, 0), GRID_DIM_SIZE);
  u32 z = MIN(MAX((u32)self->pos.z * 1.f / CUBE_SIZE, 0), GRID_DIM_SIZE);
  u32 idx = IX(x, y, z);

  for (u32 i = 0; i < grid->idx; i++) {
    if (grid->cells[i].idx == idx) {
      antPlace(self, &grid->cells[i].color);
      antMove(self);
      return;
    }
  }

  // If block doesn't exist
  gridAdd(grid, (struct Cell){.pos = self->pos, .idx = idx, .color = BLOCK_COLOR1});
}


#include "ant.h"
#include "grid.h"

#include <cglm/struct/vec3.h>
#include <cglm/util.h>
#include <cglm/vec3.h>
#include <stdlib.h>

#define MAX_XYZ (u32)(GRID_DIM_SIZE / STEP_SIZE)

Ant antCreate(vec3s pos, Mesh representation) {
  static void (*moves[4])(struct Ant*) = {
    &antRotateLeft,
    &antRotateRight,
    &antRotateForward,
    &antRotateBackwards,
  };

  return (Ant) {
    .origin = pos,
    .pos = pos,
    .dir = FORWARD,
    .mesh = representation,
    .rule = {
      moves[rand() % 4],
      moves[rand() % 4],
      moves[rand() % 4],
      moves[rand() % 4],
      moves[rand() % 4]
    },
    .steps = 0,
  };
}

void antPlace(Ant* self, struct Cell* cell) {
  enum BlockColorIndex* color = &cell->bci;

  self->rule[*color](self); // Apply rule
  *color = (1 + *color) % 5; // Change color

  // Translation vector from the origin to the current position
  glm_vec3_sub(self->pos.raw, self->origin.raw, cell->translateVal.raw);
}

void antMove(Ant* self) {
  switch (self->dir) {
    case FORWARD:
      self->pos.x += STEP_SIZE;
      meshTranslateX(&self->mesh, STEP_SIZE);
      break;
    case BACKWARD:
      self->pos.x -= STEP_SIZE;
      meshTranslateX(&self->mesh, -STEP_SIZE);
      break;
    case LEFT:
      self->pos.z -= STEP_SIZE;
      meshTranslateZ(&self->mesh, -STEP_SIZE);
      break;
    case RIGHT:
      self->pos.z += STEP_SIZE;
      meshTranslateZ(&self->mesh, STEP_SIZE);
      break;
    case UPWARD:
      self->pos.y += STEP_SIZE;
      meshTranslateY(&self->mesh, STEP_SIZE);
      break;
    case DOWNWARD:
      self->pos.y -= STEP_SIZE;
      meshTranslateY(&self->mesh, -STEP_SIZE);
      break;
  }

  // Keep ant in boundaries
  vec3s before = self->pos;
  glm_vec3_clamp(self->pos.raw, 0, MAX_XYZ);
  meshTranslate(&self->mesh, glms_vec3_sub(self->pos, before));

  self->steps++;
}

void antUpdate(Grid* grid, Ant *self) {
  u32 idx = IX((u32)(self->pos.x / STEP_SIZE), (u32)(self->pos.y / STEP_SIZE), (u32)(self->pos.z / STEP_SIZE));

  for (u32 i = 0; i < grid->idx; i++) {
    if (grid->cells[i].idx == idx) {
      antPlace(self, &grid->cells[i]);
      antMove(self);
      return;
    }
  }

  // If block doesn't exist
  gridAdd(grid, (struct Cell){.translateVal = glms_vec3_sub(self->pos, self->origin), .idx = idx, .bci = BLOCK_COLOR_IDX_0});
  antMove(self);
  antRotateRight(self);
}

// NOTE: Not including the direction whicn ant currently facing

void antRotateLeft(Ant* self) {
  switch (self->dir) {
    case FORWARD:
      self->dir = LEFT;
      break;
    case LEFT:
      self->dir = BACKWARD;
      break;
    case BACKWARD:
      self->dir = RIGHT;
      break;
    case RIGHT:
      self->dir = FORWARD;
      break;
    case UPWARD:
      self->dir = LEFT;
      break;
    case DOWNWARD:
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
    case BACKWARD:
      self->dir = LEFT;
      break;
    case RIGHT:
      self->dir = BACKWARD;
      break;
    case UPWARD:
      self->dir = RIGHT;
      break;
    case DOWNWARD:
      self->dir = LEFT;
      break;
  }
}

void antRotateForward(Ant* self) {
  switch (self->dir) {
    case UPWARD:
      self->dir = FORWARD;
      break;
    case DOWNWARD:
      self->dir = BACKWARD;
      break;
    default:
     self->dir = DOWNWARD;
  }
}

void antRotateBackwards(Ant* self) {
  switch (self->dir) {
    case UPWARD:
      self->dir = BACKWARD;
      break;
    case DOWNWARD:
      self->dir = FORWARD;
      break;
    default:
     self->dir = UPWARD;
  }
}


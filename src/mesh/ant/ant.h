#ifndef ANT_H
#define ANT_H

#include "../mesh.h"
#include "grid.h"

#define CUBE_SIZE 0.1f
#define STEP_SIZE (CUBE_SIZE * 2.f)

enum Direction : u8 {
  FORWARD, BACKWARD, LEFT, RIGHT,
  UPWARD, DOWNWARD
};

typedef struct {
  vec3s origin;
  vec3s pos;
  enum Direction dir;
  Mesh mesh;
  u32 steps;
} Ant;

[[nodiscard]] Ant antCreate(vec3s pos, Mesh representation);


void antPlace(Ant* self, struct Cell* cell);
void antMove(Ant* self);
void antUpdate(Grid* grid, Ant* self);
void antRotateLeft(Ant* self);
void antRotateRight(Ant* self);
void antRotateForward(Ant* self);
void antRotateBackwards(Ant* self);

#endif


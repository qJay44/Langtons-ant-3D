#ifndef ANT_H
#define ANT_H

#include "../mesh.h"
#include "grid.h"

#define CUBE_SIZE 0.1f

enum Direction : u8 {
  FORWARD, BACK, LEFT, RIGHT,
  UP, DOWN
};

typedef struct {
  enum Direction dir;
  vec3s pos;
  float stepSize;
  Mesh me;
  u32 steps;
} Ant;

[[nodiscard]] Ant antCreate(vec3s pos, float stepSize, Mesh representation);


void antPlace(Ant* self, enum BlockColor* color);
void antRotateLeft(Ant* self);
void antRotateRight(Ant* self);
void antRotateFront(Ant* self);
void antRotateRear(Ant* self);
void antMove(Ant* self);
void antUpdate(Grid* grid, Ant* self);

#endif


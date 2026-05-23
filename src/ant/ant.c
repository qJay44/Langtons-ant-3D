#include "ant.h"
#include "cglm/types-struct.h"
#include "grid.h"
#include "utils.h"

#include <cglm/struct/vec3.h>
#include <cglm/util.h>
#include <cglm/vec3.h>

typedef enum {
  TURN_LEFT,
  TURN_RIGHT,
  TURN_UP,
  TURN_DOWN,
} AntTurn;

Ant antCreateDefault() {
  Ant ant = {
    {{ 0,  0,  0}},
    {{ 0, -1,  0}},
    {{ 0,  0, -1}},
    0
  };

  return ant;
}

static void antMove(Ant* self) {
  self->pos = ivec3s_add(self->pos, self->dir);
  self->steps++;
}

static void antTurn(Ant* self, AntTurn turn) {
  ivec3s oldDir = self->dir;
  ivec3s right = ivec3s_cross(self->dir, self->up);

  switch (turn) {
    case TURN_LEFT:
      self->dir = (ivec3s){{-right.x, -right.y, -right.z}};
      break;
    case TURN_RIGHT:
      self->dir = right;
      break;
    case TURN_UP:
      self->dir = self->up;
      self->up = (ivec3s){{-oldDir.x, -oldDir.y, -oldDir.z}};
      break;
    case TURN_DOWN:
      self->dir = (ivec3s){{-self->up.x, -self->up.y, -self->up.z}};
      self->up = oldDir;
      break;
  }
}

void antUpdate(Ant* self) {
  int currState = gridGetVoxel(self->pos);

  switch (currState) {
    case 0:
      gridSetVoxel(self->pos, 1);
      antTurn(self, TURN_LEFT);
      break;
    case 1:
      gridSetVoxel(self->pos, 2);
      antTurn(self, TURN_RIGHT);
      break;
    case 2:
      gridSetVoxel(self->pos, 3);
      antTurn(self, TURN_UP);
      break;
    case 3:
      gridSetVoxel(self->pos, 4);
      antTurn(self, TURN_DOWN);
      break;
    case 4:
      gridSetVoxel(self->pos, 5);
      antTurn(self, TURN_UP);
      break;
    case 5:
      gridSetVoxel(self->pos, 0);
      antTurn(self, TURN_LEFT);
      break;
  }

  antMove(self);
}


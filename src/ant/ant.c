#include "ant.h"
#include "cglm/types-struct.h"
#include "grid.h"
#include "utils.h"

#include <cglm/struct/vec3.h>
#include <cglm/util.h>
#include <cglm/vec3.h>

static const ivec3s initPos = (ivec3s){{0, 0,  0}};
static const ivec3s initDir = (ivec3s){{0, 0, -1}};
static const ivec3s initUp  = (ivec3s){{0, 1,  0}};

Ant antCreateDefault() {
  Ant ant = {
    initPos,
    initDir,
    initUp,
    {
      TURN_LEFT,
      TURN_RIGHT,
      TURN_UP,
      TURN_DOWN,
      TURN_LEFT,
    },
    ANT_MAX_STATES,
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
  int nextState = (currState + 1) % self->activeStates;

  gridSetVoxel(self->pos, nextState);
  antTurn(self, self->rules[currState]);
  antMove(self);
}

void antRandomizeRules(Ant* self) {
  gridClearStates();

  self->activeStates = rand() % ANT_MAX_STATES + 1;
  for (int i = 0; i < self->activeStates; i++)
    self->rules[i] = (AntTurn)(rand() % 4);


  self->pos = initPos;
  self->dir = initDir;
  self->up = initUp;
  self->steps = 0;
}


#include "Context.h"
#include "ant.h"
#include "grid.h"
#include "utils.h"

#include <assert.h>
#include <cglm/struct/vec3.h>
#include <cglm/util.h>
#include <cglm/vec3.h>

static const ivec3s initDir = (ivec3s){{0, 0, -1}};
static const ivec3s initUp  = (ivec3s){{0, 1,  0}};

Ant* activeAnt = NULL;

Ant antCreateDefault() {
  Ant ant = {0};
  antRandomizeRules(&ant);

  return ant;
}

static void antMove(Ant* self) {
  assert(self && grid.size);

  self->pos = ivec3s_add(self->pos, self->dir);
  self->steps++;
  GLsizei edgeCoord = grid.dsize - 1;

  if (self->pos.x < 0) self->pos.x = edgeCoord;
  if (self->pos.y < 0) self->pos.y = edgeCoord;
  if (self->pos.z < 0) self->pos.z = edgeCoord;

  if (self->pos.x > edgeCoord) self->pos.x = 0;
  if (self->pos.y > edgeCoord) self->pos.y = 0;
  if (self->pos.z > edgeCoord) self->pos.z = 0;
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
  u32 currState = gridGetVoxel(self->pos);
  u32 nextState = (currState + 1) % self->activeStates;
  nextState += nextState == 0;

  if (currState == 0)
    ctx.activeVoxels++;

  gridSetVoxel(self->pos, nextState);
  antTurn(self, self->rules[currState]);
  antMove(self);
}

void antRandomizeRules(Ant* self) {
  gridClearStates();

  self->activeStates = rand() % GRID_MAX_STATES + 1;
  for (u8 i = 0; i < self->activeStates; i++)
    self->rules[i] = (AntTurn)(rand() % 4);

  ivec3s pos = {0};
  pos.x = grid.dsize / 2;
  pos.y = grid.dsize / 2;
  pos.z = grid.dsize / 2;

  self->pos = pos;
  self->dir = initDir;
  self->up = initUp;
  self->steps = 0;
}


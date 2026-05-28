#ifndef ANT_H
#define ANT_H

#include "cglm/types-struct.h"
#include "grid.h"

typedef enum {
  TURN_LEFT,
  TURN_RIGHT,
  TURN_UP,
  TURN_DOWN,
} AntTurn;

typedef struct {
  ivec3s pos; // position
  ivec3s dir; // orientation
  ivec3s up;

  // Also can be static if the only ant
  AntTurn rules[GRID_MAX_STATES];
  u8 activeStates;

  int steps;
} Ant;

[[nodiscard]] Ant antCreateDefault();

void antUpdate(Ant* self);
void antRandomizeRules(Ant* self);

#endif


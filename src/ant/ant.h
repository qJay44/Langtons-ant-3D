#ifndef ANT_H
#define ANT_H

#include "cglm/types-struct.h"

#define ANT_MAX_STATES 5u

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
  AntTurn rules[ANT_MAX_STATES];
  int activeStates;

  int steps;
} Ant;

[[nodiscard]] Ant antCreateDefault();

void antUpdate(Ant* self);
void antRandomizeRules(Ant* self);

#endif


#ifndef ANT_H
#define ANT_H

#include "cglm/types-struct.h"

typedef struct Ant {
  ivec3s pos;    // position
  ivec3s dir; // orientation
  ivec3s up;
  int steps;
} Ant;

[[nodiscard]] Ant antCreateDefault();

void antUpdate(Ant* self);

#endif


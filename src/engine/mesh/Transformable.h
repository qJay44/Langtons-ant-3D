#ifndef TRANSFORMABLE_H
#define TRANSFORMABLE_H

#include "cglm/types-struct.h"

typedef struct {
  mat4s trans;
  mat4s rot;
  mat4s scale;
} Transformable;

#endif

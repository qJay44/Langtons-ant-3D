#ifndef UTILS_H
#define UTILS_H

// Returns dynamic array of characters (needs to be freed)
#include "cglm/types-struct.h"

[[nodiscard]] char* readFile(const char* path);

static inline ivec3s ivec3s_add(ivec3s a, ivec3s b) {
  return (ivec3s){{
    a.x + b.x,
    a.y + b.y,
    a.z + b.z
  }};
}

static inline ivec3s ivec3s_cross(ivec3s a, ivec3s b) {
  return (ivec3s){{
    a.y * b.z - a.z * b.y,
    a.z * b.x - a.x * b.z,
    a.x * b.y - a.y * b.x
  }};
}

#endif


#ifndef UTILS_H
#define UTILS_H

#include <math.h>

#include "cglm/types-struct.h"
#include "cglm/util.h"

// Returns dynamic array of characters (needs to be freed)
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

static inline vec3s getDirectionYawPitchRad(float yaw, float pitch) {
  return (vec3s){{
    cosf(yaw) * cosf(pitch),
    sinf(pitch),
    sinf(yaw) * cosf(pitch)
  }};
}

static inline vec3s getDirectionYawPitchDeg(float yaw, float pitch) {
  return getDirectionYawPitchRad(glm_rad(yaw), glm_rad(pitch));
}

#endif


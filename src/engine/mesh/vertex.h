#ifndef VERTEX_H
#define VERTEX_H

#include "cglm/types-struct.h"

// NOTE: Order: P T N
// ✓ P N, P T, etc.
// ❌T N P, T P N, etc.

typedef struct {
  GLuint size;
  GLenum type;
} VertexAttribute;

typedef struct {
  const VertexAttribute* attribs;
  size_t count;
  size_t stride;
} VertexLayout;

// ----- PT -------------------------------------------------------------------------------- //

typedef struct {
  vec3s pos;
  vec2s tex;
} VertexPT;

static const VertexAttribute PT_ATTRIBS[] = {
  {3, GL_FLOAT},
  {2, GL_FLOAT},
};

static const VertexLayout PT_LAYOUT = {PT_ATTRIBS, 2, sizeof(VertexPT)};

// ----- PN -------------------------------------------------------------------------------- //

typedef struct {
  vec3s pos;
  vec3s normal;
} VertexPN;

static const VertexAttribute PN_ATTRIBS[] = {
  {3, GL_FLOAT},
  {3, GL_FLOAT},
};

static const VertexLayout PN_LAYOUT = {PN_ATTRIBS, 2, sizeof(VertexPN)};

// ----- PTN ------------------------------------------------------------------------------- //

typedef struct {
  vec3s pos;
  vec2s tex;
  vec3s normal;
} VertexPTN;

static const VertexAttribute PTN_ATTRIBS[] = {
  {3, GL_FLOAT},
  {2, GL_FLOAT},
  {3, GL_FLOAT},
};

static const VertexLayout PTN_LAYOUT = {PTN_ATTRIBS, 3, sizeof(VertexPTN)};

// ----------------------------------------------------------------------------------------- //

#endif


#ifndef VERTEX_H
#define VERTEX_H

#include "cglm/types-struct.h"

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

// ----------------------------------------------------------------------------------------- //

#endif


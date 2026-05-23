#include "Context.h"
#include "text.h"

#include <math.h>
#include <stdio.h>

#include "../mesh/vertex.h"
#include "cglm/struct/mat4.h"
#include "cglm/struct/cam.h"
#include "cglm/affine.h"
#include "cglm/struct/vec2.h"

Text textCreate(const Font* font, float scale, const char* text) {
  Text self = {0};
  self.font = font;
  self.scale = scale;

  MeshData meshData = {0};
  meshData.verticesSize = sizeof(VertexPT) * TEXT_MAX_LEN * 6;
  meshData.layout = PT_LAYOUT;
  self.mesh = meshCreateArrays(&meshData, GL_DYNAMIC_DRAW);

  textSetText(&self, text);

  return self;
}

void textSetText(Text* self, const char* text) {
  const size_t textLen = strlen(text);
  if (textLen > TEXT_MAX_LEN) {
    fprintf(stderr, "[textSetText] ❌Text length [%zu] exceeds limit [%u]\n", textLen, TEXT_MAX_LEN);
    exit(EXIT_FAILURE);
  }

  float cursorX = 0.f;

  MeshData meshData = {0};
  VertexPT vertices[TEXT_MAX_LEN * 6];
  size_t verticesCount = 0;

  for (size_t i = 0; i < textLen; i++) {
    if (text[i] == '\0')
      break;

    FontGlyph g = self->font->glyphs[(int)text[i]];
    size_t triIdx = i * 6;

    float x = cursorX + g.bearingX * self->scale;
    float y = (g.height - g.bearingY) * self->scale;
    float w = g.width * self->scale;
    float h = g.height * self->scale;

    vertices[triIdx + 0] = (VertexPT){{{x    , y + h, 0.f}}, {{g.uv0.x, g.uv0.y}}};
    vertices[triIdx + 1] = (VertexPT){{{x    , y    , 0.f}}, {{g.uv0.x, g.uv1.y}}};
    vertices[triIdx + 2] = (VertexPT){{{x + w, y    , 0.f}}, {{g.uv1.x, g.uv1.y}}};

    vertices[triIdx + 3] = (VertexPT){{{x    , y + h, 0.f}}, {{g.uv0.x, g.uv0.y}}};
    vertices[triIdx + 4] = (VertexPT){{{x + w, y    , 0.f}}, {{g.uv1.x, g.uv1.y}}};
    vertices[triIdx + 5] = (VertexPT){{{x + w, y + h, 0.f}}, {{g.uv1.x, g.uv0.y}}};

    cursorX += g.advance * self->scale;
    self->rectSize.y = fmaxf(self->rectSize.y, h);
    verticesCount += 6;
  }

  meshData.vertices = (float*)vertices;
  meshData.verticesSize = sizeof(VertexPT) * verticesCount;
  meshData.layout = PT_LAYOUT;

  self->rectSize.x = cursorX;
  meshUpdateBufferVBO(&self->mesh, &meshData, 0);
}

void textSetPos(Text* self, vec2s pos) {
  vec3s translate = {0};
  translate.x = pos.x;
  translate.y = pos.y;

  glm_translate_make(self->mesh.mats.trans.raw, translate.raw);
}

void textSetPosRelative(Text* self, vec2s pos) {
  vec2s winSize = getWinSizef();
  pos.x *= winSize.x;
  pos.y *= winSize.y;

  textSetPos(self, pos);
}

void textDraw(Text* self, const Camera* cam, Shader* shader) {
  vec2s winSize = getWinSizef();
  mat4s proj = glms_ortho(0.f, winSize.x, 0.f, winSize.y, -1.f, 1.f);

  shaderSetUniformMat4f(shader, "u_proj", proj.raw);
  texture2D_bind(self->font->atlas, 0);

  glDepthMask(GL_FALSE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);

  meshDrawArrays(&self->mesh, cam, shader);

  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);
}


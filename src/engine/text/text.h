#ifndef TEXT_H
#define TEXT_H

#include "font.h"
#include "../mesh/mesh.h"

#define TEXT_MAX_LEN 256u

typedef struct {
  const Font* font;
  float scale;
  vec2s rectSize;
  MeshArrays mesh;
} Text;

Text textCreate(const Font* font, float scale, const char* text);

void textSetText(Text* self, const char* text);
void textSetPos(Text* self, vec2s pos);
void textSetPosRelative(Text* self, vec2s pos);
void textDraw(Text* self, const Camera* cam, Shader* shader);

#endif


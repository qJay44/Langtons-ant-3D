#ifndef TEXT_H
#define TEXT_H

#include "font.h"
#include "../mesh/mesh.h"

#define TEXT_MAX_LEN 256u

typedef struct {
  const Font* font;
  vec2s rectSize;
  MeshArrays mesh;
} Text;

Text textCreate(const Font* font, const char* text);

void textSetText(Text* self, const char* text);
void textSetTextFmt(Text* self, const char* format, ...) __attribute__((format(printf, 2, 3)));
void textSetTexti(Text* self, int num);
void textSetPos(Text* self, vec2s pos);
void textSetPosRelative(Text* self, vec2s pos);
void textSetPosUnderOther(Text* self, const Text* other, vec2s offset);
void textDraw(Text* self, const Camera* cam, Shader* shader);

#endif


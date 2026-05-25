#ifndef FONT_H
#define FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include "cglm/types-struct.h"
#include "../texture/texture2D.h"

typedef struct {
  vec2s uv0, uv1;
  u32 width, height;
  FT_Int bearingX, bearingY;
  FT_Pos advance;
} FontGlyph;

typedef struct {
  FT_Library ft;
  FT_Face face;
  FontGlyph glyphs[128];
  Texture2D atlas;
} Font;

Font fontCreate(const char* fontPath, u32 height, u32 width);

void fontLoad(Font* self);

#endif


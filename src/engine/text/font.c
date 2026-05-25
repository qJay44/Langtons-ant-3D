#include "font.h"

#include <stdio.h>

#include "cglm/types-struct.h"

Font fontCreate(const char* fontPath, u32 height, u32 width) {
  Font font = {0};

  if (FT_Init_FreeType(&font.ft)) {
    fprintf(stderr, "❌[fonInit] Could not initialize FreeType Library");
    exit(EXIT_FAILURE);
  }

  if (FT_New_Face(font.ft, fontPath, 0, &font.face)) {
    fprintf(stderr, "❌[fonInit] Failed to load font [%s]", fontPath);
    exit(EXIT_FAILURE);
  }

  FT_Set_Pixel_Sizes(font.face, width, height);
  fontLoad(&font);

  return font;
}

void fontLoad(Font* self) {
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  TextureDescriptor texDesc = texture2D_defaultDesc;
  texDesc.internalFormat = GL_RED;
  texDesc.format = GL_RED;

  u32 atlasSize = 512;
  float atlasSizeInv = 1.f / (float)atlasSize;

  self->atlas = texture2D_createEmpty(&texDesc, atlasSize, atlasSize);
  int atlasX = 0;
  int atlasY = 0;
  u32 maxRowHeight = 0;
  texture2D_bind(self->atlas, 0);

  for (u8 c = 0; c < 128; c++) {
    if (FT_Load_Char(self->face, c, FT_LOAD_DEFAULT)) {
      fprintf(stderr, "[fontLoad] ⚠️Failed to load glyph [%u]", c);
      continue;
    }

    FT_Render_Glyph(self->face->glyph, FT_RENDER_MODE_SDF);

    FT_Bitmap bmp = self->face->glyph->bitmap;
    FontGlyph glyph = {0};

    if (atlasX + bmp.width >= atlasSize) {
      atlasX = 0;
      atlasY += maxRowHeight + 1;
      maxRowHeight = 0;
    }

    if (bmp.width > 0 && bmp.rows > 0)
      glTexSubImage2D(GL_TEXTURE_2D, 0, atlasX, atlasY, bmp.width, bmp.rows, GL_RED, GL_UNSIGNED_BYTE, bmp.buffer);

    glyph.uv0.x = (float)atlasX * atlasSizeInv;
    glyph.uv0.y = (float)atlasY * atlasSizeInv;
    glyph.uv1.x = (float)(atlasX + bmp.width) * atlasSizeInv;
    glyph.uv1.y = (float)(atlasY + bmp.rows) * atlasSizeInv;

    glyph.width = bmp.width;
    glyph.height = bmp.rows;
    glyph.bearingX = self->face->glyph->bitmap_left;
    glyph.bearingY = self->face->glyph->bitmap_top;
    glyph.advance = self->face->glyph->advance.x >> 6;

    self->glyphs[c] = glyph;

    atlasX += bmp.width + 1;
    if (bmp.rows > maxRowHeight)
      maxRowHeight = bmp.rows;
  }

  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}


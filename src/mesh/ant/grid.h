#ifndef GRID_H
#define GRID_H

#define GRID_DIM_SIZE 0xff

#define IX(x, y, z) ((x) + (y) * GRID_DIM_SIZE + (z) * GRID_DIM_SIZE * GRID_DIM_SIZE)

enum BlockColor : u32 {
  BLOCK_COLOR1 = 0xffbe0b, // #ffbe0b
  BLOCK_COLOR2 = 0xfb5607, // #fb5607
  BLOCK_COLOR3 = 0xff006e, // #ff006e
  BLOCK_COLOR4 = 0x8338ec, // #8338ec
  BLOCK_COLOR5 = 0x3a86ff, // #3a86ff
};

struct Cell {
  vec3s translateVal;
  u32 idx;
  enum BlockColor color;
};

typedef struct{
  struct Cell* cells;
  size_t sz;
  u32 idx;
} Grid;

[[nodiscard]] Grid gridCreate(u32 elemCount);

void gridAdd(Grid* self, struct Cell c);

#endif

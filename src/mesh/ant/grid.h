#ifndef GRID_H
#define GRID_H

#define GRID_DIM_SIZE 0xff

#define IX(x, y, z) ((x) + (y) * GRID_DIM_SIZE + (z) * GRID_DIM_SIZE * GRID_DIM_SIZE)

extern vec3s colors[5];

enum BlockColorIndex : u8 {
  BLOCK_COLOR_IDX_0,
  BLOCK_COLOR_IDX_1,
  BLOCK_COLOR_IDX_2,
  BLOCK_COLOR_IDX_3,
  BLOCK_COLOR_IDX_4,
};

struct Cell {
  vec3s translateVal;
  u32 idx;
  enum BlockColorIndex bci;
};

typedef struct {
  struct Cell* cells;
  size_t sz;
  u32 idx;
} Grid;

[[nodiscard]] Grid gridCreate(u32 elemCount);

void gridAdd(Grid* self, struct Cell c);

#endif

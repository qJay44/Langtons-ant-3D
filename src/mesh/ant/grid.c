#include "grid.h"

#include <stdlib.h>

Grid gridCreate(u32 elemCount) {
  Grid grid;
  grid.sz = sizeof(struct Cell) * elemCount,
  grid.cells = malloc(grid.sz);
  grid.idx = 0;

  return grid;
}

void gridAdd(Grid* self, struct Cell c) {
  if (self->idx == self->sz / sizeof(struct Cell)) {
    struct Cell* newGrid = malloc(self->sz * 2);
    for (u32 i = 0; i < self->idx; i++)
      newGrid[i] = self->cells[i];
    free(self->cells);
    self->sz *= 2;
    self->cells = newGrid;
  }

  self->cells[self->idx++] = c;
}


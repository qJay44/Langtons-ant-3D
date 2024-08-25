#include "ant.h"

Ant antCreate(vec2s pos) {
  Ant ant = {
    .pos = pos,
    .dir = FORWARD,
    .steps = 0
  };

  return ant;
}

void antTurnLeft();
void antTurnRight();
void antTurnUp();
void antTurnDown();
void antMove();



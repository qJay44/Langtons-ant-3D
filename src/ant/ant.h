#ifndef ANT_H
#define ANT_H

enum Direction : s8 {
  FORWARD, BACK, LEFT, RIGHT,
  TOP, BOTTOM
};

typedef struct {
  enum Direction dir;
  vec2s pos;
  u32 steps;
} Ant;

[[nodiscard]] Ant antCreate(vec2s pos);

void antTurnLeft();
void antTurnRight();
void antTurnUp();
void antTurnDown();
void antMove();

#endif


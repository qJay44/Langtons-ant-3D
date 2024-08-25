#ifndef STATE_H
#define STATE_H

struct State {
  float nearPlane;
  float farPlane;
  float winWidth;
  float winHeight;
  float time;
};

extern struct State _gState;

#endif


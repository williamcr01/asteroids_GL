#include "ship.h"

struct Bullet {
  Vector2 p1;
  Vector2 p2;
  float angle;
  float velocity;
  int id;
};
#include "../util/pointManip.h"

struct Ship {
  Vector2 p1;
  Vector2 p2;
  Vector2 p3;
  Vector2 p4;
  Vector2 p5;
  float angle;
  float rotationAngle;
  float currVelocity;
  float shipVelocity;
};

void rotateShip(struct Ship *ship);
Vector2 checkShipOutOfBounds(struct Ship *ship);
struct Ship initShip();
#include "ship.h"

Vector2 getShipCenter(struct Ship *ship) {
  float centerX =
      (ship->p1.x + ship->p2.x + ship->p3.x + ship->p4.x + ship->p5.x) / 5.0f;
  float centerY =
      (ship->p1.y + ship->p2.y + ship->p3.y + ship->p4.y + ship->p5.y) / 5.0f;
  Vector2 centerPoint = {centerX, centerY};
  return centerPoint;
}

Vector2 checkShipOutOfBounds(struct Ship *ship) {
  Vector2 returnVec = {0, 0};
  Vector2 centerPoint = getShipCenter(ship);
  if(centerPoint.x > 1){
    if(ship->p1.x > 1 && ship->p4.x > 1 && ship->p5.x > 1) {
      returnVec.x = 1;
    }
  } else if(centerPoint.x < -1) {
    if(ship->p1.x < -1 && ship->p4.x < -1 && ship->p5.x < -1) {
      returnVec.x = -1;
    }
  } else if(centerPoint.y > 1) {
    if(ship->p1.y > 1 && ship->p4.y > 1 && ship->p5.y > 1) {
      returnVec.y = 1;
    }
  } else if (centerPoint.y < -1) {
    if(ship->p1.y < -1 && ship->p4.y < -1 && ship->p5.y < -1) {
      returnVec.y = -1;
    }
  }
  return returnVec;
}

void rotateShip(struct Ship *ship) {
  Vector2 centerPoint = getShipCenter(ship);

  rotatePoint(ship->rotationAngle, &ship->p1, centerPoint);
  rotatePoint(ship->rotationAngle, &ship->p2, centerPoint);
  rotatePoint(ship->rotationAngle, &ship->p3, centerPoint);
  rotatePoint(ship->rotationAngle, &ship->p4, centerPoint);
  rotatePoint(ship->rotationAngle, &ship->p5, centerPoint);
}
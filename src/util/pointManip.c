#include "pointManip.h"

void rotatePoint(float angle, Vector2 *point, Vector2 centerPoint) {
    point->x -= centerPoint.x;
    point->y -= centerPoint.y;
    float rotationMatrix[] = {cosf(angle), -sinf(angle), sinf(angle),
                              cosf(angle)};
    float newX = rotationMatrix[0] * point->x + rotationMatrix[1] * point->y;
    float newY = rotationMatrix[2] * point->x + rotationMatrix[3] * point->y;
    point->x = newX + centerPoint.x;
    point->y = newY + centerPoint.y;
}

void setPoint(float newX, float newY, Vector2 *point) {
    point->x = newX;
    point->y = newY;
}

void movePoint(float angle, float velocity, Vector2 *point) {
    float dx = velocity * cosf(angle);
    float dy = velocity * sinf(angle);
    point->x += dx;
    point->y += dy;
}
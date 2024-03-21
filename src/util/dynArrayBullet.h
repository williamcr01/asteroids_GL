#include <stdlib.h>
#include "../entity/bullet.h "

typedef struct {
    struct Bullet *array;
    size_t used;
    size_t size;
} dynArray;

void initDynArrayBullet(dynArray *arr, size_t initialSize);
void insertDynArrayBullet(dynArray *arr, struct Bullet element);
void removeElementBullet(dynArray *arr, int index);
void freeDynArrayBullet(dynArray *arr);
#include "dynArrayBullet.h"

// init dynamic array, initialSize cant be 0
void initDynArrayBullet(dynArray *a, size_t initialSize) {
    a->array = (struct Bullet *)malloc(initialSize * sizeof(struct Bullet));
    a->used = 0;
    a->size = initialSize;
}

void insertDynArrayBullet(dynArray *a, struct Bullet element) {
    if (a->array == NULL) {
        initDynArrayBullet(a, 1);
        insertDynArrayBullet(a, element);
    } else {
        if (a->used == a->size) {
            a->size *= 2;
            a->array =
                (struct Bullet *)realloc(a->array, a->size * sizeof(element));
        }
        a->array[a->used++] = element;
    }
}

void removeElementBullet(dynArray *a, int index) {
    if (a->used == 1) {
        freeDynArrayBullet(a);
    } else {
        for (int i = index; i < a->used - 1; i++) {
            a->array[i] = a->array[i + 1];
        }
        a->used--;
    }
}

void freeDynArrayBullet(dynArray *a) {
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}

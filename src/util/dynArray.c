#include "dynArray.h"


inline void initDynArray(dynArray* a, size_t initialSize) {
    a->array = (int*)malloc(initialSize * sizeof(int));
    a->used = 0;
    a->size = initialSize;
}

void insertDynArray(dynArray* a, int element) {
    if (a->used == a->size) {
        a->size *= 2;
        a->array = (int*)realloc(a->array, a->size * sizeof(int));
    }
    a->array[a->used++] = element;
}

void removeElement(dynArray* a, int index) {
    for (int i = index; i < a->used - 1; i++) {
        a->array[i] = a->array[i + 1];
    }
    a->used--;
}

void freeDynArray(dynArray* a) {
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}

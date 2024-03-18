#include <stdlib.h>

typedef struct {
    int* array;
    size_t used;
    size_t size;
} dynArray;

void initDynArray(dynArray* arr, size_t initialSize);
void insertDynArray(dynArray* arr, int element);
void removeElement(dynArray* arr, int index);
void freeDynArray(dynArray* arr);
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

struct Vector {
   char* data;
   size_t elementSize;
   uint64_t capacity;
   uint64_t count;
};

struct Vector * vectorInitialize(size_t elementSize, uint64_t arr_length, void* arr[]){


    uint64_t capacity = 16;
    for (int i = 4; i < 32; i++) {
        if (capacity < (arr_length * 2)) {
            capacity = capacity << 1;
        }
    }
    if (capacity < arr_length) return NULL; 

    char * data = calloc(capacity, elementSize);
    if (data == NULL) return NULL; 

    struct Vector v = {
        data,
        elementSize,
        capacity,
        0
    };
    if (arr != NULL) { 
        memcpy(v.data, arr, (arr_length * elementSize)); 
        v.count = arr_length;
    };
    struct Vector * res = malloc(sizeof(struct Vector));
    if (res == NULL) return NULL;
    memcpy(res, &v, sizeof(struct Vector)); 
    return res;
}

void * vectorAtIndex(struct Vector * v, uint64_t index){
    if (v == NULL) return NULL;
    if (index >= v->count) return NULL;
    char * p = v->data; 
    p += v->elementSize * index;
    void * res = calloc(1, v->elementSize);
    memcpy(res, p, v->elementSize);
    return res;
}

int vectorPush(struct Vector * v, void * item){
    if (v == NULL) return -1;
    if (v->count >= v->capacity - 1){
        if (v->capacity >= UINT64_MAX / 2) return -1;
        v->capacity = v->capacity * 2; 
        v->data = reallocarray(v->data, v->capacity, v->elementSize);
        if (v->data == NULL) return -1;
    }
    char * p = v->data; 
    p += v->elementSize * v->count;
    v->count += 1;
    memcpy(p, item, v->elementSize);
    return 0;
} 

void * vectorPop(struct Vector * v){
    if (v == NULL) return NULL;
    if (v->count == 0) return NULL;
    if (v->count <= v->capacity / 4 && v->capacity > 16){
        v->capacity = v->capacity / 2;
        v->data = reallocarray(v->data, v->capacity, v->elementSize);
        if (v->data == NULL) return NULL;
    }
    void * p = vectorAtIndex(v, v->count - 1);
    v->count -= 1;
    return p;
} 

int vectorInsert(struct Vector * v, void * item, uint64_t index){
    if (v == NULL) return -1;
    if (index > v->count) return -1;
    if (v->count >= v->capacity - 1){
        if (v->capacity >= UINT64_MAX / 2) return -1;
        v->capacity = v->capacity * 2; 
        v->data = reallocarray(v->data, v->capacity, v->elementSize);
        if (v->data == NULL) return -1;
    }
    if (index == v->count) return vectorPush(v, item);
    uint64_t moveLength = (v->count - index) * v->elementSize;
    char * offset = v->data + (index * v->elementSize);
    memmove( offset+v->elementSize, offset, moveLength);
    memcpy(offset, item, v->elementSize);
    v->count += 1;
    return 0;
}

int vectorDelete(struct Vector * v, uint64_t index){
    if (index >= v->count) return -1;
    if (v->count <= v->capacity / 4 && v->capacity > 16){
        v->capacity = v->capacity / 2;
        v->data = reallocarray(v->data, v->capacity, v->elementSize);
        if (v->data == NULL) return -1;
    }
    uint64_t moveLength = (v->count - index - 1) * v->elementSize;
    char * offset = v->data + (index * v->elementSize);
    memmove( offset, offset+v->elementSize, moveLength);
    v->count -= 1;
    return 0;
}

uint64_t vectorFind(struct Vector * v, void * target){
    if (target == NULL) return -1;
    char * p = (char *) v->data;
    for (uint64_t i = 0; i < v->count; i++){
        if (memcmp((void *) p, target, v->elementSize) == 0) return i;
        p += v->elementSize;
    }
    return -1;
}

int vectorRemove(struct Vector * v, void * target){
    uint64_t targetIndex = vectorFind(v, target);
    if (targetIndex < 0) return targetIndex;
    return vectorDelete(v, targetIndex);
}

uint64_t vectorSize(struct Vector * v){
    if (v == NULL) return -1;
    return v->count;
}

uint64_t vectorCapacity(struct Vector * v){
    if (v == NULL) return -1;
    return v->capacity;
}

int vectorIsEmpty(struct Vector * v){
    if (v == NULL) return -1;
    return (v->count == 0);
}

int vectorPrepend(struct Vector * v, void * item){
    return vectorInsert(v, item, 0);
}

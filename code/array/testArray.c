#include "array.c"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int assert(int assertion, char * text){
    if (assertion == 1){
        printf("%s - SUCCESS \n", text);
    }else{
        printf("%s - FAILURE \n", text);
    }
    return assertion;
}

int main(void){
    uint32_t * arr = calloc(1000, sizeof(uint32_t));
    for (int i = 0; i < 1000; i++) {
        arr[i] = rand();
    }
    struct Vector * test 
        = vectorInitialize(sizeof(uint32_t), 1000, (void *) arr);
    assert(test->capacity == 2048, "Cap is 2048");
    assert(test->count == 1000, "Count is 4");
    assert(test->elementSize == sizeof(uint32_t), "Size is 4 byte");
    assert(memcmp(test->data, arr, test->elementSize * test->count) == 0, "Arr is copied");
    for (int i = 0; i < 512; i++) {
        uint32_t * t = vectorPop(test);
        assert(*t == arr[999-i], "Pop");
    }
    assert(test->capacity == 1024, "Cap is now 1024");
    
}

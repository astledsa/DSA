#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint32_t length;
    uint32_t num_elements;
}ArrInfo;

void copyArray(int* source, int* dest, int n) {
    memcpy(dest, source, n * sizeof(int));
}

void** DynamicArray (int first) {
    ArrInfo* p1 = (ArrInfo*)malloc(sizeof(ArrInfo));
    p1->length = 1;
    p1->num_elements = 1;

    void** d_array = (void**)malloc(2*sizeof(void*));
    int* data = (int*)malloc(p1->length * sizeof(int));
    data[0] = first;
    d_array[0] = p1;
    d_array[1] = data;

    return d_array;
}

void Append (void** dArray, int element) {
    ArrInfo* info = (ArrInfo*)dArray[0];
    int* data = (int*)dArray[1];

    if (info->length == info->num_elements) {
        info->length *= 2;
        int* newData = (int*)malloc(info->length * sizeof(int));
        copyArray(data, newData, info->num_elements);

        newData[info->num_elements] = element;
        dArray[1] = newData;
        info->num_elements += 1;
        
        free(data);
        return;
    }
    data[info->num_elements] = element;
    info->num_elements += 1;
}

void print_array (void** dArray) {
    ArrInfo* info = (ArrInfo*)dArray[0];
    int* data = (int*)dArray[1];

    for (uint32_t i = 0; i < info->num_elements; i ++) {
        printf("%d\n", data[i]);
    }
}

int main () {
    void** arr = DynamicArray(0);
    print_array(arr);
}
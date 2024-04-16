#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int* data;
    unsigned int * shape;
    unsigned int totalLength;
    unsigned int num_dimensions;
}NDarray;

void PrintNDarray (NDarray* ndarray) {
    for (int i = 0; i < ndarray->totalLength; i++) {
        printf("%d\n", ndarray->data[i]);
    }
}

void PrintShape (NDarray* ndarray) {
    printf("{ ");
    for (int i = 0; i < ndarray->num_dimensions; i++) {
        printf("%d ", ndarray->shape[i]);
    }
    printf("}");
}

int NDIndex_to_1DIndex (unsigned int* indexes, unsigned int* shape, ssize_t size) {
    int linear_index = 0;
    int stride = 1;
    
    for (int i = 0; i < size; i++) {
        if (indexes[i] < 0 || indexes[i] >= shape[i]) {
            return -1;
        }
        
        linear_index += indexes[i] * stride;
        stride *= shape[i];
    }
    
    return linear_index;
}

NDarray* createMatrix (unsigned int* shape, ssize_t size) {

    unsigned int arrLength = 1;
    for (size_t i = 0; i < size; i++) {
        arrLength *= shape[i];
    }

    NDarray* ndarray = (NDarray*)malloc(sizeof(NDarray));
    ndarray->data = (int*)malloc(arrLength * sizeof(int));
    ndarray->shape = (unsigned int*)malloc(size * sizeof(unsigned int));

    for (size_t i = 0; i < arrLength; i++) {
        ndarray->data[i] = 0;
    }
    for (size_t i = 0; i < size; i++) {
        ndarray->shape[i] = shape[i];
    }
    ndarray->totalLength = arrLength;
    ndarray->num_dimensions = size;

    return ndarray;
}

NDarray* ElementWiseOperation (NDarray* m1, NDarray* m2, char operation[]) {
    if (m1->totalLength != m2->totalLength || m1->num_dimensions != m2->num_dimensions) {
        return NULL;
    }

    char add[] = "add";
    char sub[] = "sub";
    char mult[] = "mult";
    NDarray* m3 = createMatrix (m1->shape, m1->num_dimensions);

    if (strcmp(operation, add) == 0) {
        for (size_t i = 0; i < m1->num_dimensions; i++) {
            m3->data[i] = m1->data[i] + m2->data[i];
        }
    }
    if (strcmp(operation, sub) == 0) {
        for (size_t i = 0; i < m1->num_dimensions; i++) {
            m3->data[i] = m1->data[i] - m2->data[i];
        }
    }
    if (strcmp(operation, mult) == 0) {
        for (size_t i = 0; i < m1->num_dimensions; i++) {
            m3->data[i] = m1->data[i] * m2->data[i];
        }
    }

    return m3;
}

NDarray* Add (NDarray* m1, NDarray* m2) {
    char add[] = "add";
    return ElementWiseOperation(m1, m2, add);
}

NDarray* Subract (NDarray* m1, NDarray* m2) {
    char sub[] = "sub";
    return ElementWiseOperation(m1, m2, sub);
}

NDarray* Mult (NDarray* m1, NDarray* m2) {
    char mult[] = "mult";
    return ElementWiseOperation(m1, m2, mult);
}

void InsertValue (NDarray* ndarray, unsigned int* indexes, int value) {
    int index_to_store = NDIndex_to_1DIndex(indexes, ndarray->shape, ndarray->num_dimensions);
    ndarray->data[index_to_store] = value;
}

void InsertRange (NDarray* ndarray, unsigned int* from, unsigned int* to, unsigned int* values) {
    int from_index = NDIndex_to_1DIndex(from, ndarray->shape, ndarray->num_dimensions);
    int to_index = NDIndex_to_1DIndex(to, ndarray->shape, ndarray->num_dimensions);

    if (to_index < from_index) {
        printf("Wrong indexes provided");
    }

    int Range = to_index - from_index;

    for (int i = from_index; i < to_index+1; i++) {
        ndarray->data[i] = values[i - from_index];
    }
}

int main () {
    unsigned int shape[] = {2, 2};
    ssize_t num_dimensions = sizeof(shape) / sizeof(shape[0]);
    NDarray* m1 = createMatrix(shape, num_dimensions);
    PrintNDarray(m1);
}
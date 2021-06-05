/**
 * <------------------------------------------------------------------>
 * @name    2D Engine
 * @author  Martin Krcma
 * @date    28. 2. 2021
 * <------------------------------------------------------------------>
 * @file    vector.h
 * @brief
 * <------------------------------------------------------------------>
 */

#include "vector.h"

#include "util.h"
#include <stdlib.h>


static void quicksort(Vector_Element ** elements, Vector_comparator * cmp, int first, int last);


bool Vector_init(Vector * vector, unsigned int size, unsigned int alloc_step) {
    if(vector == NULL) return false;

    vector->alloc_step = alloc_step;
    vector->count = 0;
    vector->size = size;
    vector->elements = malloc(sizeof(Vector_Element*) * size);

    return vector->elements != NULL;
}

bool Vector_clear(Vector * vector) {
    if(vector == NULL) return false;

    if(vector->elements != NULL) {
        Vector_Element * e;
        for(unsigned int i = 0; i < vector->count && i < vector->size; ++i) {
            e = *(vector->elements + i);
            if(e != NULL) {
                if(e->destruct) e->destruct(e->ptr);
                *(vector->elements + i) = NULL;
            }
        }
    }
    vector->count = 0;

    return true;
}

bool Vector_destruct(Vector * vector) {
    if(vector == NULL) return false;
    if(vector->elements == NULL) return false;

    Vector_Element * e;
    for(unsigned int i = 0; i < vector->count && i < vector->size; ++i) {
        e = *(vector->elements + i);
        if(e != NULL) {
            if(e->destruct) e->destruct(e->ptr);
            free(e);
            *(vector->elements + i) = NULL;
        }
    }

    free(vector->elements);
    vector->elements = NULL;

    return true;
}

Vector_Element * Vector_get(Vector * vector, unsigned int index) {
    if(vector == NULL) return NULL;
    if(vector->elements == NULL) return NULL;
    if(index >= vector->size) return NULL;

    return vector->elements[index];
}

bool Vector_remove(Vector * vector, unsigned int index) {
    if(vector == NULL) return false;
    if(vector->elements == NULL) return false;

    if(vector->count != 0) {
        Vector_Element * e = vector->elements[index];
        if(e != NULL) {
            if(e->destruct) e->destruct(e->ptr);
            free(e);
        }
        for(unsigned int i = index; i + 1 < vector->count && i + 1 < vector->size; ++i) {
            vector->elements[i] = vector->elements[i + 1];
        }
        vector->count -= 1;
    }

    return true;
}

bool Vector_append(Vector * vector, Vector_Element * element) {
    if(vector == NULL) return false;
    if(vector->elements == NULL) return false;

    if(vector->count >= vector->size) {
        unsigned int nSize = vector->size + vector->alloc_step;
        Vector_Element ** nArray = realloc(vector->elements, sizeof(Vector_Element*) * nSize);
        if(nArray == NULL) return false;
        vector->elements = nArray;
        vector->size = nSize;
    }
    vector->elements[vector->count] = element;
    vector->count += 1;

    return true;
}

bool Vector_contains(Vector * vector, Vector_Element * element) {
    if(vector == NULL) return false;
    if(vector->elements == NULL) return false;

    for(unsigned int i = 0; i < vector->count && i < vector->size; ++i) {
        if(vector->elements[i] == element) {
            return true;
        }
    }

    return false;
}

bool Vector_shuffle(Vector * vector) {
    if(vector == NULL) return false;
    if(vector->elements == NULL) return false;

    unsigned int rnd;
    Vector_Element * tmp;
    for(unsigned int i = 0; i < vector->count && i < vector->size; ++i) {
        rnd = UTIL_randInt(vector->count);
        if(rnd < vector->count) {
            tmp = vector->elements[i];
            vector->elements[i] = vector->elements[rnd];
            vector->elements[rnd] = tmp;
        }
    }

    return true;
}

bool Vector_sort(Vector * vector, Vector_comparator * cmp) {
    if(vector == NULL || cmp == NULL) return false;
    if(vector->elements == NULL) return false;

    quicksort(vector->elements, cmp, 0, vector->count);

    return true;
}

static void quicksort(Vector_Element ** elements, Vector_comparator * cmp, int first, int last){
    int i, j, pivot;
    Vector_Element * temp;
    if(first < last){

        pivot = first;
        i = first;
        j = last;
        while(i < j){
            while((*cmp)(elements[i], elements[pivot]) < 1 && i < last)
                i++;
            while((*cmp)(elements[i], elements[pivot]) == GREATHER)
                j--;
            if(i < j){
                temp = elements[i];
                elements[i] = elements[j];
                elements[j] = temp;
            }
        }

        temp = elements[pivot];
        elements[pivot] = elements[j];
        elements[j] = temp;

        quicksort(elements, cmp, first, j - 1);
        quicksort(elements, cmp, j + 1, last);
    }
}

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

#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>


//element of vector
typedef struct _Vector_Element {
    void * ptr; /** pointer at data of element */
    void (*destruct)(void * ptr); /** pointer at destructor of data (ptr = pointer at data) */
} Vector_Element;

//vector struct
typedef struct {
    Vector_Element ** elements; /** elements of vector (array of pointers) */
    unsigned int size; /** size of vector */
    unsigned int count; /** number of elements in vector */
    unsigned int alloc_step; /** size of alloc step */
} Vector;

//result of comparator
typedef enum {
    EQUAL = 0,  /** both elements are equal*/
    GREATHER = 1,   /** first element is greather than second*/
    SMALLER = -1,   /** first element is smaller than second*/
    UNDEFINED = 2   /** undefined*/
} CMP;

/**
 * @brief Compare two elements of vector
 * @param el1   First element
 * @param el2   Second element
 * @return 0 [equal value], 1 [first is greater], -1 [second is greater], 2 [undefined]
 */
typedef CMP (*Vector_comparator)(Vector_Element * el1, Vector_Element * el2);


/**
 * @brief Vector_init
 * @param vector
 * @param size
 * @param alloc_step
 * @return
 */
bool Vector_init(Vector * vector, unsigned int size, unsigned int alloc_step);

/**
 * @brief Vector_clear
 * @param vector
 * @return
 */
bool Vector_clear(Vector * vector);

/**
 * @brief Vector_destruct
 * @param vector
 * @return
 */
bool Vector_destruct(Vector * vector);

/**
 * @brief Vector_get
 * @param vector
 * @param index
 * @return
 */
Vector_Element * Vector_get(Vector * vector, unsigned int index);

/**
 * @brief Vector_remove
 * @param vector
 * @param index
 * @return
 */
bool Vector_remove(Vector * vector, unsigned int index);

/**
 * @brief Vector_append
 * @param vector
 * @param element
 * @return
 */
bool Vector_append(Vector * vector, Vector_Element * element);

/**
 * @brief Vector_contains
 * @param vector
 * @param element
 * @return
 */
bool Vector_contains(Vector * vector, Vector_Element * element);

/**
 * @brief Vector_shuffle
 * @param vector
 * @return
 */
bool Vector_shuffle(Vector * vector);

/**
 * @brief Vector_sort
 * @param vector
 * @param cmp
 * @return
 */
bool Vector_sort(Vector * vector, Vector_comparator * cmp);


#endif // VECTOR_H

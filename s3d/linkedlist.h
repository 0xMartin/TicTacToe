/**
 * <------------------------------------------------------------------>
 * @name    2D Engine
 * @author  Martin Krcma
 * @date    28. 2. 2021
 * <------------------------------------------------------------------>
 * @file    linkedlist.h
 * @brief   LinkedList consist form elements, each element have link
 *          on next element in list
 * <------------------------------------------------------------------>
 */

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdbool.h>


//element of linked list
typedef struct _LinkedList_Element {
    void * ptr; /** pointer at data of element */
    void (*destruct)(void * ptr); /** pointer at destructor of data (ptr = pointer at data)*/
    struct _LinkedList_Element * next; /** next element in list */
} LinkedList_Element;


//linked list
typedef struct {
    LinkedList_Element * first; /** first element of list */
    LinkedList_Element * last; /** last element of list */
    unsigned int count; /** number of elements in list */
} LinkedList;


/**
 * @brief Init LinkedList structure
 * @param list  Pointer at LinkedList structure
 * @return True -> success
 */
bool LinkedList_init(LinkedList * list);

/**
 * @brief Insert new element to list
 * @param list      Pointer at LinkedList structure
 * @param element   Pointer at new element (create copy of element)
 * @return True -> success
 */
bool LinkedList_insert(LinkedList * list, LinkedList_Element * element);

/**
 * @brief Return element with specified index
 * @param list  Pointer at LinkedList structure
 * @param index Index of element
 * @return Element on index
 */
LinkedList_Element * LinkedList_valueAt(LinkedList * list, unsigned int index);

/**
 * @brief Return following element after current element
 * @param element   Pointer at current element
 * @return Next element in list
 */
LinkedList_Element * LinkedList_next(LinkedList_Element * element);

/**
 * @brief Remove element from list
 * @param list  Pointer at LinkedList structure
 * @param item  Pointer at item
 * @param destruct  True -> destruct data of removed element
 * @return
 */
bool LinkedList_removeElement(LinkedList * list, LinkedList_Element * element, bool destruct);

/**
 * @brief Remove element from list
 * @param list      Pointer at LinkedList structure
 * @param index     Index of element that will be removed
 * @param destruct  True -> destruct data of removed element
 * @return True -> success
 */
bool LinkedList_removeIndex(LinkedList * list, unsigned int index, bool destruct);

/**
 * @brief Check if list contains element
 * @param list      Pointer at LinkedList structure
 * @param element   Pointer at element
 * @return
 */
bool LinkedList_contains(LinkedList * list, LinkedList_Element * element);

/**
 * @brief Clear linked list (call destructor of each element in list), LinkedList struct is not freed
 * @param list  Pointer at LinkedList structure
 */
void LinkedList_destruct(LinkedList * list);

#endif // LINKEDLIST_H

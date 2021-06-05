/**
 * <------------------------------------------------------------------>
 * @name    2D Engine
 * @author  Martin Krcma
 * @date    28. 2. 2021
 * <------------------------------------------------------------------>
 * @file    engine_object.c
 * @brief   Implementation of engine_object.h
 * <------------------------------------------------------------------>
 */

#include "engine_object.h"

bool E_Obj_init(E_Obj * obj) {
    if(obj == NULL) return false;
    obj->data = NULL;
    obj->events = NULL;
    return true;
}

void E_Obj_destruct(E_Obj * obj) {
    if(obj == NULL) return;
    if(obj->events == NULL) return;
    if(obj->events->destruct) obj->events->destruct(obj);
}

bool E_Obj_insertToList(LinkedList * list, E_Obj * obj) {
    if(obj == NULL || list == NULL) return false;

    LinkedList_Element * element = malloc(sizeof(LinkedList_Element));
    element->ptr = obj;
    element->destruct = obj->events->destruct;
    return LinkedList_insert(list, element);
}

bool E_Obj_insertToVector(Vector * vector, E_Obj * obj) {
    if(obj == NULL || vector == NULL) return false;

    Vector_Element * element = malloc(sizeof(Vector_Element));
    element->ptr = obj;
    element->destruct = obj->events->destruct;
    return Vector_append(vector, element);
}


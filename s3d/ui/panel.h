/**
 * <------------------------------------------------------------------>
 * @name    2D Engine
 * @author  Martin Krcma
 * @date    28. 4. 2021
 * <------------------------------------------------------------------>
 * @file    label.h
 * @brief
 * <------------------------------------------------------------------>
 */

#ifndef PANEL_H
#define PANEL_H

#include "../event.h"
#include "../render.h"
#include "../engine_object.h"
#include "../linkedlist.h"
#include <stdbool.h>


typedef struct {
    //[engine object event]
    const E_Obj_Evts * objEvts;

    //[data]

    //UI object events
    UI_Events events;

    //position and size of UI object
    Point2D position;
    GLfloat width;
    GLfloat height;

    //data
    Color background;
    LinkedList childs;
} Panel;

/**
 * @brief Panel_create
 * @param x
 * @param y
 * @param width
 * @param height
 * @return
 */
Panel * Panel_create(int x, int y, size_t width, size_t height);

/**
 * @brief Panel_destruct
 * @param pan
 */
void Panel_destruct(Panel * pan);


/**
 * @brief Panel_insertChild
 * @param pan
 * @param obj
 * @return
 */
bool Panel_insertChild(Panel * pan, E_Obj * obj);

#endif // PANEL_H

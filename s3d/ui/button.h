/**
 * <------------------------------------------------------------------>
 * @name    2D Engine
 * @author  Martin Krcma
 * @date    11. 3. 2021
 * <------------------------------------------------------------------>
 * @file    button.h
 * @brief
 * <------------------------------------------------------------------>
 */

#ifndef BUTTON_H
#define BUTTON_H

#include "../event.h"
#include "../render.h"
#include "../engine_object.h"
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
    Color foreground;
    char * text;
} Button;


/**
 * @brief Button_create
 * @param x
 * @param y
 * @param width
 * @param heigth
 * @param txt
 * @return
 */
Button * Button_create(int x, int y, size_t width, size_t heigth,
                       const char * const txt);

/**
 * @brief Button_destruct
 * @param btn
 */
void Button_destruct(Button * btn);

#endif // BUTTON_H

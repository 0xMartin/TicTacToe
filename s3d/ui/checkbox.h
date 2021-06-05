/**
 * <------------------------------------------------------------------>
 * @name    2D Engine
 * @author  Martin Krcma
 * @date    19. 3. 2021
 * <------------------------------------------------------------------>
 * @file    checkbox.h
 * @brief
 * <------------------------------------------------------------------>
 */

#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "../event.h"
#include "../render.h"
#include "../engine_object.h"
#include "label.h"
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
    bool value;
    Color background;
    Color borderColor;
    Color foreground;
    Label * label;
} CheckBox;


/**
 * @brief CheckBox_create
 * @param x
 * @param y
 * @param size
 * @param value
 * @param txt
 * @return
 */
CheckBox * CheckBox_create(int x, int y, size_t size, bool value, const char * const txt);

/**
 * @brief TextField_destruct
 * @param btn
 */
void CheckBox_destruct(CheckBox * btn);

#endif // BUTTON_H

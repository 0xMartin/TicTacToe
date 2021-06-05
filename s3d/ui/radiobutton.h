/**
 * <------------------------------------------------------------------>
 * @name    2D Engine
 * @author  Martin Krcma
 * @date    19. 3. 2021
 * <------------------------------------------------------------------>
 * @file    radiobutton.h
 * @brief
 * <------------------------------------------------------------------>
 */

#ifndef RADIOBUTTON_H
#define RADIOBUTTON_H

#include "../event.h"
#include "../render.h"
#include "../engine_object.h"
#include "label.h"
#include <stdbool.h>


typedef struct {
    //[engine object event]
    const E_Obj_Evts * objEvts;

    //[data]
    LinkedList radiobuttons;
} RadioButtonGroup;


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
    RadioButtonGroup * group;
} RadioButton;


/**
 * @brief RadioButton_create
 * @param x
 * @param y
 * @param size
 * @param value
 * @param txt
 * @return
 */
RadioButton * RadioButton_create(int x, int y, size_t size, bool value, const char * const txt);

/**
 * @brief TextField_destruct
 * @param btn
 */
void RadioButton_destruct(RadioButton * btn);


/**
 * @brief RadioButtonGroup_create
 * @return
 */
RadioButtonGroup * RadioButtonGroup_create();

/**
 * @brief RadioButtonGroup_add
 * @param group
 * @param btn
 * @return
 */
bool RadioButtonGroup_add(RadioButtonGroup * group, RadioButton * btn);

/**
 * @brief RadioButtonGroup_destruct
 * @param group
 */
void RadioButtonGroup_destruct(RadioButtonGroup * group);

#endif // BUTTON_H

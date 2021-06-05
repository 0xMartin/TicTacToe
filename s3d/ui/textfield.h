/**
 * <------------------------------------------------------------------>
 * @name    2D Engine
 * @author  Martin Krcma
 * @date    11. 3. 2021
 * <------------------------------------------------------------------>
 * @file    textfield.h
 * @brief
 * <------------------------------------------------------------------>
 */

#ifndef TEXTFIELD_H
#define TEXTFIELD_H

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
    Color caret;
    int caret_position;
    int caret_offset;
    int caret_time;
    size_t maxTextLength;
    char * text;
} TextField;


/**
 * @brief TextField_create
 * @param x
 * @param y
 * @param width
 * @param heigth
 * @param max_str_len
 * @return
 */
TextField * TextField_create(int x, int y, size_t width, size_t heigth, size_t max_str_len);

/**
 * @brief TextField_destruct
 * @param btn
 */
void TextField_destruct(TextField * tf);

/**
 * @brief TextField_setText
 * @param tf
 * @param txt
 * @return
 */
bool TextField_setText(TextField * tf, const char * const txt);

#endif // BUTTON_H

/**
 * <------------------------------------------------------------------>
 * @name    2D Engine
 * @author  Martin Krcma
 * @date    27. 4. 2021
 * <------------------------------------------------------------------>
 * @file    checkbox.h
 * @brief
 * <------------------------------------------------------------------>
 */

#ifndef IMAGE_H
#define IMAGE_H

#include "../event.h"
#include "../render.h"
#include "../engine_object.h"
#include <stdbool.h>


/**
  * if you assigned this value to the width or height of the image then this dimension assign to the second dimension
  */
#define IMAGE_SIZE_AUTO 0


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
    Texture * texture;
} Image;


/**
 * @brief Image_create
 * @param x
 * @param y
 * @param width
 * @param heigth
 * @param texture
 * @return
 */
Image * Image_create(int x, int y, size_t width, size_t heigth, Texture * texture);

/**
 * @brief Image_destruct
 * @param btn
 */
void Image_destruct(Image * btn);

#endif // BUTTON_H

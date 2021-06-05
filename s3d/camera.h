/**
 * <------------------------------------------------------------------>
 * @name    2D Engine
 * @author  Martin Krcma
 * @date    19. 3. 2021
 * <------------------------------------------------------------------>
 * @file    camera.h
 * @brief   Engine camera
 * <------------------------------------------------------------------>
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "render.h"

#include <stdbool.h>

#define CAMERA_DIR_Y_RANGE 1.5

#define CAMERA_INIT {\
    0.0f, 0.0f,\
    {0.0f, 0.0f, 1.0f},\
    Z_VERTEX,\
    0.0f, 0.0f,\
    Z_VERTEX,\
    {-CAMERA_DIR_Y_RANGE, CAMERA_DIR_Y_RANGE},\
    0.0f\
    }


typedef struct {
    float min;
    float max;
} FRange;

/**
  * Camera
  */
typedef struct {
    float angleXZ, angleY;  /** angle of rotation for the camera direction*/
    Vertex dir; /** actual vector representing the camera's direction*/
    Vertex pos; /** XYZ position of the camera*/
    float deltaAngleXZ, deltaAngleY;    /** control angle*/
    Vertex deltaPos;    /** control position*/
    FRange limitAngleY; /** camera angle limit*/
    float cosAY;    /** cos(angleY) for camera direcion*/
} Camera;


/**
 * @brief CAMERA_computePosition    Compute new X, Y, Z position of
 * camera for straight move -> Control variable: cam.deltaPos
 * @param cam   Camera
 * @return TRUE (no error) / FALSE (something go wrong)
 */
bool Camera_computePosition(Camera * cam);

/**
 * @brief CAMERA_computeDirection   Update new value of vector representing
 * the camera's direction -> Control variable: cam.deltaAngle
 * @param cam   Camera
 * @return TRUE (no error) / FALSE (something go wrong)
 */
bool Camera_computeDirection(Camera * cam);

/**
 * @brief CAMERA_viewTransformation Apply camera view transformation
 * @param cam   Camera
 * @return TRUE (no error) / FALSE (something go wrong)
 */
bool Camera_viewTransformation(Camera * cam);

#endif // CAMERA_H

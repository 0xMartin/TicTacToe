/**
 * <------------------------------------------------------------------>
 * @name    2D Engine
 * @author  Martin Krcma
 * @date    19. 3. 2021
 * <------------------------------------------------------------------>
 * @file    camera.c
 * @brief   Implementation of camera.h
 * <------------------------------------------------------------------>
 */

#include "camera.h"


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "util.h"
#include <math.h>


bool Camera_computePosition(Camera * cam) {
    if(cam == NULL) return false;

    // moving in z axis
    if (cam->deltaPos.z) {
        cam->pos.x += cam->deltaPos.z * cam->dir.x;
        cam->pos.z += cam->deltaPos.z * cam->dir.z;
    }

    //moving in y axis
    if (cam->deltaPos.y) {
        cam->pos.y += cam->deltaPos.y;
    }

    //moving in x axis
    if (cam->deltaPos.x) {
        cam->pos.x += cam->deltaPos.x * cam->dir.z;
        cam->pos.z -= cam->deltaPos.x * cam->dir.x;
    }

    return true;
}

bool Camera_computeDirection(Camera * cam) {
    if(cam == NULL) return false;

    if (cam->deltaAngleXZ && cam->deltaAngleY) {
        //compute new value of angle XZ
        cam->angleXZ += cam->deltaAngleXZ;
        //compute new angle of Y
        cam->angleY += cam->deltaAngleY;

        //limit
        cam->angleY = MAX(cam->angleY, cam->limitAngleY.min);
        cam->angleY = MIN(cam->angleY, cam->limitAngleY.max);

        //compute x, y and z of directin vector
        cam->dir.x = cos(cam->angleXZ);
        cam->dir.z = sin(cam->angleXZ);
        //compute y of directin vector
        cam->dir.y = -sin(cam->angleY);
        cam->cosAY = cos(cam->angleY);
    }

    return true;
}

bool Camera_viewTransformation(Camera * cam) {
    if(cam == NULL) return false;

    gluLookAt(
                cam->pos.x,
                cam->pos.y,
                cam->pos.z,
                cam->pos.x + cam->dir.x * cam->cosAY,
                cam->pos.y + cam->dir.y,
                cam->pos.z + cam->dir.z * cam->cosAY,
                0.0f,
                1.0f,
                0.0f
                );

    return true;
}

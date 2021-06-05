/**
 * <------------------------------------------------------------------>
 * @name    2D Engine
 * @author  Martin Krcma
 * @date    6. 3. 2021
 * <------------------------------------------------------------------>
 * @file    util.h
 * @brief
 * <------------------------------------------------------------------>
 */


#ifndef UTIL_H
#define UTIL_H

#if __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <stdbool.h>
#include <stdio.h>
#include <time.h>


#define MAX(a, b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })
#define MIN(a, b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })
#define IN_RANGE(v, min, max) (v >= min && v <= max)


//texture
typedef struct {
    GLuint width;
    GLuint height;
    GLuint imageSize;
    GLuint textureID;
} Texture;


/**
 * @brief Load bmp image and create opengl texture
 * @param path Path to image
 * @return Pointer on loaded texture
 */
Texture * UTIL_loadTextureBMP(const char * path, bool rgba_mode);

/**
 * @brief Load png image and create opengl texture
 * @param path Path to image
 * @return Pointer on loaded texture
 */
Texture * UTIL_loadTexturePNG(const char * path);

/**
 * @brief Generate random number in range from 0 to n
 * @param n The highest possible number
 * @return Random int
 */
int UTIL_randInt(int n);

/**
 * @brief Get current system time
 * @return Nanoseconds
 */
__syscall_slong_t UTIL_getSystemNanoTime();

/**
 * @brief Get current system time
 * @return Seconds
 */
__time_t UTIL_getSystemSeconds();

/**
 * @brief Get current system time
 * @return timespec
 */
struct timespec UTIL_getSystemTime();

/**
 * @brief Destruct simple data structure
 * @param data  Pointer on data structure
 */
void UTIL_simpleDestructor(void * data);

#endif // UTIL_H

/**
 * <------------------------------------------------------------------>
 * @name    2D Engine
 * @author  Martin Krcma
 * @date    8. 3. 2021
 * <------------------------------------------------------------------>
 * @file    render.h
 * @brief
 * <------------------------------------------------------------------>
 */

#ifndef RENDER_H
#define RENDER_H


#if __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#if _WIN32
#include <Windows.h>
#endif

#include "util.h"
#include "linkedlist.h"
#include "event.h"


//colors
#define COLOR_BLACK     (Color){0.0,   0.0,    0.0,    1.0}
#define COLOR_BLUE      (Color){0.0,   0.0,    1.0,    1.0}
#define COLOR_CYAN      (Color){0.0,   1.0,    1.0,    1.0}
#define COLOR_DARKGRAY	(Color){0.25,  0.25,   0.25,   1.0}
#define COLOR_GRAY      (Color){0.5,   0.5,    0.5,    1.0}
#define COLOR_GREEN     (Color){0.0,   1.0,    0.0,    1.0}
#define COLOR_LIGHTGRAY	(Color){0.75,  0.75,   0.75,   1.0}
#define COLOR_MAGENTA	(Color){1.0,   0.0,    1.0,    1.0}
#define COLOR_ORANGE	(Color){1.0,   0.78,   0.0,    1.0}
#define COLOR_PINK      (Color){1.0,   0.69,   0.69,   1.0}
#define COLOR_RED       (Color){1.0,   0.0,    0.0,    1.0}
#define COLOR_WHITE     (Color){1.0,   1.0,    1.0,    1.0}
#define COLOR_YELLOW	(Color){1.0,   1.0,    0.0,    1.0}


#define COLOR_DARKER(c, factor)	((Color){.red=c.red*(1.0-factor),\
    .green=c.green*(1.0-factor), .blue=c.blue*(1.0-factor), .alpha=c.alpha})

#define COLOR_LIGHTER(c, factor) ((Color){.red=MIN(1.0, c.red * 1.0 + factor),\
    .green=MIN(1.0, c.green * 1.0 + factor), .blue=MIN(1.0, c.blue * 1.0 + factor), .alpha=c.alpha})

#define COLOR_CHANGE_OPACITY(c, opacity) ((Color){.red=c.red, .green=c.green, .blue=c.blue, .alpha=opacity})


//fonts
#define E2D_STROKE_ROMAN            GLUT_STROKE_ROMAN
#define E2D_STROKE_MONO_ROMAN       GLUT_STROKE_MONO_ROMAN
#define E2D_BITMAP_9_BY_15          GLUT_BITMAP_9_BY_15
#define E2D_BITMAP_8_BY_13          GLUT_BITMAP_8_BY_13
#define E2D_BITMAP_TIMES_ROMAN_10   GLUT_BITMAP_TIMES_ROMAN_10
#define E2D_BITMAP_TIMES_ROMAN_24   GLUT_BITMAP_TIMES_ROMAN_24
#define E2D_BITMAP_HELVETICA_10     GLUT_BITMAP_HELVETICA_10
#define E2D_BITMAP_HELVETICA_12     GLUT_BITMAP_HELVETICA_12
#define E2D_BITMAP_HELVETICA_18     GLUT_BITMAP_HELVETICA_18

#define RENDER_DEFAULT_LINE_WIDTH 1


//RGB color
typedef struct {
    GLfloat red; /** red color*/
    GLfloat green; /** green color*/
    GLfloat blue; /** blue color*/
    GLfloat alpha; /** alpha*/
} Color;

//2D point
typedef struct {
    GLfloat x; /** X position of point*/
    GLfloat y; /** Y position of point*/
    Color color; /** color of point*/
} Point2D;

//bounds
typedef struct {
    Point2D position; /** Position */
    size_t width; /** Width */
    size_t height; /** Height */
} Bounds;

//vertex
typedef struct {
    GLfloat x; /** X position*/
    GLfloat y; /** Y position*/
    GLfloat z; /** Z position*/
} Vertex;

//face 2D
typedef struct {
    Point2D * points;
    size_t count;
    GLint textureID;
} Face2D;

//face 3D
typedef struct {
    Vertex * vetices;
    size_t count;
    GLint textureID;
} Face3D;

/**
 * @brief Render_scale
 * @param x
 * @param y
 */
void Render_scale(GLfloat x, GLfloat y);

/**
 * @brief Render_lineWidth
 * @param width
 */
void Render_lineWidth(GLfloat width);

/**
 * @brief Render_setMaxAlpha
 * @param alpha
 */
void Render_setMaxAlpha(GLfloat alpha);

/**
 * @brief Render_getStringWidth
 * @param alpha
 * @return
 */
int Render_getStringWidth(const char * str);

/**
 * @brief Render_getStringWidthIndex
 * @param str
 * @param lastCharIndex
 * @return
 */
int Render_getStringWidthIndex(const char * str, int lastCharIndex);

/**
 * @brief Render_getStringWidthRange
 * @param str
 * @param start
 * @param end
 * @return
 */
int Render_getStringWidthRange(const char * str, int start, int end);

/**
 * @brief Render_getStringHeight
 * @return
 */
int Render_getStringHeight();

/**
 * @brief Render_setColorRGB
 * @param red
 * @param green
 * @param blue
 */
void Render_setColorRGB(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

/**
 * @brief Render_setColor
 * @param color
 */
void Render_setColor(Color * color);

/**
 * @brief Render_setAlpha
 * @param alpha
 */
void Render_setAlpha(GLfloat alpha);

/**
 * @brief Render_setColor
 * @param color
 */
void Render_enablePtColor();

/**
 * @brief Render_setColor
 * @param color
 */
void Render_disablePtColor();

/**
 * @brief Render_applyOffset
 * @param x
 * @param y
 * @param z
 */
void Render_applyOffset(GLfloat x, GLfloat y, GLfloat z);

/**
 * @brief Render_clearOffset
 */
void Render_clearOffset();

/**
 * @brief Render_setScissor
 * @param x
 * @param y
 * @param width
 * @param heigh
 * @param evt
 */
void Render_setScissor(GLfloat x, GLfloat y, GLfloat width,
                       GLfloat height, const Event_Render * evt);

/**
 * @brief Render_resetScissor
 * @param evt
 */
void Render_resetScissor(const Event_Render * evt);

/**
 * @brief Render_clear
 * @param evt
 * @param color
 */
void Render_clear(Event_Render * evt, Color * color);

/**
 * @brief Render_drawLine
 * @param p1
 * @param p2
 */
void Render_drawLine(Point2D * p1, Point2D * p2);

/**
 * @brief Render_drawLines
 * @param p
 * @param count
 */
void Render_drawLines(Point2D * p, size_t count);

/**
 * @brief Render_drawTriangle
 * @param p1
 * @param p2
 * @param p3
 */
void Render_drawTriangle(Point2D * p1, Point2D * p2, Point2D * p3);

/**
 * @brief Render_drawQuad
 * @param p1
 * @param p2
 * @param p3
 * @param p4
 */
void Render_drawQuad(Point2D * p1, Point2D * p2, Point2D * p3, Point2D * p4);

/**
 * @brief Render_drawPolygon
 * @param p
 * @param count
 */
void Render_drawPolygon(Point2D * p, size_t count);

/**
 * @brief Render_drawRectangle
 * @param p
 * @param width
 * @param height
 */
void Render_drawRectangle(Point2D * p, size_t width, size_t height);

/**
 * @brief Render_fillTriangle
 * @param p1
 * @param p2
 * @param p3
 */
void Render_fillTriangle(Point2D * p1, Point2D * p2, Point2D * p3);

/**
 * @brief Render_fillQuad
 * @param p1
 * @param p2
 * @param p3
 * @param p4
 */
void Render_fillQuad(Point2D * p1, Point2D * p2, Point2D * p3, Point2D * p4);

/**
 * @brief Render_fillPolygon
 * @param p
 * @param count
 */
void Render_fillPolygon(Point2D * p, size_t count);

/**
 * @brief Render_drawEllipse
 * @param p
 * @param rx
 * @param ry
 */
void Render_drawEllipse(Point2D * p, GLfloat rx, GLfloat ry);

/**
 * @brief Render_fillEllipse
 * @param p
 * @param rx
 * @param ry
 */
void Render_fillEllipse(Point2D * p, GLfloat rx, GLfloat ry);

/**
 * @brief Render_fillRectangle
 * @param p
 * @param width
 * @param height
 */
void Render_fillRectangle(Point2D * p, size_t width, size_t height);

/**
 * @brief Render_drawImage
 * @param p
 * @param texture
 * @param defaultShape
 */
void Render_drawImage(Point2D * p, Texture * texture, bool defaultShape);

/**
 * @brief Render_setFont
 * @param font
 * @param size
 */
void Render_setFont(void * font, GLint size);

/**
 * @brief Render_drawString
 * @param p
 * @param str
 */
void Render_drawString(GLfloat x, GLfloat y,  const char * const str);

/**
 * @brief Render_mesh
 * @param p
 */
void Render_mesh(Point2D * p, LinkedList * faces);


#endif // RENDER_H

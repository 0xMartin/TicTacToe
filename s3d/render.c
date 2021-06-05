/**
 * <------------------------------------------------------------------>
 * @name    2D Engine
 * @author  Martin Krcma
 * @date    8. 3. 2021
 * <------------------------------------------------------------------>
 * @file    util.c
 * @brief   Implementation of render.h
 * <------------------------------------------------------------------>
 */

#include "render.h"

#include <math.h>
#include <string.h>


static Color COLOR = COLOR_WHITE;

static GLfloat MAX_ALPHA = 1.0;

static GLfloat SCALE_X = 1.0;
static GLfloat SCALE_Y = 1.0;

static GLfloat OFFSET_X = 0.0;
static GLfloat OFFSET_Y = 0.0;
static GLfloat OFFSET_Z = 0.0;

static void * FONT = E2D_BITMAP_HELVETICA_18;

static bool ENABLE_PT_COLOR = false;


static void applyPtColor(Point2D * p) {
    if(p != NULL) {
        if(ENABLE_PT_COLOR) {
            glColor4f(p->color.red, p->color.green, p->color.blue, MIN(p->color.alpha, MAX_ALPHA));
        }
    }
}

void Render_scale(GLfloat x, GLfloat y) {
    if(x > 0.0 && y > 0.0) {
        if(SCALE_X != 1.0 || SCALE_Y != 1.0) {
            glScalef(1.0 / SCALE_X, 1.0 / SCALE_Y, 1.0);
        }
        SCALE_X = x;
        SCALE_Y = y;
        glScalef(x, y, 1.0);
    }
}

void Render_lineWidth(GLfloat width) {
    if(width > 0.0) {
        glLineWidth(width);
    }
}

void Render_setMaxAlpha(GLfloat alpha) {
    if(IN_RANGE(alpha, 0.0, 1.0)) {
        MAX_ALPHA = alpha;
    }
}

int Render_getStringWidth(const char * str) {
    if(str == NULL) return 0;
    if(FONT == NULL) return 0;

    return glutBitmapLength(FONT, (const unsigned char *) str);
}

int Render_getStringWidthIndex(const char * str, int lastCharIndex) {
    if(str == NULL) return 0;
    if(FONT == NULL) return 0;
    if(lastCharIndex < 0) return 0;

    int length = 0;
    int len = strlen(str);
    for(int index = 0; index < lastCharIndex && index < len && index >= 0; ++index) {
        length += glutBitmapWidth(FONT, *(str + index));
    }

    return length;
}

int Render_getStringWidthRange(const char * str, int start, int end) {
    if(str == NULL) return 0;
    if(FONT == NULL) return 0;
    if(start > end) return 0;

    int length = 0;
    int len = strlen(str);
    for(int index = start; index < end && index < len && index >= 0; ++index) {
        length += glutBitmapWidth(FONT, *(str + index));
    }

    return length;
}

int Render_getStringHeight() {
    if(FONT == NULL) return 0;

    if(FONT == E2D_STROKE_ROMAN) {
        return 10;
    } else if(FONT == E2D_STROKE_MONO_ROMAN) {
        return 10;
    } else if(FONT == E2D_BITMAP_9_BY_15) {
        return 15;
    } else if(FONT == E2D_BITMAP_8_BY_13) {
        return 13;
    } else if(FONT == E2D_BITMAP_TIMES_ROMAN_10) {
        return 10;
    } else if(FONT == E2D_BITMAP_TIMES_ROMAN_24) {
        return 24;
    } else if(FONT == E2D_BITMAP_HELVETICA_10) {
        return 10;
    } else if(FONT == E2D_BITMAP_HELVETICA_12) {
        return 12;
    } else if(FONT == E2D_BITMAP_HELVETICA_18) {
        return 18;
    }

    return 0;
}

void Render_setColorRGB(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
    if(IN_RANGE(red, 0.0, 1.0) && IN_RANGE(green, 0.0, 1.0)
            && IN_RANGE(blue, 0.0, 1.0) && IN_RANGE(alpha, 0.0, 1.0)) {
        COLOR.red = red;
        COLOR.green = green;
        COLOR.blue = blue;
        COLOR.alpha = alpha;
    }
}

void Render_setColor(Color * color) {
    if(color != NULL) {
        if(IN_RANGE(color->red, 0.0, 1.0) &&
                IN_RANGE(color->green, 0.0, 1.0) &&
                IN_RANGE(color->blue, 0.0, 1.0) &&
                IN_RANGE(color->alpha, 0.0, 1.0)) {
            COLOR = *color;
        }
    }
}

void Render_setAlpha(GLfloat alpha) {
    if(IN_RANGE(alpha, 0.0, 1.0)) {
        COLOR.alpha = alpha;
    }
}

void Render_enablePtColor() {
    ENABLE_PT_COLOR = true;
}

void Render_disablePtColor() {
    ENABLE_PT_COLOR = false;
}

void Render_applyOffset(GLfloat x, GLfloat y, GLfloat z) {
    if(x != 0.0 || y != 0.0 || z != 0.0) {
        OFFSET_X += x;
        OFFSET_Y += y;
        OFFSET_Z += z;
        glTranslatef(x, y, z);
    }
}

void Render_clearOffset() {
    if(OFFSET_X != 0.0 || OFFSET_Y != 0.0 || OFFSET_Z != 0.0) {
        glTranslatef(-OFFSET_X, -OFFSET_Y, -OFFSET_Z);
        OFFSET_X = 0.0;
        OFFSET_Y = 0.0;
        OFFSET_Z = 0.0;
    }
}

void Render_setScissor(GLfloat x, GLfloat y, GLfloat width,
                       GLfloat height, const Event_Render * evt) {
    return;  //remove
    if(width > 0 && height > 0 && evt->window_width > 0 && evt->window_height > 0) {
        int x_s = OFFSET_X + x;
        int y_s = evt->window_height - y - OFFSET_Y;
        glScissor(x_s, y_s,
                  MIN(width, evt->window_width - x),
                  MIN(height, evt->window_height - y));
    }
}

void Render_resetScissor(const Event_Render * evt) {
    if(evt->window_width > 0 && evt->window_height > 0) {
        glScissor(OFFSET_X, OFFSET_Y,
                  evt->window_width, evt->window_height);
    }
}

void Render_clear(Event_Render * evt, Color * color) {
    glClear(GL_COLOR_BUFFER_BIT);

    if(color != NULL) {
        glClearColor(color->red, color->green, color->blue, 1.0);
    }

    if(evt != NULL) {
        Render_resetScissor(evt);
    }

    MAX_ALPHA = 1.0;
    COLOR.red = COLOR.green = COLOR.blue = 1.0;
    FONT = E2D_BITMAP_HELVETICA_18;
    glLineWidth(1.0);

    Render_clearOffset();

    if(SCALE_X != 1.0 || SCALE_Y != 1.0) {
        glScalef(1.0 / SCALE_X, 1.0 / SCALE_Y, 1.0);
        SCALE_X = 1.0;
        SCALE_Y = 1.0;
    }
}

void Render_drawLine(Point2D * p1, Point2D * p2) {
    if(p1 != NULL && p2 != NULL) {
        if(!ENABLE_PT_COLOR) {
            glColor4f(COLOR.red, COLOR.green, COLOR.blue, MIN(COLOR.alpha, MAX_ALPHA));
        }
        glBegin(GL_LINES);
        applyPtColor(p1);
        glVertex2f(p1->x, p1->y);
        applyPtColor(p2);
        glVertex2f(p2->x, p2->y);
        glEnd();
    }
}

void Render_drawLines(Point2D * p, size_t count) {
    if(p != NULL && count > 0) {
        Point2D * p1, *p2;
        p1 = p;
        p2 = NULL;
        if(!ENABLE_PT_COLOR) {
            glColor4f(COLOR.red, COLOR.green, COLOR.blue, MIN(COLOR.alpha, MAX_ALPHA));
        }
        for(size_t i = 1; i < count; ++i) {
            p2 = p1 + 1;
            glBegin(GL_LINES);
            applyPtColor(p1);
            glVertex2f(p1->x, p1->y);
            applyPtColor(p2);
            glVertex2f(p2->x, p2->y);
            glEnd();
            p1 = p2;
        }
    }
}

void Render_drawTriangle(Point2D * p1, Point2D * p2, Point2D * p3) {
    if(p1 != NULL && p2 != NULL && p3 != NULL) {
        if(!ENABLE_PT_COLOR) {
            glColor4f(COLOR.red, COLOR.green, COLOR.blue, MIN(COLOR.alpha, MAX_ALPHA));
        }
        glBegin(GL_LINE_LOOP);
        applyPtColor(p1);
        glVertex2f(p1->x, p1->y);
        applyPtColor(p2);
        glVertex2f(p2->x, p2->y);
        applyPtColor(p3);
        glVertex2f(p3->x, p3->y);
        glEnd();
    }
}

void Render_drawQuad(Point2D * p1, Point2D * p2, Point2D * p3, Point2D * p4) {
    if(p1 != NULL && p2 != NULL && p3 != NULL) {
        if(!ENABLE_PT_COLOR) {
            glColor4f(COLOR.red, COLOR.green, COLOR.blue, MIN(COLOR.alpha, MAX_ALPHA));
        }
        glBegin(GL_LINE_LOOP);
        applyPtColor(p1);
        glVertex2f(p1->x, p1->y);
        applyPtColor(p2);
        glVertex2f(p2->x, p2->y);
        applyPtColor(p3);
        glVertex2f(p3->x, p3->y);
        applyPtColor(p4);
        glVertex2f(p4->x, p4->y);
        glEnd();
    }
}

void Render_drawPolygon(Point2D * p, size_t count) {
    if(p != NULL && count > 3) {
        if(!ENABLE_PT_COLOR) {
            glColor4f(COLOR.red, COLOR.green, COLOR.blue, MIN(COLOR.alpha, MAX_ALPHA));
        }
        glBegin(GL_LINE_LOOP);
        for(size_t i = 0; i <= count; ++i) {
            applyPtColor(p + i);
            glVertex2f((p + i)->x, (p + i)->y);
        }
        glEnd();
    }
}

void Render_drawRectangle(Point2D * p, size_t width, size_t height) {
    if(p != NULL && width > 0 && height > 0) {
        glColor4f(COLOR.red, COLOR.green, COLOR.blue, MIN(COLOR.alpha, MAX_ALPHA));
        glBegin(GL_LINE_LOOP);
        glVertex2f(p->x, p->y);
        glVertex2f(p->x + width, p->y);
        glVertex2f(p->x + width, p->y + height);
        glVertex2f(p->x, p->y + height);
        glEnd();
    }
}

void Render_fillTriangle(Point2D * p1, Point2D * p2, Point2D * p3) {
    if(p1 != NULL && p2 != NULL && p3 != NULL) {
        if(!ENABLE_PT_COLOR) {
            glColor4f(COLOR.red, COLOR.green, COLOR.blue, MIN(COLOR.alpha, MAX_ALPHA));
        }
        glBegin(GL_TRIANGLES);
        applyPtColor(p1);
        glVertex2f(p1->x, p1->y);
        applyPtColor(p2);
        glVertex2f(p2->x, p2->y);
        applyPtColor(p3);
        glVertex2f(p3->x, p3->y);
        glEnd();
    }
}

void Render_fillQuad(Point2D * p1, Point2D * p2, Point2D * p3, Point2D * p4) {
    if(p1 != NULL && p2 != NULL && p3 != NULL) {
        if(!ENABLE_PT_COLOR) {
            glColor4f(COLOR.red, COLOR.green, COLOR.blue, MIN(COLOR.alpha, MAX_ALPHA));
        }
        glBegin(GL_QUADS);
        applyPtColor(p1);
        glVertex2f(p1->x, p1->y);
        applyPtColor(p2);
        glVertex2f(p2->x, p2->y);
        applyPtColor(p3);
        glVertex2f(p3->x, p3->y);
        applyPtColor(p4);
        glVertex2f(p4->x, p4->y);
        glEnd();
    }
}

void Render_fillPolygon(Point2D * p, size_t count) {
    if(p != NULL && count > 3) {
        if(!ENABLE_PT_COLOR) {
            glColor4f(COLOR.red, COLOR.green, COLOR.blue, MIN(COLOR.alpha, MAX_ALPHA));
        }
        glBegin(GL_POLYGON);
        for(size_t i = 0; i < count; ++i) {
            applyPtColor(p + i);
            glVertex2f((p + i)->x, (p + i)->y);
        }
        glEnd();
    }
}

void Render_drawEllipse(Point2D * p, GLfloat rx, GLfloat ry) {
    if(p != NULL) {
        int num_segments = 12 + MAX(rx, ry) / 5;
        float theta = 2 * 3.1415926 / num_segments;
        float c = cosf(theta);
        float s = sinf(theta);
        float t;
        float x = 1;
        float y = 0;
        glColor4f(COLOR.red, COLOR.green, COLOR.blue, MIN(COLOR.alpha, MAX_ALPHA));
        glBegin(GL_LINE_LOOP);
        for(int i = 0; i < num_segments; i++)
        {
            glVertex2f(x * rx + p->x, y * ry + p->y);
            t = x;
            x = c * x - s * y;
            y = s * t + c * y;
        }
        glEnd();
    }
}

void Render_fillEllipse(Point2D * p, GLfloat rx, GLfloat ry) {
    if(p != NULL) {
        int num_segments = 12 + MAX(rx, ry) / 5;
        float theta = 2 * 3.1415926 / num_segments;
        float c = cosf(theta);
        float s = sinf(theta);
        float t;
        float x = 1;
        float y = 0;
        glColor4f(COLOR.red, COLOR.green, COLOR.blue, MIN(COLOR.alpha, MAX_ALPHA));
        glBegin(GL_POLYGON);
        for(int i = 0; i < num_segments; i++)
        {
            glVertex2f(x * rx + p->x, y * ry + p->y);
            t = x;
            x = c * x - s * y;
            y = s * t + c * y;
        }
        glEnd();
    }
}

void Render_fillRectangle(Point2D * p, size_t width, size_t height) {
    if(p != NULL && width > 0 && height > 0) {
        glColor4f(COLOR.red, COLOR.green, COLOR.blue, MIN(COLOR.alpha, MAX_ALPHA));
        glBegin(GL_QUADS);
        glVertex2f(p->x, p->y);
        glVertex2f(p->x + width, p->y);
        glVertex2f(p->x + width, p->y + height);
        glVertex2f(p->x, p->y + height);
        glEnd();
    }
}

void Render_drawImage(Point2D * p, Texture * texture, bool defaultShape) {
    if(p != NULL) {
        glColor4f(1.0, 1.0, 1.0, COLOR.alpha);
        glBindTexture(GL_TEXTURE_2D, texture->textureID);
        glEnable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);

        if(defaultShape) {
            glTexCoord2i(0, 0);
            glVertex2f(p->x, p->y + texture->height);

            glTexCoord2i(0, 1);
            glVertex2f(p->x, p->y);

            glTexCoord2i(1, 1);
            glVertex2f(p->x + texture->width, p->y);

            glTexCoord2i(1, 0);
            glVertex2f(p->x + texture->width, p->y + texture->height);
        } else {
            for(unsigned int i = 0; i < 4; ++i) {
                glTexCoord2i(i > 1, i > 0 && i < 3);
                glVertex2f((p + i)->x, (p + i)->y);
            }
        }

        glEnd();
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Render_setFont(void * font, GLint size) {
    if(font != NULL && size > 5) {
        if(font == E2D_STROKE_ROMAN ||
                font == E2D_STROKE_MONO_ROMAN ||
                font == E2D_BITMAP_9_BY_15 ||
                font == E2D_BITMAP_8_BY_13 ||
                font == E2D_BITMAP_TIMES_ROMAN_10 ||
                font == E2D_BITMAP_TIMES_ROMAN_24 ||
                font == E2D_BITMAP_HELVETICA_10 ||
                font == E2D_BITMAP_HELVETICA_12 ||
                font == E2D_BITMAP_HELVETICA_18) {
            FONT = font;
        }
    }
}

void Render_drawString(GLfloat x, GLfloat y, const char * const str) {
    if(str != NULL) {
        if(strlen(str) > 0) {
            glColor4f(COLOR.red, COLOR.green, COLOR.blue, MIN(COLOR.alpha, MAX_ALPHA));
            glRasterPos2f(x, y);
            const char * c  = str;
            while(*c){
                glutBitmapCharacter(FONT, *c);
                c = c + 1;
            }
        }
    }
}

void Render_mesh(Point2D * p, LinkedList * faces) {
    if(p == NULL || faces == NULL) return;
}

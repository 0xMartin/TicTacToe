#ifndef CELL_H
#define CELL_H


#include "../s3d/engine_object.h"
#include "../s3d/linkedlist.h"
#include "../s3d/render.h"

typedef enum {
    Symbol_X, Symbol_O, Symbol_None
} Symbol;

#define CELL_BG_COLOR (Color){0.259, 0.267, 0.314, 0.950}
#define CELL_BG_PLACE_COLOR (Color){0.4, 0.4, 0.3, 0.950}

typedef struct {
    //[engine object event]
    const E_Obj_Evts * events;
    //[data]
    Point2D position;
    GLfloat size;
    Symbol symbol;
    Color background;
    bool mouseOn;
} Cell;

/**
 * @brief Field_init
 * @param field
 * @param x
 * @param y
 * @param size
 */
void Cell_init(Cell * cell, int x, int y, size_t size);

#endif // CELL_H

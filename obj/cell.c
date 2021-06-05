#include "cell.h"

#include "../s3d/util.h"

#define X_COLOR (Color){0.900, 0.300, 0.300, 1.000}
#define O_COLOR (Color){0.100, 0.500, 1.000, 1.000}


static void render(void * obj, const Event_Render * evt) {
    Cell * cell = (Cell*) obj;

    if(cell->mouseOn && cell->symbol == Symbol_None) {
        Render_setColor(&COLOR_LIGHTER(cell->background, 0.1));
    } else {
        Render_setColor(&cell->background);
    }
    Render_fillRectangle(&cell->position, cell->size, cell->size);
    Color border = COLOR_LIGHTER(cell->background, 0.4);
    Render_setColor(&border);
    Render_drawRectangle(&cell->position, cell->size, cell->size);

    switch (cell->symbol) {
    case Symbol_X:
        Render_setColor(&X_COLOR);

        Point2D pts[4];
        pts[0] = pts[1] = cell->position;

        pts[0].x += cell->size * 0.2;
        pts[0].y += cell->size * 0.2;
        pts[1].x += cell->size * 0.8;
        pts[1].y += cell->size * 0.8;

        pts[2].x = pts[1].x;
        pts[2].y = pts[0].y;
        pts[3].x = pts[0].x;
        pts[3].y = pts[1].y;

        Render_lineWidth(3);
        Render_drawLine(&pts[0], &pts[1]);
        Render_drawLine(&pts[2], &pts[3]);
        Render_lineWidth(RENDER_DEFAULT_LINE_WIDTH);
        break;
    case Symbol_O:
        Render_setColor(&O_COLOR);

        Point2D center = cell->position;
        center.x += cell->size/2;
        center.y += cell->size/2;

        Render_lineWidth(3);
        Render_drawEllipse(&center, cell->size * 0.3, cell->size * 0.3);
        Render_lineWidth(RENDER_DEFAULT_LINE_WIDTH);
        break;
    }
}

static void mouseMoveEvt(void * obj, SceneData * scene, const Event_Mouse * evt) {
    Cell * cell = (Cell*) obj;

    if(evt->x >= 0 && evt->y >= 0) {
        if(IN_RANGE(evt->x, cell->position.x, cell->position.x + cell->size)) {
            if(IN_RANGE(evt->y, cell->position.y, cell->position.y + cell->size)) {
                cell->mouseOn = true;
                return;
            }
        }
    }
    cell->mouseOn = false;

}

static const E_Obj_Evts e_obj_evts = {
    .destruct = NULL,
    .render = render,
    .resize = NULL,
    .update = NULL,
    .mouseMoveEvt = mouseMoveEvt,
    .mouseButtonEvt = NULL,
    .pressKeyEvt = NULL,
    .releaseKeyEvt = NULL
};

void Cell_init(Cell * cell, int x, int y, size_t size) {
    if(cell != NULL) {
        cell->events = &e_obj_evts;
        cell->position.x = x;
        cell->position.y = y;
        cell->size = size;
        cell->background = CELL_BG_COLOR;
        cell->symbol = Symbol_None;
        cell->mouseOn = false;
    }
}

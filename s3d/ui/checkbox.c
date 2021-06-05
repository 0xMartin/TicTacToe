/**
 * <------------------------------------------------------------------>
 * @name    2D Engine
 * @author  Martin Krcma
 * @date    19. 3. 2021
 * <------------------------------------------------------------------>
 * @file    checkbox.c
 * @brief   Implementation of checkbox.h
 * <------------------------------------------------------------------>
 */

#include "checkbox.h"

#include "../util.h"
#include "ui_obj.h"
#include "colors.h"
#include <stdlib.h>
#include <stdio.h>


/* Object event functions -------------------------------------------------------- */

static void destruct(void * obj) {
    CheckBox * cb = (CheckBox*) obj;
    CheckBox_destruct(cb);
}

static void render(void * obj, const Event_Render * evt) {
    CheckBox * cb = (CheckBox*) obj;
    if(!cb->events.visible) return;

    Render_setColor(&cb->background);
    Render_fillRectangle(&cb->position, cb->width, cb->height);
    Render_setColor(&cb->borderColor);
    Render_drawRectangle(&cb->position, cb->width, cb->height);

    if(cb->value) {
        Render_setColor(&cb->foreground);
        Point2D pts[3];
        pts[0].x = cb->position.x + (int)cb->width / 4;
        pts[0].y = cb->position.y + (int)cb->height / 4;
        pts[1].x = cb->position.x + (int)cb->width / 2;
        pts[1].y = cb->position.y + (int)cb->height * 0.7;
        pts[2].x = cb->position.x + (int)cb->width * 1.2;
        pts[2].y = cb->position.y - (int)cb->height / 6;
        Render_lineWidth(2.5);
        Render_drawLines(pts, 3);
        Render_lineWidth(RENDER_DEFAULT_LINE_WIDTH);
    }

    if(cb->label != NULL) {
        if(cb->label->objEvts->render) {
            int center = cb->height/2 + Render_getStringHeight()/3;
            //shift origin
            glTranslatef(cb->position.x, cb->position.y + center, 0);

            //render
            cb->label->objEvts->render(cb->label, evt);

            //shift origin back
            glTranslatef(-cb->position.x, -cb->position.y - center, 0);
        }
    }
}

static void resize(void * obj, const Event_Resize * evt) {
    UI_Obj * uiobj = (UI_Obj*) obj;
    if(uiobj->events.resizable) {
        uiobj->position.x *= evt->resize_ratio_horizontal;
        uiobj->position.y *= evt->resize_ratio_vertical;
    }
}

static void mouseButtonEvt(void * obj, SceneData * scene, const Event_Mouse * evt) {
    CheckBox * cb = (CheckBox*) obj;
    if(!cb->events.enabled || !cb->events.visible) return;

    cb->events.focus = false;
    if(evt->x >= 0 && evt->y >= 0) {
        if(IN_RANGE(evt->x, cb->position.x, cb->position.x + cb->width)) {
            if(IN_RANGE(evt->y, cb->position.y, cb->position.y + cb->height)) {
                cb->events.focus = true;

                if(evt->state == EVT_M_DOWN) {
                    cb->value = !cb->value;
                    if(cb->events.mousePressAction) cb->events.mousePressAction(cb, evt);
                } else if(evt->state == EVT_M_UP) {
                    if(cb->events.mouseReleaseAction) cb->events.mouseReleaseAction(cb, evt);
                }
            }
        }
    }
}


static const E_Obj_Evts e_obj_evts = {
    .destruct = destruct,
    .render = render,
    .resize = resize,
    .update = NULL,
    .mouseMoveEvt = UI_OBJ_mouseMoveEvt,
    .mouseButtonEvt = mouseButtonEvt,
    .pressKeyEvt = UI_OBJ_pressKeyEvt,
    .releaseKeyEvt = UI_OBJ_releaseKeyEvt,
    .onLoad = NULL
};

/* Object functions -------------------------------------------------------- */

CheckBox * CheckBox_create(int x, int y, size_t size, bool value,
                           const char * const txt) {
    CheckBox * cb = malloc(sizeof(CheckBox));
    if(cb == NULL) return NULL;

    cb->objEvts = &e_obj_evts;
    cb->events = UI_EVENTS_INIT;

    cb->position.x = x;
    cb->position.y = y;
    cb->value = value;
    cb->width = size;
    cb->height = size;
    cb->background = UI_CHECK_BG_COLOR;
    cb->borderColor = UI_CHECK_BORDER_COLOR;
    cb->foreground = UI_CHECK_FG_COLOR;

    if(txt != NULL) {
        if(*txt != 0) {
            cb->label = Label_create(size + 10, 0, txt);
        }
    }

    return cb;
}

void CheckBox_destruct(CheckBox * cb) {
    if(cb != NULL) {
        if(cb->label) Label_destruct(cb->label);
        free(cb);
    }
}


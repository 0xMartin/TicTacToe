/**
 * <------------------------------------------------------------------>
 * @name    2D Engine
 * @author  Martin Krcma
 * @date    11. 3. 2021
 * <------------------------------------------------------------------>
 * @file    label.c
 * @brief   Implementation of label.h
 * <------------------------------------------------------------------>
 */

#include "label.h"

#include "../util.h"
#include "ui_obj.h"
#include "colors.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/* Object event functions -------------------------------------------------------- */

static void destruct(void * obj) {
    Label * btn = (Label*) obj;
    Label_destruct(btn);
}

static void render(void * obj, const Event_Render * evt) {
    Label * lab = (Label*) obj;
    if(!lab->events.visible) return;

    Render_setColor(&lab->foreground);
    if(lab->centered) {
        Render_drawString(
                    lab->position.x - Render_getStringWidth(lab->text) / 2,
                    lab->position.y,
                    lab->text);
    } else {
        Render_drawString(
                    lab->position.x,
                    lab->position.y,
                    lab->text);
    }
}


static const E_Obj_Evts e_obj_evts = {
    .destruct = destruct,
    .render = render,
    .resize = UI_OBJ_resize,
    .update = NULL,
    .mouseMoveEvt = UI_OBJ_mouseMoveEvt,
    .mouseButtonEvt = UI_OBJ_mouseButtonEvt,
    .pressKeyEvt = UI_OBJ_pressKeyEvt,
    .releaseKeyEvt = UI_OBJ_releaseKeyEvt,
    .onLoad = NULL
};

/* Object functions -------------------------------------------------------- */

Label * Label_create(int x, int y, const char * const txt) {
    Label * lab = malloc(sizeof(Label));
    if(lab == NULL) return NULL;

    lab->objEvts = &e_obj_evts;
    lab->events = UI_EVENTS_INIT;

    lab->foreground = UI_LABEL_FG_COLOR;
    lab->position.x = x;
    lab->position.y = y;
    lab->centered = false;
    if(txt != NULL) {
        lab->text = malloc(sizeof (char) * (strlen(txt) + 1));
        sprintf(lab->text, "%s", txt);
    } else {
        lab->text = NULL;
    }

    return lab;
}

void Label_destruct(Label * lab) {
    if(lab != NULL) {
        if(lab->text) free(lab->text);
        free(lab);
    }
}

void Label_changeText(Label * lab, const char * const txt) {
    if(lab == NULL) return;

    if(lab->text) free(lab->text);
    lab->text = malloc(sizeof (char) * (strlen(txt) + 1));
    sprintf(lab->text, "%s", txt);
}


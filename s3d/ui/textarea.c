/**
 * <------------------------------------------------------------------>
 * @name    2D Engine
 * @author  Martin Krcma
 * @date    19. 3. 2021
 * <------------------------------------------------------------------>
 * @file    textarea.c
 * @brief   Implementation of textarea.h
 * <------------------------------------------------------------------>
 */

#include "textarea.h"

#include "../util.h"
#include "ui_obj.h"
#include "colors.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>


/* Object event functions -------------------------------------------------------- */

static void destruct(void * obj) {
    TextArea * ta = (TextArea*) obj;
    TextArea_destruct(ta);
}

static void render(void * obj, const Event_Render * evt) {

}

static void update(void * obj, SceneData * scene, const Event_Update * evt) {

}

static void mouseMoveEvt(void * obj, SceneData * scene, const Event_Mouse * evt) {

}

static void mouseButtonEvt(void * obj, SceneData * scene, const Event_Mouse * evt) {

}

static void pressKeyEvt(void * obj, SceneData * scene, const Event_Key * evt) {

}

static void releaseKeyEvt(void * obj, SceneData * scene, const Event_Key * evt) {

}

static const E_Obj_Evts e_obj_evts = {
    .destruct = destruct,
    .render = render,
    .resize = UI_OBJ_resize,
    .update = update,
    .mouseMoveEvt = mouseMoveEvt,
    .mouseButtonEvt = mouseButtonEvt,
    .pressKeyEvt = pressKeyEvt,
    .releaseKeyEvt = releaseKeyEvt,
    .onLoad = NULL
};

/* Object functions -------------------------------------------------------- */

TextArea * TextArea_create(int x, int y, size_t width, size_t heigth,
                             size_t maxTextLength) {
    TextArea * ta = malloc(sizeof(TextArea));
    if(ta == NULL) return NULL;

    ta->objEvts = &e_obj_evts;

    return ta;
}

void TextArea_destruct(TextArea * tf) {
    if(tf != NULL) {
        if(tf->text) free(tf->text);
        free(tf);
    }
}

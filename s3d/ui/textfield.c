/**
 * <------------------------------------------------------------------>
 * @name    2D Engine
 * @author  Martin Krcma
 * @date    11. 3. 2021
 * <------------------------------------------------------------------>
 * @file    texfield.c
 * @brief   Implementation of texfield.h
 * <------------------------------------------------------------------>
 */

#include "textfield.h"

#include "../util.h"
#include "ui_obj.h"
#include "colors.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>


/* Object event functions -------------------------------------------------------- */

static void destruct(void * obj) {
    TextField * tf = (TextField*) obj;
    TextField_destruct(tf);
}

static void render(void * obj, const Event_Render * evt) {
    TextField * tf = (TextField*) obj;
    if(!tf->events.visible) return;

    if(tf->events.focus) {
        Color darker_bg = COLOR_DARKER(tf->background, 0.4);
        Render_setColor(&darker_bg);
    } else {
        Render_setColor(&tf->background);
    }
    Render_fillRectangle(&tf->position, tf->width, tf->height);
    Color border = COLOR_LIGHTER(tf->background, 0.2);
    Render_setColor(&border);
    Render_drawRectangle(&tf->position, tf->width, tf->height);


    Render_setColor(&tf->foreground);

    float line_start = (tf->height + Render_getStringHeight())/2;

    Render_setScissor(tf->position.x + 5, tf->position.y + tf->height,
                      tf->width - 10, tf->height, evt);
    Render_drawString(tf->position.x + 5 - Render_getStringWidthRange(
                          tf->text, tf->caret_position - abs(tf->caret_offset), tf->caret_position),
                      tf->position.y + line_start,
                      tf->text);

    if(tf->events.focus) {
        if(tf->caret_time % 2 == 0) {
            Point2D p1, p2;
            p1.x = tf->position.x + 5 + Render_getStringWidthIndex(tf->text, tf->caret_position + tf->caret_offset) + 2;
            p2.x = p1.x;
            p1.y = tf->position.y + line_start;
            p2.y = tf->position.y + line_start - Render_getStringHeight();
            Render_setColor(&tf->caret);
            Render_drawLine(&p1, &p2);
        }
    }

    Render_resetScissor(evt);
}

static void update(void * obj, SceneData * scene, const Event_Update * evt) {
    TextField * tf = (TextField*) obj;

    tf->caret_time = evt->ns_time/4e8;
}

static void mouseMoveEvt(void * obj, SceneData * scene, const Event_Mouse * evt) {
    TextField * tf = (TextField*) obj;
    if(!tf->events.enabled || !tf->events.visible) return;

    if(evt->x >= 0 && evt->y >= 0) {
        if(IN_RANGE(evt->x, tf->position.x, tf->position.x + tf->width)) {
            if(IN_RANGE(evt->y, tf->position.y, tf->position.y + tf->height)) {
                tf->events.hover = true;
                if(tf->events.mouseMovedAction) tf->events.mouseMovedAction(tf, evt);
                return;
            }
        }
    }
    tf->events.hover = false;
}

static void mouseButtonEvt(void * obj, SceneData * scene, const Event_Mouse * evt) {
    TextField * tf = (TextField*) obj;
    if(!tf->events.enabled || !tf->events.visible) return;

    tf->events.focus = false;
    if(evt->x >= 0 && evt->y >= 0) {
        if(IN_RANGE(evt->x, tf->position.x, tf->position.x + tf->width)) {
            if(IN_RANGE(evt->y, tf->position.y, tf->position.y + tf->height)) {
                tf->events.focus = true;
                if(evt->state == EVT_M_DOWN) {
                    if(tf->events.mousePressAction) tf->events.mousePressAction(tf, evt);
                } else if(evt->state == EVT_M_UP) {
                    if(tf->events.mouseReleaseAction) tf->events.mouseReleaseAction(tf, evt);
                }

                int min = INT16_MAX;
                int v;
                int index = 0;
                for(int i = 0; i <= tf->maxTextLength; ++i) {
                    v = tf->position.x + 5 + Render_getStringWidthIndex(tf->text, i) + 2;
                    v = abs(v - evt->x);
                    if(v < min) {
                        min = v;
                        index = i;
                    }
                }
                tf->caret_position = index;
            }
        }
    }
}

static void pressKeyEvt(void * obj, SceneData * scene, const Event_Key * evt) {
    TextField * tf = (TextField*) obj;
    if(!tf->events.enabled || !tf->events.visible) return;

    if(tf->events.focus) {
        if(tf->events.keyPressedAction) tf->events.keyPressedAction(tf, evt);

        if(isprint(evt->key)) {
            if(strlen(tf->text) + 1 <= (unsigned long)tf->maxTextLength) {
                for(int i = strlen(tf->text); i > tf->caret_position; --i) {
                    tf->text[i] = tf->text[i - 1];
                }
                tf->text[tf->caret_position] = evt->key;
                tf->caret_position++;

                if((unsigned long)Render_getStringWidthIndex(
                            tf->text, tf->caret_position + tf->caret_offset) > tf->width - 10) {
                    tf->caret_offset--;
                }
            }
        } else {
            switch(evt->key) {
            case EVT_C_BACKSPACE:
                if(tf->caret_position > 0) {
                    for(unsigned long i = tf->caret_position - 1; i < strlen(tf->text); ++i) {
                        tf->text[i] = tf->text[i + 1];
                    }
                    if(tf->caret_offset < 0) {
                        tf->caret_offset++;
                    }
                    tf->caret_position--;
                }
                break;
            }
            switch(evt->arrow) {
            case LEFT:
                if(tf->caret_position > 0) {
                    tf->caret_position--;
                }
                break;
            case RIGHT:
                if((unsigned long)tf->caret_position < strlen(tf->text)) {
                    tf->caret_position++;
                }
                break;
            }
        }
    }
}

static void releaseKeyEvt(void * obj, SceneData * scene, const Event_Key * evt) {
    TextField * tf = (TextField*) obj;
    if(!tf->events.enabled || !tf->events.visible) return;

    if(tf->events.focus) {
        if(tf->events.keyPressedAction) tf->events.keyPressedAction(tf, evt);
    }
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

TextField * TextField_create(int x, int y, size_t width, size_t heigth,
                             size_t max_str_len) {
    TextField * tf = malloc(sizeof(TextField));
    if(tf == NULL) return NULL;

    tf->objEvts = &e_obj_evts;
    tf->events = UI_EVENTS_INIT;

    tf->position.x = x;
    tf->position.y = y;
    tf->width = width;
    tf->height = heigth;
    tf->caret_position = 0;
    tf->caret_offset = 0;
    tf->background = UI_TEXTFIELD_BG_COLOR;
    tf->foreground = UI_TEXTFIELD_FG_COLOR;
    tf->caret = UI_TEXTFIELD_CARET_COLOR;
    tf->maxTextLength = max_str_len;
    tf->text = calloc(max_str_len + 1, sizeof (char));

    return tf;
}

void TextField_destruct(TextField * tf) {
    if(tf != NULL) {
        if(tf->text) free(tf->text);
        free(tf);
    }
}

bool TextField_setText(TextField * tf, const char * const txt) {
    if(tf == NULL) return false;

    size_t txt_length = strlen(txt);
    unsigned int i;
    for(i = 0; i < tf->maxTextLength - 1 && i < txt_length; ++i) {
        *(tf->text + i) = *(txt + i);
    }
    *(tf->text + i) = 0;
    return true;
}

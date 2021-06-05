/**
 * <------------------------------------------------------------------>
 * @name    2D Engine
 * @author  Martin Krcma
 * @date    19. 3. 2021
 * <------------------------------------------------------------------>
 * @file    radiobutton.c
 * @brief   Implementation of radiobutton.h
 * <------------------------------------------------------------------>
 */

#include "radiobutton.h"

#include "../util.h"
#include "ui_obj.h"
#include "colors.h"
#include <stdlib.h>


/* Object event functions -------------------------------------------------------- */

static void destruct(void * obj) {
    RadioButton * rb = (RadioButton*) obj;
    RadioButton_destruct(rb);
}

static void render(void * obj, const Event_Render * evt) {
    RadioButton * rb = (RadioButton*) obj;
    if(!rb->events.visible) return;

    Point2D p = rb->position;
    p.x += rb->width/2;
    p.y += rb->height/2;

    Render_setColor(&rb->background);
    //Render_fillRectangle(&rb->position, rb->size, rb->size);
    Render_setColor(&rb->borderColor);
    Render_lineWidth(2);
    Render_drawEllipse(&p, rb->width/2, rb->height/2);
    Render_lineWidth(RENDER_DEFAULT_LINE_WIDTH);

    if(rb->value) {
        Render_setColor(&rb->foreground);
        Render_fillEllipse(&p, rb->width/3, rb->height/3);
    }

    if(rb->label != NULL) {
        if(rb->label->objEvts->render) {
            int center = rb->height/2 + Render_getStringHeight()/3;
            //shift origin
            glTranslatef(rb->position.x, rb->position.y + center, 0);

            //render
            rb->label->objEvts->render(rb->label, evt);

            //shift origin back
            glTranslatef(-rb->position.x, -rb->position.y - center, 0);
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
    RadioButton * rb = (RadioButton*) obj;
    if(!rb->events.enabled || !rb->events.visible) return;

    rb->events.focus = false;
    if(evt->x >= 0 && evt->y >= 0) {
        if(IN_RANGE(evt->x, rb->position.x, rb->position.x + rb->width)) {
            if(IN_RANGE(evt->y, rb->position.y, rb->position.y + rb->height)) {
                rb->events.focus = true;
                if(evt->state == EVT_M_DOWN) {

                    //change value of radio button, set false value for all in group
                    rb->value = true;
                    if(rb->group) {
                        RadioButton * child;
                        LinkedList_Element * el = rb->group->radiobuttons.first;
                        while(el != NULL) {
                            if(el->ptr != obj) {
                                child = (RadioButton*) el->ptr;
                                child->value = false;
                            }
                            el = LinkedList_next(el);
                        }
                    }

                    if(rb->events.mousePressAction) rb->events.mousePressAction(rb, evt);
                } else if(evt->state == EVT_M_UP) {
                    if(rb->events.mouseReleaseAction) rb->events.mouseReleaseAction(rb, evt);
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

RadioButton * RadioButton_create(int x, int y, size_t size, bool value,
                             const char * const txt) {
    RadioButton * rb = malloc(sizeof(RadioButton));
    if(rb == NULL) return NULL;

    rb->objEvts = &e_obj_evts;
    rb->events = UI_EVENTS_INIT;
    rb->position.x = x;
    rb->position.y = y;
    rb->width = size;
    rb->height = size;
    rb->value = value;
    rb->group = NULL;
    rb->background = UI_CHECK_BG_COLOR;
    rb->borderColor = UI_CHECK_BORDER_COLOR;
    rb->foreground = UI_CHECK_FG_COLOR;

    if(txt != NULL) {
        if(*txt != 0) {
            rb->label = Label_create(size + 10, 0, txt);
        }
    }

    return rb;
}

void RadioButton_destruct(RadioButton * rb) {
    if(rb != NULL) {
        rb->events = UI_EVENTS_INIT;
        if(rb->label) Label_destruct(rb->label);
    }
}


RadioButtonGroup * RadioButtonGroup_create() {
    RadioButtonGroup * group = malloc(sizeof(RadioButtonGroup));
    if(group == NULL) return NULL;

    if(!LinkedList_init(&group->radiobuttons)) {
        free(group);
        return NULL;
    }

    return group;
}

bool RadioButtonGroup_add(RadioButtonGroup * group, RadioButton * btn) {
    if(group == NULL || btn == NULL ) return false;

    LinkedList_Element * element = malloc(sizeof(LinkedList_Element));
    element->ptr = btn;
    btn->group = group;
    element->destruct = NULL;

    return LinkedList_insert(&group->radiobuttons, element);
}

void RadioButtonGroup_destruct(RadioButtonGroup * group) {
    if(group != NULL) {
        LinkedList_destruct(&group->radiobuttons);
        free(group);
    }
}


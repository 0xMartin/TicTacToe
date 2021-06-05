#include "ui_obj.h"

void UI_OBJ_resize(void * obj, const Event_Resize * evt) {
    UI_Obj * uiobj = (UI_Obj*) obj;
    if(uiobj->events.resizable) {
        uiobj->position.x *= evt->resize_ratio_horizontal;
        uiobj->position.y *= evt->resize_ratio_vertical;
        uiobj->width *= evt->resize_ratio_horizontal;
        uiobj->height *= evt->resize_ratio_vertical;
    }
}

void UI_OBJ_mouseMoveEvt(void * obj, SceneData * scene, const Event_Mouse * evt) {
    UI_Obj * uiobj = (UI_Obj*) obj;
    if(!uiobj->events.enabled || !uiobj->events.visible) return;

    if(evt->x >= 0 && evt->y >= 0) {
        if(IN_RANGE(evt->x, uiobj->position.x, uiobj->position.x + uiobj->width)) {
            if(IN_RANGE(evt->y, uiobj->position.y, uiobj->position.y + uiobj->height)) {
                uiobj->events.hover = true;
                if(uiobj->events.mouseMovedAction) uiobj->events.mouseMovedAction(uiobj, evt);
                return;
            }
        }
    }
    uiobj->events.hover = false;
}

void UI_OBJ_mouseButtonEvt(void * obj, SceneData * scene, const Event_Mouse * evt) {
    UI_Obj * uiobj = (UI_Obj*) obj;
    if(!uiobj->events.enabled || !uiobj->events.visible) return;

    uiobj->events.focus = false;
    if(evt->x >= 0 && evt->y >= 0) {
        if(IN_RANGE(evt->x, uiobj->position.x, uiobj->position.x + uiobj->width)) {
            if(IN_RANGE(evt->y, uiobj->position.y, uiobj->position.y + uiobj->height)) {
                uiobj->events.focus = true;
                if(evt->state == EVT_M_DOWN) {
                    if(uiobj->events.mousePressAction) uiobj->events.mousePressAction(uiobj, evt);
                } else if(evt->state == EVT_M_UP) {
                    if(uiobj->events.mouseReleaseAction) uiobj->events.mouseReleaseAction(uiobj, evt);
                }
            }
        }
    }
}

void UI_OBJ_pressKeyEvt(void * obj, SceneData * scene, const Event_Key * evt) {
    UI_Obj * uiobj = (UI_Obj*) obj;
    if(!uiobj->events.enabled || !uiobj->events.visible) return;

    if(uiobj->events.focus) {
        if(uiobj->events.keyPressedAction) uiobj->events.keyPressedAction(uiobj, evt);
    }
}

void UI_OBJ_releaseKeyEvt(void * obj, SceneData * scene, const Event_Key * evt) {
    UI_Obj * uiobj = (UI_Obj*) obj;
    if(!uiobj->events.enabled || !uiobj->events.visible) return;

    if(uiobj->events.focus) {
        if(uiobj->events.keyPressedAction) uiobj->events.keyPressedAction(uiobj, evt);
    }
}

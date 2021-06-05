#ifndef UI_OBJ_H
#define UI_OBJ_H

#include "../event.h"
#include "../render.h"
#include "../engine_object.h"
#include <stdbool.h>


typedef struct {
    //[engine object event]
    const E_Obj_Evts * objEvts;

    //[data]

    //UI object events
    UI_Events events;

    //position and size of UI object
    Point2D position;
    GLfloat width;
    GLfloat height;

    //data
    void * data;
} UI_Obj;


/**
 * @brief UI_OBJ_resize
 * @param obj
 * @param evt
 */
void UI_OBJ_resize(void * obj, const Event_Resize * evt);

/**
 * @brief mouseMoveEvt
 * @param obj
 * @param cntx
 * @param evt
 */
void UI_OBJ_mouseMoveEvt(void * obj, SceneData * scene, const Event_Mouse * evt);

/**
 * @brief mouseButtonEvt
 * @param obj
 * @param cntx
 * @param evt
 */
void UI_OBJ_mouseButtonEvt(void * obj, SceneData * scene, const Event_Mouse * evt);

/**
 * @brief UI_OBJ_pressKeyEvt
 * @param obj
 * @param cntx
 * @param evt
 */
void UI_OBJ_pressKeyEvt(void * obj, SceneData * scene, const Event_Key * evt);

/**
 * @brief UI_OBJ_releaseKeyEvt
 * @param obj
 * @param cntx
 * @param evt
 */
void UI_OBJ_releaseKeyEvt(void * obj, SceneData * scene, const Event_Key * evt);


#endif // UI_OBJ_H

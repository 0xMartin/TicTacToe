/**
 * <------------------------------------------------------------------>
 * @name    2D Engine
 * @author  Martin Krcma
 * @date    28. 2. 2021
 * <------------------------------------------------------------------>
 * @file    engine_object.h
 * @brief
 * <------------------------------------------------------------------>
 */

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "core.h"
#include "vector.h"
#include "linkedlist.h"
#include "event.h"


/**
static const E_Obj_Evts e_obj_evts = {
    .destruct = NULL,
    .render = NULL,
    .update = NULL,
    .mouseMoveEvt = NULL,
    .mouseButtonEvt = NULL,
    .pressKeyEvt = NULL,
    .releaseKeyEvt = NULL,
    .onLoad = NULL
};
*/

/** structure qith virtual functions */
typedef struct {
    void (*destruct)(void * obj);  /** Destruct object */
    void (*render)(void * obj, const Event_Render * evt);    /** Render object */
    void (*resize)(void * obj, const Event_Resize * evt);    /** Render object */
    void (*update)(void * obj, SceneData * scene, const Event_Update * evt);    /** Update object */
    void (*mouseMoveEvt)(void * obj, SceneData * scene, const Event_Mouse * evt); /** Mouse move event */
    void (*mouseButtonEvt)(void * obj, SceneData * scene, const Event_Mouse * evt); /** Mouse button event */
    void (*pressKeyEvt)(void * obj, SceneData * scene, const Event_Key * evt); /** Key pressed event */
    void (*releaseKeyEvt)(void * obj, SceneData * scene, const Event_Key * evt);    /** Key released event */
    void (*onLoad)(void * obj, SceneData * scene); /** on scene load */
} E_Obj_Evts;


/** abstract object type */
typedef struct {
    const E_Obj_Evts * events;
    void * data;  /** data of object */
} E_Obj;


/**
 * @brief E_Obj_init
 * @param g_obj
 * @return
 */
bool E_Obj_init(E_Obj * g_obj);

/**
 * @brief G_Obj_destruct
 * @param g_obj
 */
void E_Obj_destruct(E_Obj * g_obj);

/**
 * @brief E_Obj_insertToList
 * @param list
 * @param obj
 * @return
 */
bool E_Obj_insertToList(LinkedList * list, E_Obj * obj);

/**
 * @brief E_Obj_insertToVector
 * @param vector
 * @param obj
 * @return
 */
bool E_Obj_insertToVector(Vector * vector, E_Obj * obj);

#endif // ENGINE_OBJECT_H

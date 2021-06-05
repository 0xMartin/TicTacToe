/**
 * <------------------------------------------------------------------>
 * @name    2D Engine
 * @author  Martin Krcma
 * @date    28. 2. 2021
 * <------------------------------------------------------------------>
 * @file    core.h
 * @brief
 * <------------------------------------------------------------------>
 */

#ifndef GAME_CORE_H
#define GAME_CORE_H

#include "api.h"
#include "util.h"
#include "event.h"
#include "io.h"
#include "linkedlist.h"
#include "vector.h"
#include "render.h"
#include <stdbool.h>


#define CORE_DEFAULT_CONFIG {\
    800,\
    600,\
    "Window",\
    false,\
    50,\
    50,\
    true,\
    false,\
    (Color){0.0, 0.0, 0.0, 1.0},\
    NULL,\
    NULL\
    }

typedef struct {
    LinkedList * gameData;  /** game data, list consisting only from <E_Obj> engine_object.h */
    Vector * files; /** vector of all opened files */
} SceneData;

typedef struct {
    unsigned int window_width;  /** Window width */
    unsigned int window_height; /** Window height */
    char windonw_title[255];   /** Window title */
    bool window_fullScreen; /** True -> fullscreen mode */
    unsigned int fps;   /** Frames per second */
    unsigned int ups;   /** Updates per second */
    bool visibleCursor; /** Is cursor visible? */
    bool mouseCamControl; /** Control camera by mouse motion */
    Color clearColor; /** Background color of window */
    SceneData * scene;  /** Core context */
    Vector * textures;    /** vector of all textures, consisting only from <Texture> util.h */
} CORE;


/**
 * @brief CORE_engineInit
 * @param argc
 * @param argv
 * @param core
 * @return
 */
E2API bool CORE_init(int argc, char **argv, CORE * core);

/**
 * @brief CORE_setSceneData
 * @param contx
 * @return
 */
E2API bool CORE_setSceneData(SceneData * scene);

/**
 * @brief CORE_run
 * @param core
 * @return
 */
E2API bool CORE_run();

/**
 * @brief CORE_stop
 */
E2API void CORE_stop();

/**
 * @brief CORE_destruct
 */
E2API bool CORE_destruct();

/**
 * @brief CORE_loadTexture
 * @param path
 * @param texture
 * @return
 */
E2API bool CORE_loadTexture(const char * path, Texture ** texture, bool rgba_mode);

/**
 * @brief CORE_getTextures
 * @return
 */
E2API Vector * CORE_getTextures();

/**
 * @brief SceneData_create
 * @return
 */
E2API SceneData * SceneData_create();

/**
 * @brief SceneData_destruct
 * @param contx
 * @return
 */
E2API bool SceneData_destruct(SceneData * scene);


#endif // CORE_H

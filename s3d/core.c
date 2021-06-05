/**
 * <------------------------------------------------------------------>
 * @name    2D Engine
 * @author  Martin Krcma
 * @date    28. 2. 2021
 * <------------------------------------------------------------------>
 * @file    core.c
 * @brief   Implementation of core.h
 * <------------------------------------------------------------------>
 */

#include "core.h"

#include <pthread.h>
#include <unistd.h>
#include "engine_object.h"


static CORE * _core = NULL;

static pthread_t _thread_update;

static bool _running = false;
static bool _glut_main_loop = false;

//core events iterrupts
static bool _interrupt_resize = false;
static bool _interrupt_render = false;
static bool _interrupt_update = false;
static bool _interrupt_press_key = false;
static bool _interrupt_release_key = false;
static bool _interrupt_mouse_move = false;
static bool _interrupt_mouse_button = false;

//core events
static Event_Key    _key_event_press;
static Event_Key    _key_event_release;
static Event_Mouse  _mouse_event_move;
static Event_Mouse  _mouse_event_button;
static Event_Render _render_event;
static Event_Resize _resize_event;
static Event_Update _update_event;


/* Private func declaration -------------------------------------------------------- */

static bool switchSceneData(SceneData * data);

/**
 * @brief Keyboard pressed function
 * @param key   Pressed key
 * @param x     none
 * @param y     none
 */
static void keyboardFunc(unsigned char key, int x, int y);

/**
 * @brief Keyboard released function
 * @param key   Released key
 * @param x     none
 * @param y     none
 */
static void keyboardUpFunc(unsigned char key, int x, int y);

/**
 * @brief Special keyboard event (pressed)
 * @param key   Key ID
 * @param x     none
 * @param y     none
 */
static void specialFunc(int key, int x, int y);

/**
 * @brief specialUpFunc     Special keyboard event (released)
 * @param key   Key ID
 * @param x     none
 * @param y     none
 */
static void specialUpFunc(int key, int x, int y);

/**
 * @brief Key press event
 * @param key   Pressed key (char)
 * @param ctrl  CTRL is down?
 * @param alt   ALT is down?
 * @param shift SHIFT is down?
 */
static void evt_pressKey(unsigned char key,  bool ctrl,
                         bool alt, bool shift, Arrow_key arrow);

/**
 * @brief Key release event
 * @param key   Released key (char)
 * @param ctrl  CTRL is down?
 * @param alt   ALT is down?
 * @param shift SHIFT is down?
 */
static void evt_releaseKey(unsigned char key,  bool ctrl,
                           bool alt, bool shift, Arrow_key arrow);

/**
 * @brief Mouse move event
 * @param x         X position of mouse cursore
 * @param y         Y position of mouse cursore
 */
static void evt_mouseMove(int x, int y);

/**
 * @brief Mouse button event
 * @param button    Button ID
 * @param state     Button state
 * @param x         none
 * @param y         none
 */
static void evt_mouseButton(int button, int state, int x, int y);

/**
 * @brief GLUT reshape function
 * @param w Window width
 * @param h Window height
 */
static void reshape(int w, int h);

/**
 * @brief Update eache E_Obj of current scene
 */
static void updateScene();

/**
 * @brief Rendering loop
 */
static void renderLoop();

/**
 * @brief Update loop
 */
static void * updateLoop(void * args);

/**
 * @brief Render each E_Obj of current scene
 */
static void renderScene();

/**
 * @brief Exit event of engine
 */
static void exitEvent();


bool CORE_init(int argc, char **argv, CORE * core) {
    if(_core != NULL || core == NULL || argc < 0 || argv == NULL) return false;

    //init core struct
    _core = core;
    _core->scene = NULL;

    _core->textures = (Vector*) malloc(sizeof(Vector));
    if(!Vector_init(_core->textures, 20, 10)) return false;

    //init glut window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(core->window_width, core->window_height);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - core->window_width) / 2,
                           (glutGet(GLUT_SCREEN_HEIGHT) - core->window_height) / 2
                           );
    glutCreateWindow(core->windonw_title);

    //exit evt
    atexit(exitEvent);

    //rendering
    glutDisplayFunc(renderScene);
    glutReshapeFunc(reshape);

    //control events
    glutKeyboardFunc(keyboardFunc);
    glutSpecialFunc(specialFunc);
    glutKeyboardUpFunc(keyboardUpFunc);
    glutSpecialUpFunc(specialUpFunc);
    glutIgnoreKeyRepeat(1);
    glutMouseFunc(evt_mouseButton);
    glutPassiveMotionFunc(evt_mouseMove);

    //opengl config
    glEnable(GL_SCISSOR_TEST);
    glDepthFunc(GL_NEVER);
    glEnable(GL_MULTISAMPLE);
    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //cursor
    if(!core->visibleCursor) {
        glutSetCursor(GLUT_CURSOR_NONE);
    }

    return true;
}

bool CORE_setSceneData(SceneData * scene) {
    if(_core == NULL || scene == NULL) return false;

    bool status = false;
    if(_core->scene == NULL) {
        _core->scene = scene;
        status = true;
    } else {
        status = switchSceneData(scene);
    }

    E_Obj * obj;
    LinkedList_Element * el = _core->scene->gameData->first;
    while(el != NULL) {
        if(el->ptr != NULL) {
            obj = (E_Obj*) el->ptr;
            if(obj->events) {
                if(obj->events->onLoad) obj->events->onLoad(obj, _core->scene);
            }
        }
        el = LinkedList_next(el);
    }

    return status;
}

bool CORE_run() {
    if(_core == NULL) return false;

    _interrupt_resize = true;
    _interrupt_render = true;
    _interrupt_update = true;
    _interrupt_press_key = true;
    _interrupt_release_key = true;
    _interrupt_mouse_move = true;
    _interrupt_mouse_button = true;
    _running = true;

    if(pthread_create(&_thread_update, NULL, updateLoop, NULL)) {
        return false;
    }

    renderLoop();

    if(!_glut_main_loop) {
        _glut_main_loop = true;
        glutMainLoop();
    }

    return true;
}


void CORE_stop() {
    if(_core == NULL) return;
    _interrupt_resize = false;
    _interrupt_render = false;
    _interrupt_update = false;
    _interrupt_press_key = false;
    _interrupt_release_key = false;
    _interrupt_mouse_move = false;
    _interrupt_mouse_button = false;
    _running = false;
}


bool CORE_destruct() {
    if(_core == NULL) return false;

    if(SceneData_destruct(_core->scene)) {
        _core->scene = NULL;
    }

    if(_core->textures != NULL) {
        Vector_destruct(_core->textures);
        free(_core->textures);
    }

    return true;
}

bool CORE_loadTexture(const char * path, Texture ** texture, bool rgba_mode) {
    if(_core == NULL) return false;
    if(_core->textures == NULL) return false;

    Texture * tex = UTIL_loadTextureBMP(path, rgba_mode);
    if(tex == NULL) return false;
    Vector_Element * el = (Vector_Element*) malloc(sizeof (Vector_Element));
    if(el == NULL) return false;
    el->ptr = tex;
    el->destruct = UTIL_simpleDestructor;
    Vector_append(_core->textures, el);
    if(texture != NULL) {
        *texture = tex;
    }

    return true;
}

E2API Vector * CORE_getTextures() {
    if(_core == NULL) return NULL;
    if(_core->textures == NULL) return NULL;

    return _core->textures;
}

SceneData * SceneData_create() {
    SceneData * scene = (SceneData*) malloc(sizeof(SceneData));
    if(scene == NULL) return NULL;

    scene->gameData = (LinkedList*) malloc(sizeof(LinkedList));
    if(!LinkedList_init(scene->gameData)) {
        free(scene);
        return NULL;
    }

    scene->files = (Vector*) malloc(sizeof(Vector));
    if(!Vector_init(scene->files, 20, 10)) {
        free(scene->gameData);
        free(scene);
        return NULL;
    }

    return scene;
}

bool SceneData_destruct(SceneData * scene) {
    if(scene == NULL) return false;

    if(scene) {
        if(scene->gameData) {
            LinkedList_destruct(scene->gameData);
        }
        if(scene->files) {
            Vector_destruct(scene->files);
        }
        free(scene);
    }

    return true;
}

static void reshape(int w, int h) {
    if(_core != NULL) {
        _resize_event.resize_ratio_horizontal = (double) w / (double) _core->window_width;
        _resize_event.resize_ratio_vertical = (double) h / (double) _core->window_height;
        _resize_event.current_window_width = w;
        _resize_event.current_window_height = h;

        _core->window_width = w;
        _core->window_height = h;
        glViewport(0, 0, w, h);       /* Establish viewing area to cover entire window. */
        glMatrixMode(GL_PROJECTION);  /* Start modifying the projection matrix. */
        glLoadIdentity();             /* Reset project matrix. */
        glOrtho(0, w, 0, h, -1, 1);   /* Map abstract coords directly to window coords. */
        glScalef(1, -1, 1);           /* Invert Y axis so increasing Y goes down. */
        glTranslatef(0, -h, 0);       /* Shift origin up to upper-left corner. */

        if(_core->scene != NULL) {
            if(_core->scene->gameData != NULL) {
                E_Obj * obj;
                LinkedList_Element * el = _core->scene->gameData->first;
                while(el != NULL) {
                    if(_interrupt_resize) {
                        _interrupt_resize = false;
                        break;
                    }
                    if(el->ptr != NULL) {
                        obj = (E_Obj*) el->ptr;
                        if(obj->events) {
                            if(obj->events->resize) obj->events->resize(obj, &_resize_event);
                        }
                    }
                    el = LinkedList_next(el);
                }
            }
        }
    }
}

static void renderScene() {
    if(_core == NULL) return;

    _render_event.window_width = _core->window_width;
    _render_event.window_height = _core->window_height;
    _render_event.sender = _core;

    Render_clear(&_render_event, &_core->clearColor);

    if(_core->scene == NULL) {
        glutSwapBuffers();
        return;
    }
    if(_core->scene->gameData == NULL) {
        glutSwapBuffers();
        return;
    }

    E_Obj * obj;
    LinkedList_Element * el = _core->scene->gameData->first;
    while(el != NULL) {
        if(_interrupt_render) {
            _interrupt_render = false;
            break;
        }
        if(el->ptr != NULL) {
            obj = (E_Obj*) el->ptr;
            if(obj->events) {
                if(obj->events->render) obj->events->render(obj, &_render_event);
            }
        }
        el = LinkedList_next(el);
    }

    glutSwapBuffers();
}

static void updateScene() {
    if(_core == NULL) return;
    if(!_running) return;

    if(_core->scene != NULL) {
        if(_core->scene->gameData != NULL) {

            struct timespec time = UTIL_getSystemTime();
            //diff
            __syscall_slong_t diff = time.tv_nsec - _update_event.ns_time;
            if (diff < 0) {
                _update_event.ns_diff = time.tv_nsec - _update_event.ns_time + 1000000000UL;
                _update_event.s_diff = time.tv_sec - _update_event.s_time + 1;
            } else {
                _update_event.ns_diff = diff;
                _update_event.s_diff = time.tv_sec - _update_event.s_time;
            }
            //current time
            _update_event.ns_time = time.tv_nsec;
            _update_event.s_time = time.tv_sec;
            _update_event.sender = &_core;

            //update all
            E_Obj * obj;
            LinkedList_Element * el = _core->scene->gameData->first;
            while(el != NULL) {
                if(_interrupt_update) {
                    _interrupt_update = false;
                    break;
                }
                if(el->ptr != NULL) {
                    obj = (E_Obj*) el->ptr;
                    if(obj->events) {
                        if(obj->events->update) obj->events->update(obj, _core->scene,
                                                                    &_update_event);
                    }
                }
                el = LinkedList_next(el);
            }
        }
    }
}

static void renderLoop() {
    if(!_core) return;
    if(!_running) return;
    glutPostRedisplay();
    glutTimerFunc(1000.0/_core->fps, renderLoop, 0);
}

static void * updateLoop(void * args) {
    while(_running) {
        updateScene();
        usleep((unsigned int)(1e6/_core->ups));
    }
    return NULL;
}

static void keyboardFunc(unsigned char key, int x, int y) {
    int mod = glutGetModifiers();
    evt_pressKey(key, mod == GLUT_ACTIVE_CTRL,
                 mod == GLUT_ACTIVE_ALT, mod == GLUT_ACTIVE_SHIFT, NONE);
}

static void keyboardUpFunc(unsigned char key, int x, int y) {
    int mod = glutGetModifiers();
    evt_releaseKey(key, mod == GLUT_ACTIVE_CTRL,
                   mod == GLUT_ACTIVE_ALT, mod == GLUT_ACTIVE_SHIFT, NONE);
}

static void specialFunc(int key, int x, int y) {
    Arrow_key arrow_key = NONE;
    switch(key)
    {
    case GLUT_KEY_UP:
        arrow_key = UP;
        break;
    case GLUT_KEY_DOWN:
        arrow_key = DOWN;
        break;
    case GLUT_KEY_LEFT:
        arrow_key = LEFT;
        break;
    case GLUT_KEY_RIGHT:
        arrow_key = RIGHT;
        break;
    }
    evt_pressKey(0x0, key == 0x72, key == 0x74, key == 0x70, arrow_key);
}

static void specialUpFunc(int key, int x, int y) {
    Arrow_key arrow_key = NONE;
    switch(key)
    {
    case GLUT_KEY_UP:
        arrow_key = UP;
        break;
    case GLUT_KEY_DOWN:
        arrow_key = DOWN;
        break;
    case GLUT_KEY_LEFT:
        arrow_key = LEFT;
        break;
    case GLUT_KEY_RIGHT:
        arrow_key = RIGHT;
        break;
    }
    evt_releaseKey(0x0, key == 0x72, key == 0x74, key == 0x70, arrow_key);
}

static void evt_pressKey(unsigned char key, bool ctrl,
                         bool alt, bool shift, Arrow_key arrow) {
    _key_event_press.key = key;
    _key_event_press.ctrl = ctrl;
    _key_event_press.alt = alt;
    _key_event_press.shift = shift;
    _key_event_press.arrow = arrow;
    _key_event_press.sender = &_core;

    if(_core == NULL) return;
    if(!_running) return;
    if(_core->scene == NULL) return;
    if(_core->scene->gameData == NULL) return;

    E_Obj * obj;
    LinkedList_Element * el = _core->scene->gameData->first;
    while(el != NULL) {
        if(_interrupt_press_key) {
            _interrupt_press_key = false;
            break;
        }
        if(el->ptr != NULL) {
            obj = (E_Obj*) el->ptr;
            if(obj->events) {
                if(obj->events->pressKeyEvt) obj->events->pressKeyEvt(obj, _core->scene,
                                                                      &_key_event_press);
            }
        }
        el = LinkedList_next(el);
    }
}

static void evt_releaseKey(unsigned char key, bool ctrl,
                           bool alt, bool shift, Arrow_key arrow) {
    _key_event_release.key = key;
    _key_event_release.ctrl = ctrl;
    _key_event_release.alt = alt;
    _key_event_release.shift = shift;
    _key_event_release.arrow = arrow;
    _key_event_release.sender = &_core;

    if(_core == NULL) return;
    if(!_running) return;
    if(_core->scene == NULL) return;
    if(_core->scene->gameData == NULL) return;

    E_Obj * obj;
    LinkedList_Element * el = _core->scene->gameData->first;
    while(el != NULL) {
        if(_interrupt_release_key) {
            _interrupt_release_key = false;
            break;
        }
        if(el->ptr != NULL) {
            obj = (E_Obj*) el->ptr;
            if(obj->events) {
                if(obj->events->releaseKeyEvt) obj->events->releaseKeyEvt(obj, _core->scene,
                                                                          &_key_event_release);
            }
        }
        el = LinkedList_next(el);
    }
}

static void evt_mouseMove(int x, int y) {
    _mouse_event_move.x = x;
    _mouse_event_move.y = y;
    _mouse_event_button.sender = &_core;

    if(_core == NULL) return;
    if(!_running) return;
    if(_core->scene == NULL) return;
    if(_core->scene->gameData == NULL) return;

    E_Obj * obj;
    LinkedList_Element * el = _core->scene->gameData->first;
    while(el != NULL) {
        if(_interrupt_mouse_move) {
            _interrupt_mouse_move = false;
            break;
        }
        if(el->ptr != NULL) {
            obj = (E_Obj*) el->ptr;
            if(obj->events) {
                if(obj->events->mouseMoveEvt) obj->events->mouseMoveEvt(obj, _core->scene,
                                                                        &_mouse_event_move);
            }
        }
        el = LinkedList_next(el);
    }
}

static void evt_mouseButton(int button, int state, int x, int y) {
    _mouse_event_button.button = button;
    _mouse_event_button.state = state;
    _mouse_event_button.x = x;
    _mouse_event_button.y = y;
    _mouse_event_button.y = y;
    _mouse_event_button.sender = &_core;

    if(_core == NULL) return;
    if(!_running) return;
    if(_core->scene == NULL) return;
    if(_core->scene->gameData == NULL) return;

    E_Obj * obj;
    LinkedList_Element * el = _core->scene->gameData->first;
    while(el != NULL) {
        if(_interrupt_mouse_button) {
            _interrupt_mouse_button = false;
            break;
        }
        if(el->ptr != NULL) {
            obj = (E_Obj*) el->ptr;
            if(obj->events) {
                if(obj->events->mouseButtonEvt) obj->events->mouseButtonEvt(obj, _core->scene,
                                                                            &_mouse_event_button);
            }
        }
        el = LinkedList_next(el);
    }
}

static bool switchSceneData(SceneData * data) {
    if(data == NULL) return false;
    if(_core == NULL) return false;

    _interrupt_resize = false;
    _interrupt_render = false;
    _interrupt_update = false;
    _interrupt_press_key = false;
    _interrupt_release_key = false;
    _interrupt_mouse_move = false;
    _interrupt_mouse_button = false;

    _core->scene = data;

    return true;
}

static void exitEvent() {
    if(_core != NULL) {

        _running = false;
        _interrupt_resize = false;
        _interrupt_render = false;
        _interrupt_update = false;
        _interrupt_press_key = false;
        _interrupt_release_key = false;
        _interrupt_mouse_move = false;
        _interrupt_mouse_button = false;
        sleep(1);

        CORE_destruct();
    }
}

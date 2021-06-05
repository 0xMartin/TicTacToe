/**
 * <------------------------------------------------------------------>
 * @name    2D Engine
 * @author  Martin Krcma
 * @date    28. 2. 2021
 * <------------------------------------------------------------------>
 * @file    ai.h
 * @brief   AI for player
 * <------------------------------------------------------------------>
 */

#ifndef AI_H
#define AI_H

#include <stdbool.h>

typedef struct _AI {
    bool (*init)(struct _AI ai);    /** Init AI */
    bool (*destruct)(struct _AI ai);    /** Destruct AI */
    bool (*process)(struct _AI ai, void * gameData);    /** Process game data and perform action */
} AI;

#endif // AI_H

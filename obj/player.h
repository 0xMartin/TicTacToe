#ifndef PLAYER_H
#define PLAYER_H


#include <stdbool.h>
#include "ai.h"

#define PLAYER_NAME_MAX_LENGTH 255
#define PLAYER_TIME_PER_TURN 5

typedef void (*Player_timeOut)(void * player);

typedef struct {
    //[engine object event]
    const E_Obj_Evts * events;
    //[data]
    //player info panel
    Point2D position;
    size_t width;
    //player data
    char * name;
    unsigned int score;
    AI * ai;
    bool active;
    float time;
    Player_timeOut timeOut;
} Player;


/**
 * @brief Player_create
 * @param name
 * @param score
 * @param timeOut
 * @return
 */
Player * Player_create(const char * const name, unsigned int score, Player_timeOut timeOut);

/**
 * @brief Player_destruct
 * @param p
 */
void Player_destruct(Player * p);

/**
 * @brief Player_setName
 * @param p
 * @param name
 * @return
 */
bool Player_setName(Player * p, const char * const name);

/**
 * @brief Player_setAI
 * @param p
 * @param ai
 */
void Player_setAI(Player * p, AI * ai);

/**
 * @brief Player_setActive
 * @param p
 */
void Player_activate(Player * p);

/**
 * @brief Player_deactivate
 * @param p
 */
void Player_deactivate(Player * p);


#endif // PLAYER_H

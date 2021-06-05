#ifndef GAMEBOARD_H
#define GAMEBOARD_H


#include "../s3d/engine_object.h"
#include "../s3d/linkedlist.h"
#include "../s3d/render.h"

#include "cell.h"
#include "player.h"

typedef void (*GameEnd)(void * winner);

typedef struct {
    //[engine object event]
    const E_Obj_Evts * events;
    //[data]
    Point2D position;
    GLfloat size;

    Cell * cells;
    unsigned int cell_count;

    Player * player1;
    Player * player2;
    bool firstPlayerOnTurn;

    Point2D * line;
    bool gameEnd;
    GameEnd gameEndEvt;
} GameBoard;

/**
 * @brief GameBoard_create
 * @param x
 * @param y
 * @param size
 * @param count
 * @param gameEndEvt
 * @return
 */
GameBoard * GameBoard_create(int x, int y, size_t size, unsigned int count, GameEnd gameEndEvt);

/**
 * @brief GameBoard_destruct
 * @param board
 */
void GameBoard_destruct(GameBoard * board);

/**
 * @brief GameBoard_turn
 * @param board
 * @param x
 * @param y
 */
bool GameBoard_turn(GameBoard * board, unsigned int x, unsigned int y, Symbol symbol);

/**
 * @brief GameBoard_clearGame
 * @param board
 */
void GameBoard_clearGame(GameBoard * board);

/**
 * @brief GameBoard_find5InLine
 * @param board
 * @param start_end
 */
bool GameBoard_find5InLine(GameBoard * board, Point2D * start_end);

/**
 * @brief GameBoard_setPlayers
 * @param board
 * @param p1
 * @param p2
 */
void GameBoard_setPlayers(GameBoard * board, Player * p1, Player * p2);


#endif // GAMEBOARD_H

#ifndef AI_H
#define AI_H

#include "cell.h"

typedef struct {
    int x;
    int y;
} Node;

typedef struct {
    unsigned int search_depth;
    Symbol symbol;
    unsigned int cell_count;
    Symbol * gameData;

    int * values;
} AI;

/**
 * @brief AI_create
 * @param search_depth
 * @return
 */
AI * AI_create(unsigned int search_depth);

/**
 * @brief AI_destruct
 * @param ai
 */
void AI_destruct(AI * ai);

/**
 * @brief AI_refeshGameData
 * @param ai
 * @param cells
 * @param count
 * @param symbol
 */
void AI_refeshGameData(AI * ai, Cell * cells, unsigned int count, Symbol symbol);

/**
 * @brief AI_doTurn
 * @param ai
 * @return
 */
Node AI_doTurn(AI * ai);


#endif // AI_H

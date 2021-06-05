 #include "ai.h"


#include <stdlib.h>
#include <limits.h>

#define MAX(a, b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })
#define MIN(a, b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })
#define OPPOSITE(s) (s == Symbol_X ? Symbol_O : Symbol_X)

#define NODE_BUFFER_SIZE 128


typedef struct {
    Node data[NODE_BUFFER_SIZE];
    unsigned int count;
} Nodes;


static int alphabeta(AI * ai, Node node, unsigned int depth,
                     int alpha, int beta, Symbol turnNow);

static void getPosibleMoves(Nodes * nodes, AI * ai);

static Node checkForWin(AI * ai);

static int evaluate(AI * ai, Symbol turnNow);

static int evaluateNode(AI * ai, Node node, Symbol turnNow);



AI * AI_create(unsigned int search_depth) {
    if(search_depth == 0) return NULL;

    AI * ai = malloc(sizeof(AI));
    if(ai == NULL) return NULL;

    ai->gameData = NULL;
    ai->cell_count = 0;
    ai->search_depth = search_depth;
    ai->symbol = Symbol_None;
    ai->values = NULL;

    return ai;
}

void AI_destruct(AI * ai) {
    if(ai != NULL) {
        if(ai->gameData) free(ai->gameData);
        if(ai->values) free(ai->values);
        free(ai);
    }
}

void AI_refeshGameData(AI * ai, Cell * cells, unsigned int count, Symbol symbol) {
    if(ai == NULL || cells == NULL || count == 0 || symbol == Symbol_None) return;

    if(ai->gameData == NULL) {
        ai->gameData = malloc(sizeof(Symbol) * count * count);
        ai->values = malloc(sizeof(int) * count * count);
    } else if(ai->cell_count != count) {
        Symbol * newData = realloc(ai->gameData, sizeof(Symbol) * count * count);
        if(newData == NULL) return;
        ai->gameData = newData;
        int * newValues = malloc(sizeof(int) * count * count);
        if(newValues == NULL) return;
        ai->values = newValues;
    }

    ai->cell_count = count;
    ai->symbol = symbol;

    for(unsigned int i = 0; i < ai->cell_count * ai->cell_count; ++i) {
        ai->gameData[i] = cells[i].symbol;
    }
}

//###############################################################################################
//  AI  #########################################################################################
//###############################################################################################

Node AI_doTurn(AI * ai) {
    if(ai == NULL) return (Node){.x = -1, .y = -1};

    int max = INT_MIN;
    int value;
    Node best = {.x = -1, .y = -1};
    Node node;

    Node win = checkForWin(ai);
    if(win.x >= 0 && win.y >= 0) {
        return win;
    }

    Nodes nodes;
    getPosibleMoves(&nodes, ai);
    for(int i = 0; i < (int) nodes.count; ++i) {
        node = nodes.data[i];
        value = alphabeta(ai, nodes.data[i], ai->search_depth - 1, INT_MIN, INT_MAX, ai->symbol);
        if(value > max) {
            max = value;
            best = node;
        }
    }

    return best;
}

static int alphabeta(AI * ai, Node node, unsigned int depth,
                     int alpha, int beta, Symbol turnNow) {

    int index = node.x + node.y * ai->cell_count;
    ai->gameData[index] = turnNow;

    Nodes nodes;
    getPosibleMoves(&nodes, ai);

    if(depth <= 0 || nodes.count == 0) {
        int value = evaluate(ai, turnNow);
        ai->gameData[index] = Symbol_None;
        return value;
    } else if(turnNow == ai->symbol) {
        //AI
        int value = INT_MAX;
        Symbol next_turn = OPPOSITE(turnNow);

        for(unsigned int i = 0; i < nodes.count; ++i) {
            value = MIN(value, alphabeta(ai, nodes.data[i], depth - 1, alpha, beta, next_turn));
            beta = MIN(beta, value);
            if(beta <= alpha) {
                break;
            }
        }

        ai->gameData[index] = Symbol_None;
        return value;
    } else {
        //opponent
        int value = INT_MIN;
        Symbol next_turn = OPPOSITE(turnNow);

        for(unsigned int i = 0; i < nodes.count; ++i) {
            value = MAX(value, alphabeta(ai, nodes.data[i], depth - 1, alpha, beta, next_turn));
            alpha = MAX(alpha, value);
            if(alpha >= beta) {
                break;
            }
        }

        ai->gameData[index] = Symbol_None;
        return value;
    }
}

#define SEARCH_RANGE 1

static void getPosibleMoves(Nodes * nodes, AI * ai) {
    nodes->count = 0;

    int x, y;
    int i, j;
    bool add;

    for(x = 0; x < (int) ai->cell_count; ++x) {
        for(y = 0; y < (int) ai->cell_count; ++y) {
            if(ai->gameData[x + y * ai->cell_count] == Symbol_None) {
                add = false;
                for(i = -SEARCH_RANGE; i <= SEARCH_RANGE; ++i) {
                    if(x + i < 0) continue;
                    if(x + i >= (int) ai->cell_count) break;
                    for(j = -SEARCH_RANGE; j <= SEARCH_RANGE; ++j) {
                        if(y + j < 0) continue;
                        if(y + j >= (int) ai->cell_count) break;
                        if(ai->gameData[x + i + (y + j) * ai->cell_count] != Symbol_None) {
                            add = true;
                            goto ADD;
                        }
                    }
                }
ADD:
                if(add) {
                    if(nodes->count < NODE_BUFFER_SIZE) {
                        nodes->data[nodes->count].x = x;
                        nodes->data[nodes->count].y = y;
                        ++nodes->count;
                    } else {
                        return;
                    }
                }
            }
        }
    }

    if(nodes->count == 0) {
        int range = ai->cell_count / 4;
        range = range % 2 != 0 ? range + 1 : range;
        srand(time(0));
        int x = ai->cell_count / 2 + (rand() % range - range / 2);
        srand(time(0));
        int y = ai->cell_count / 2 + (rand() % range - range / 2);
        x = x % ai->cell_count;
        y = y % ai->cell_count;
        nodes->data[0].x = x;
        nodes->data[0].y = y;
        nodes->count = 1;
    }
}

static Node checkForWin(AI * ai) {

    int x, y;
    int dx, dy;
    int offset;
    int gap_index;
    int nx, ny;
    Symbol s;

    for(x = 0; x < (int) ai->cell_count; ++x) {
        for(y = 0; y < (int) ai->cell_count; ++y) {
            if(ai->gameData[x + y * ai->cell_count] != ai->symbol) continue;

            for(dx = -1; dx <= 1; ++dx) {
                for(dy = -1; dy <= 1; ++dy) {
                    if(dx == 0 && dy == 0) continue;
                    gap_index = -1;
                    for(offset = 1; offset < 5; ++offset) {
                        nx = x + offset * dx;
                        ny = y + offset * dy;

                        if(nx < 0 || nx >= (int) ai->cell_count) break;
                        if(ny < 0 || ny >= (int) ai->cell_count) break;

                        s = ai->gameData[nx + ny * ai->cell_count];
                        if(s == Symbol_None) {
                            if(gap_index >= 0) break;
                            gap_index = offset;
                        } else if (s != ai->symbol) {
                            break;
                        }
                    }
                    if(gap_index >= 0 && offset == 5) return (Node){.x=x+gap_index*dx, .y=y+gap_index*dy};
                }
            }

        }
    }

    return (Node){.x=-1, .y=-1};
}


#define ABS(val) (val < 0 ? val * -1 : val)

static int evaluate(AI * ai, Symbol turnNow) {
    int value = 0;

    //evalueate all cells
    Node node;
    int current;
    for(unsigned int x = 0; x < ai->cell_count; ++x) {
        for(unsigned int y = 0; y < ai->cell_count; ++y) {
            node.x = x;
            node.y = y;
            current = evaluateNode(ai, node, turnNow);
            ai->values[x + y * ai->cell_count] = current;
            value += current;
        }
    }

    return value;
}

typedef enum {
    FIVE = 1000000,
    FOUR = 5000,
    FOUR_BLOCKED = 1000,
    THREE = 500,
    THREE_BLOCKED = 200,
    TWO = 100,
    TWO_BLOCKED = 20
} Score;

static int OPEN_SCORE[3] = {TWO, THREE, FOUR};
static int BLOCKED_SCORE[3] = {TWO_BLOCKED, THREE_BLOCKED, FOUR_BLOCKED};

#define EVAL_MAX_GAPS 1
#define SIGN(val) ((0 < val) - (val < 0))

static int evaluateNode(AI * ai, Node node, Symbol turnNow) {

    Symbol origin = ai->gameData[node.x + node.y * ai->cell_count];
    if(origin == Symbol_None) return 0;

    int value = 0;

    int offset;
    int cnt;
    int gaps;
    int x, y;
    int dx, dy;
    Symbol current;

    for(dx = -1; dx <= 1; ++dx) {
        for(dy = -1; dy <= 1; ++dy) {
            if(dx == 0 && dy == 0) continue;

            //process symbols
            for(offset = 0, cnt = 0, gaps = 0; offset < 5; ++offset) {
                x = node.x + offset * dx;
                y = node.y + offset * dy;

                if(x < 0 || y < 0) break;
                if(x >= (int) ai->cell_count || y >= (int) ai->cell_count) break;

                current = ai->gameData[x + y * ai->cell_count];
                if(current == origin) {
                    if(gaps > EVAL_MAX_GAPS) break;
                    ++cnt;
                } else if(current == Symbol_None) {
                    ++gaps;
                } else {
                    break;
                }
            }

            //add score
            if(cnt < 2) continue;

            int sign = 1;
            if(turnNow == Symbol_None) {
                sign = ai->symbol == origin ? 1 : -1;
            } else if(turnNow == ai->symbol) {
                sign = ai->symbol == origin ? 1 : -2;
            } else {
                sign = ai->symbol == origin ? 2 : -1;
            }

            if(cnt == 5) {
                value += FIVE * sign;
            } else {
                if(offset != 5) {
                    //blocked from end side
                    x = node.x - dx;
                    y = node.y - dy;
                    if(x >= 0 && x < (int) ai->cell_count && y >= 0 && y < (int) ai->cell_count) {
                        current = ai->gameData[x + y * ai->cell_count];
                        if(current != OPPOSITE(origin)) {
                            //add only if not blocked from start
                            value += BLOCKED_SCORE[cnt - 2] * sign;
                        }
                    }
                } else {
                    //opened from end side
                    x = node.x - dx;
                    y = node.y - dy;
                    if(x >= 0 && x < (int) ai->cell_count && y >= 0 && y < (int) ai->cell_count) {
                        current = ai->gameData[x + y * ai->cell_count];
                        if(current != OPPOSITE(origin)) {
                            //opened from both sides
                            value += OPEN_SCORE[cnt - 2] * sign;
                        } else {
                            //blocked from beginning
                            value += BLOCKED_SCORE[cnt - 2] * sign;
                        }
                    } else {
                        //blocked from beginning
                        value += BLOCKED_SCORE[cnt - 2] * sign;
                    }
                }
            }

        }
    }

    return value;
}

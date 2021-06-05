#include "gameboard.h"

#include <assert.h>
#include "ai.h"


static void destruct(void * obj) {
    GameBoard * board = (GameBoard*) obj;
    GameBoard_destruct(board);
}

static void render(void * obj, const Event_Render * evt) {
    GameBoard * board = (GameBoard*) obj;

    if(board->cells != NULL) {
        Render_applyOffset(board->position.x, board->position.y, 0.0);

        //render fields
        Cell * cell;
        for(unsigned int i = 0; i < board->cell_count * board->cell_count; ++i) {
            cell = &board->cells[i];
            if(cell->events->render) cell->events->render(cell, evt);
        }

        //render win line
        if(board->gameEnd && board->line->x != -1) {
            float fSize = (float)board->size / (float)board->cell_count;
            Point2D start = board->line[0], end = board->line[1];
            start.x = (start.x + 0.5) * fSize;
            start.y = (start.y + 0.5) * fSize;
            end.x = (end.x + 0.5) * fSize;
            end.y = (end.y + 0.5) * fSize;
            Render_lineWidth(4.0);
            Render_setColor(&COLOR_YELLOW);
            Render_drawLine(&start, &end);
            Render_lineWidth(RENDER_DEFAULT_LINE_WIDTH);
        }

        Render_clearOffset();
    }

    //render player info
    if(board->player1->events->render) board->player1->events->render(board->player1, evt);
    if(board->player2->events->render) board->player2->events->render(board->player2, evt);
}

static void mouseMoveEvt(void * obj, SceneData * scene, const Event_Mouse * evt) {
    GameBoard * board = (GameBoard*) obj;

    if(IN_RANGE(evt->x, board->position.x, board->position.x + board->size)) {
        if(IN_RANGE(evt->y, board->position.y, board->position.y + board->size)) {
            Event_Mouse board_evt = *evt;
            board_evt.x -= board->position.x;
            board_evt.y -= board->position.y;
            board_evt.sender = board;

            Cell * cell;
            for(unsigned int i = 0; i < board->cell_count * board->cell_count; ++i) {
                cell = &board->cells[i];
                if(cell->events->mouseMoveEvt) cell->events->mouseMoveEvt(cell, scene, &board_evt);
            }
        }
    }
}

static void mouseButtonEvt(void * obj, SceneData * scene, const Event_Mouse * evt) {
    GameBoard * board = (GameBoard*) obj;

    Player * player = board->firstPlayerOnTurn ? board->player1 : board->player2;

    if(player->ai == NULL || true) {
        //homan move
        if(IN_RANGE(evt->x, board->position.x, board->position.x + board->size)) {
            if(IN_RANGE(evt->y, board->position.y, board->position.y + board->size)) {
                if(evt->state == EVT_M_DOWN) {
                    float fSize = (float)board->size / (float)board->cell_count;
                    unsigned int x = (evt->x - board->position.x) / fSize;
                    unsigned int y = (evt->y - board->position.y) / fSize;
                    GameBoard_turn(board, x, y, board->firstPlayerOnTurn ? Symbol_X : Symbol_O);
                }
            }
        }
    }
}

static void update(void * obj, SceneData * scene, const Event_Update * evt) {
    GameBoard * board = (GameBoard*) obj;

    if(board->gameEnd) return;

    //time
    if(board->player1->events->update) board->player1->events->update(board->player1, scene, evt);
    if(board->player2->events->update) board->player2->events->update(board->player2, scene, evt);


    //AI move
    Player * player = board->firstPlayerOnTurn ? board->player1 : board->player2;
    if(player->ai != NULL) {
        AI_refeshGameData(player->ai, board->cells, board->cell_count,
                          board->firstPlayerOnTurn ? Symbol_X : Symbol_O);
        Node turn = AI_doTurn(player->ai);
        assert(turn.x != -1 && turn.y != -1);
        bool AI_turn = GameBoard_turn(board, turn.x, turn.y, player->ai->symbol);
        assert(AI_turn);
    }
}

static const E_Obj_Evts e_obj_evts = {
    .destruct = destruct,
    .render = render,
    .resize = NULL,
    .update = update,
    .mouseMoveEvt = mouseMoveEvt,
    .mouseButtonEvt = mouseButtonEvt,
    .pressKeyEvt = NULL,
    .releaseKeyEvt = NULL,
    .onLoad = NULL
};

GameBoard * GameBoard_create(int x, int y, size_t size, unsigned int count, GameEnd gameEndEvt) {
    if(count < 3) return NULL;

    GameBoard * board = malloc(sizeof(GameBoard));
    if(board == NULL) return NULL;

    board->events = &e_obj_evts;
    board->position.x = x;
    board->position.y = y;
    board->size = size;
    board->cell_count = count;
    board->firstPlayerOnTurn = true;
    board->line = malloc(sizeof(Point2D) * 2);
    board->line[0].x = -1;
    board->gameEnd = false;
    board->gameEndEvt = gameEndEvt;

    board->cells = malloc(sizeof(Cell) * count * count);
    if(board->cells == NULL) {
        free(board);
        return NULL;
    }

    float fSize = (float)size / (float)count;
    for(unsigned int x = 0; x < board->cell_count; ++x) {
        for(unsigned int y = 0; y < board->cell_count; ++y) {
            Cell_init(&board->cells[x + y * board->cell_count],
                    x * fSize, y * fSize, fSize);
        }
    }

    return board;
}

void GameBoard_destruct(GameBoard * board) {
    if(board != NULL) {
        if(board->cells) free(board->cells);
        if(board->line) free(board->line);
        free(board);
    }
}

static Cell * last_placed_cell;

bool GameBoard_turn(GameBoard * board, unsigned int x, unsigned int y, Symbol symbol) {
    if(board == NULL) return false;
    if(board->gameEnd) return false;
    if(x >= board->cell_count || y >= board->cell_count) return false;
    if(board->cells == NULL) return false;

    //place symbol
    if(last_placed_cell != NULL) {
        last_placed_cell->background = CELL_BG_COLOR;
    }
    Cell * cell = &board->cells[x + y * board->cell_count];
    last_placed_cell = cell;
    if(cell->symbol != Symbol_None) return false;
    cell->symbol = symbol;
    cell->background = CELL_BG_PLACE_COLOR;

    //check winner
    Point2D line[2];
    if(GameBoard_find5InLine(board, line)) {
        board->line[0] = line[0];
        board->line[1] = line[1];
        board->gameEnd = true;
        if(board->gameEndEvt) board->gameEndEvt(board->firstPlayerOnTurn ? board->player1 : board->player2);
    }

    //change player
    board->firstPlayerOnTurn = !board->firstPlayerOnTurn;
    if(board->firstPlayerOnTurn) {
        Player_activate(board->player1);
        Player_deactivate(board->player2);
    } else {
        Player_activate(board->player2);
        Player_deactivate(board->player1);
    }

    return true;
}

void GameBoard_clearGame(GameBoard * board) {
    if(board != NULL) {
        for(unsigned int i = 0; i < board->cell_count * board->cell_count; ++i) {
            board->cells[i].symbol = Symbol_None;
            board->cells[i].background = CELL_BG_COLOR;
        }
        board->gameEnd = false;
        board->firstPlayerOnTurn = true;
        board->line[0].x = -1;
        if(board->player1 != NULL && board->player2 != NULL) {
            Player_activate(board->player1);
            Player_deactivate(board->player2);
        }
    }
}

bool GameBoard_find5InLine(GameBoard * board, Point2D * start_end) {
    if(board == NULL || start_end == NULL) return false;

    unsigned int i;
    Symbol symbol;
    for(unsigned int x = 0; x < board->cell_count; ++x) {
        for(unsigned int y = 0; y < board->cell_count; ++y) {

            if(board->cells[x + y * board->cell_count].symbol == Symbol_None) {
                continue;
            }

            //horizontal
            if(x + 5 < board->cell_count) {
                symbol = board->cells[x + y * board->cell_count].symbol;
                for(i = 0; i < 5; ++i) {
                    if(symbol != board->cells[x + i + y * board->cell_count].symbol) {
                        break;
                    }
                }
                if(i == 5) {
                    start_end[0].y = start_end[1].y = y;
                    start_end[0].x = x;
                    start_end[1].x = x + 4;
                    return true;
                }
            }

            //vertical
            if(y + 5 < board->cell_count) {
                symbol = board->cells[x + y * board->cell_count].symbol;
                for(i = 0; i < 5; ++i) {
                    if(symbol != board->cells[x + (y + i) * board->cell_count].symbol) {
                        break;
                    }
                }
                if(i == 5) {
                    start_end[0].x = start_end[1].x = x;
                    start_end[0].y = y;
                    start_end[1].y = y + 4;
                    return true;
                }
            }

            //digonal 1
            if(y + 5 < board->cell_count && x + 5 < board->cell_count) {
                symbol = board->cells[x + y * board->cell_count].symbol;
                for(i = 0; i < 5; ++i) {
                    if(symbol != board->cells[x + i + (y + i) * board->cell_count].symbol) {
                        break;
                    }
                }
                if(i == 5) {
                    start_end[0].x = x;
                    start_end[0].y = y;
                    start_end[1].x = x + 4;
                    start_end[1].y = y + 4;
                    return true;
                }
            }

            //digonal 2
            if(y + 5 < board->cell_count && x - 5 >= 0) {
                symbol = board->cells[x + y * board->cell_count].symbol;
                for(i = 0; i < 5; ++i) {
                    if(symbol != board->cells[x - i + (y + i) * board->cell_count].symbol) {
                        break;
                    }
                }
                if(i == 5) {
                    start_end[0].x = x;
                    start_end[0].y = y;
                    start_end[1].x = x - 4;
                    start_end[1].y = y + 4;
                    return true;
                }
            }

        }
    }

    return false;
}

void GameBoard_setPlayers(GameBoard * board, Player * p1, Player * p2) {
    if(board != NULL && p1 != NULL && p2 != NULL) {
        board->player1 = p1;
        board->player2 = p2;
        if(board->firstPlayerOnTurn) {
            Player_activate(board->player1);
            Player_deactivate(board->player2);
        } else {
            Player_activate(board->player2);
            Player_deactivate(board->player1);
        }
    }
}

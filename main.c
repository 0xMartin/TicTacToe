/**
 * <------------------------------------------------------------------>
 * @name    TicTacToe
 * @author  Martin Krcma
 * @date    29. 4. 2021
 * <------------------------------------------------------------------>
 * @file    main.c
 * <------------------------------------------------------------------>
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "s3d/core.h"
#include "s3d/engine_object.h"
#include "s3d/ui/colors.h"
#include "s3d/ui/button.h"
#include "s3d/ui/label.h"
#include "s3d/ui/panel.h"
#include "s3d/ui/textfield.h"
#include "s3d/ui/checkbox.h"
#include "s3d/ui/image.h"

#include "obj/gameboard.h"
#include "obj/player.h"
#include "obj/ai.h"


static Texture * bgImage = NULL;
static SceneData * mainMenu = NULL;
static SceneData * game = NULL;



static GameBoard * board = NULL;
static Panel * panelGameOver = NULL;
static Label * labelGameOver = NULL;


static void timeOut(void * player) {
    panelGameOver->events.visible = true;
    labelGameOver->foreground = COLOR_RED;
    board->gameEnd = true;
    if(board->player1 == player)  {
        board->player2->score++;
    } else {
        board->player1->score++;
    }
    char buffer[256];
    snprintf(buffer, 256, "[%s] lost the game", ((Player*)player)->name);
    Label_changeText(labelGameOver, buffer);
}

static void gameEnd(void * winner) {
    panelGameOver->events.visible = true;
    labelGameOver->foreground = COLOR_GREEN;
    char buffer[256];
    snprintf(buffer, 256, "[%s] is winner", ((Player*)winner)->name);
    ((Player*)winner)->score++;
    Label_changeText(labelGameOver, buffer);
}

static void backToMenu(void * sender, const void * evt) {
    if(panelGameOver->events.focus) {
        panelGameOver->events.visible = false;
        CORE_setSceneData(mainMenu);
        GameBoard_clearGame(board);
    }
}

static void nextGame(void * sender, const void * evt) {
    if(panelGameOver->events.focus) {
        panelGameOver->events.visible = false;
        GameBoard_clearGame(board);
    }
}

static void initGame(size_t w, size_t h, size_t size, size_t count) {
    game = SceneData_create();
    assert(game != NULL);

    if(bgImage != NULL) {
        Image * img = Image_create(0, 0, w, h, bgImage);
        E_Obj_insertToList(game->gameData, (E_Obj*) img);
    }

    board = GameBoard_create((w - size)/2, (h - size) * 0.8, size, count, gameEnd);
    E_Obj_insertToList(game->gameData, (E_Obj*) board);

    Player * p1 = Player_create("Player1", 0, timeOut);
    Player * p2 = Player_create("Player2", 0, timeOut);

    p1->position.x = (w - size)/2;
    p2->position.x = (w - size)/2 + size - size/2;
    p1->position.y = p2->position.y = 10;
    p1->width = p2->width = size/2;
    GameBoard_setPlayers(board, p1, p2);

    panelGameOver = Panel_create((w - 360)/2, (h - 360)/2, 360, 300);
    panelGameOver->background = COLOR_CHANGE_OPACITY(UI_PANEL_BG_COLOR, 0.8);
    panelGameOver->events.visible = false;
    E_Obj_insertToList(game->gameData, (E_Obj*) panelGameOver);

    Label * label = Label_create(panelGameOver->width/2, 50, "Game Over");
    label->foreground = COLOR_WHITE;
    label->centered = true;
    Panel_insertChild(panelGameOver, (E_Obj*) label);

    labelGameOver = Label_create(panelGameOver->width/2, 100, NULL);
    labelGameOver->centered = true;
    Panel_insertChild(panelGameOver, (E_Obj*) labelGameOver);

    Button * btn = Button_create((panelGameOver->width - 250)/2, 160, 250, 50, "Back to menu");
    btn->background = COLOR_CHANGE_OPACITY(UI_BUTTON_BG_COLOR, 0.7);
    btn->events.mousePressAction = backToMenu;
    Panel_insertChild(panelGameOver, (E_Obj*) btn);

    btn = Button_create((panelGameOver->width - 250)/2, 220, 250, 50, "Next game");
    btn->background = COLOR_CHANGE_OPACITY(UI_BUTTON_BG_COLOR, 0.7);
    btn->events.mousePressAction = nextGame;
    Panel_insertChild(panelGameOver, (E_Obj*) btn);
}


static TextField * name1;
static TextField * name2;
static CheckBox * ai1;
static CheckBox * ai2;

#define AI_SEARCH_DEPTH 3

static void startGame(void * sender, const void * evt) {
    if(!Player_setName(board->player1, name1->text)) return;
    if(!Player_setName(board->player2, name2->text)) return;

    if(ai1->value) {
        AI * ai = AI_create(AI_SEARCH_DEPTH);
        Player_setAI(board->player1, ai);
    } else {
        if(board->player1->ai) AI_destruct(board->player1->ai);
        Player_setAI(board->player1, NULL);
    }

    if(ai2->value) {
        AI * ai = AI_create(AI_SEARCH_DEPTH);
        Player_setAI(board->player2, ai);
    } else {
        if(board->player2->ai) AI_destruct(board->player2->ai);
        Player_setAI(board->player2, NULL);
    }

    CORE_setSceneData(game);
}

static void initMainMenu(size_t w, size_t h) {
    mainMenu = SceneData_create();
    assert(mainMenu != NULL);

    if(bgImage != NULL) {
        Image * img = Image_create(0, 0, w, h, bgImage);
        E_Obj_insertToList(mainMenu->gameData, (E_Obj*) img);
    }

    Panel * panel = Panel_create((w - 500)/2, (h - 380)/2, 500, 380);
    panel->background = COLOR_CHANGE_OPACITY(panel->background, 0.95);
    E_Obj_insertToList(mainMenu->gameData, (E_Obj*) panel);

    Label * label = Label_create(30, 80, "Player 1 name:");
    Panel_insertChild(panel, (E_Obj*) label);
    label = Label_create(30, 160, "Player 2 name:");
    Panel_insertChild(panel, (E_Obj*) label);

    name1 = TextField_create(190, 50, 250, 40, 20);
    TextField_setText(name1, "Player1");
    Panel_insertChild(panel, (E_Obj*) name1);

    name2 = TextField_create(190, 130, 250, 40, 20);
    TextField_setText(name2, "Player2");
    Panel_insertChild(panel, (E_Obj*) name2);

    ai1 = CheckBox_create(30, 210, 20, false, "Player 1 AI");
    Panel_insertChild(panel, (E_Obj*) ai1);
    ai2 = CheckBox_create(30, 260, 20, false, "Player 2 AI");
    Panel_insertChild(panel, (E_Obj*) ai2);

    Button * start = Button_create(50, 310, panel->width - 100, 50, "Start game");
    start->events.mouseReleaseAction = startGame;
    Panel_insertChild(panel, (E_Obj*) start);
}

int main(int argc, char **argv) {
    CORE core = CORE_DEFAULT_CONFIG;
    sprintf(core.windonw_title, "%s", "TicTacToe");
    core.window_width = 800;
    core.window_height = 800;
    core.clearColor = UI_BG_COLOR;
    CORE_init(argc, argv, &core);

    CORE_loadTexture("data/img.bmp", &bgImage, true);
    initGame(core.window_width, core.window_height, 650, 20);
    initMainMenu(core.window_width, core.window_height);

    CORE_setSceneData(mainMenu);    
    CORE_run();

    return 0;
}

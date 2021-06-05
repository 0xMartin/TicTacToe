#include "player.h"

#include <stdlib.h>

#define BG_COLOR (Color){0.259, 0.267, 0.314, 0.600}
#define BORDER_COLOR (Color){0.800, 0.800, 0.800, 0.500}
#define STRING_COLOR (Color){0.900, 0.900, 0.900, 1.000}
#define STRING_ACTIVE_COLOR (Color){0.900, 0.900, 0.100, 1.000}

static void destruct(void * obj) {
    Player * p = (Player*) obj;
    Player_destruct(p);
}

static void render(void * obj, const Event_Render * evt) {
    Player * p = (Player*) obj;

    //outline
    Render_setColor(&BG_COLOR);
    Render_fillRectangle(&p->position, p->width, 70);
    Render_setColor(&BORDER_COLOR);
    Render_drawRectangle(&p->position, p->width, 70);

    //color
    if(p->active) {
        Render_setColor(&STRING_ACTIVE_COLOR);
    } else {
        Render_setColor(&STRING_COLOR);
    }

    //name
    Render_drawString(p->position.x + 5, p->position.y + Render_getStringHeight() + 5, p->name);

    //score
    static char buffer[255];
    sprintf(buffer, "Score: %d", p->score);
    Render_drawString(p->position.x + p->width - 90, p->position.y + Render_getStringHeight() + 5, buffer);

    //time
    Point2D pt = p->position;
    pt.x += 25;
    pt.y += 40;
    Render_drawRectangle(&pt, p->width - 50, 20);
    if(p->active) {
        float ratio = MAX(1.0 - p->time / PLAYER_TIME_PER_TURN, 0.0);
        Color c;
        c.red = 1.0 - ratio;
        c.green = ratio;
        c.blue = 0.0;
        c.alpha = 1.0;
        Render_setColor(&c);
        Render_fillRectangle(&pt, (p->width - 50) * ratio, 20);
    }
}

static void update(void * obj, SceneData * scene, const Event_Update * evt) {
    Player * p = (Player*) obj;
    if(p->active) {
        p->time += evt->ns_diff / 1e9;
        if(p->time > PLAYER_TIME_PER_TURN) {
            if(p->timeOut) p->timeOut(p);
        }
    }
}

static const E_Obj_Evts e_obj_evts = {
    .destruct = destruct,
    .render = render,
    .resize = NULL,
    .update = update,
    .mouseMoveEvt = NULL,
    .mouseButtonEvt = NULL,
    .pressKeyEvt = NULL,
    .releaseKeyEvt = NULL
};

Player * Player_create(const char * const name, unsigned int score, Player_timeOut timeOut) {
    Player * player = malloc(sizeof(Player));
    if(player == NULL) return NULL;

    player->events = &e_obj_evts;
    player->position.x = 0;
    player->position.y = 0;
    player->width = 0;
    player->score = score;
    player->active = false;
    player->ai = NULL;
    player->timeOut = timeOut;
    player->time = 0;
    player->name = malloc(sizeof(char) * PLAYER_NAME_MAX_LENGTH);
    if(name != NULL) {
        const char * c = name;
        unsigned int i;
        for(i = 0; i < PLAYER_NAME_MAX_LENGTH-1 && *c; ++i, ++c) {
            player->name[i] = *c;
        }
        player->name[i] = 0;
    } else {
        *player->name = 0;
    }

    return player;
}

void Player_destruct(Player * p) {
    if(p == NULL) return;

    if(p->name) free(p->name);
    if(p->ai) AI_destruct(p->ai);
    free(p);
}

bool Player_setName(Player * p, const char * const name) {
    if(name == NULL) return false;

    const char * c = name;
    unsigned int i;
    for(i = 0; i < PLAYER_NAME_MAX_LENGTH - 1 && *c; ++i, ++c) {
        p->name[i] = *c;
    }
    p->name[i] = 0;
    return true;
}

void Player_setAI(Player * p, AI * ai) {
    if(p != NULL) {
        p->ai = ai;
    }
}

void Player_activate(Player * p) {
    if(p != NULL) {
        p->active = true;
        p->time = 0.0;
    }
}

void Player_deactivate(Player * p) {
    if(p != NULL) {
        p->active = false;
        p->time = 0.0;
    }
}

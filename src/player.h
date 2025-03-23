#ifndef PLAYER_H
#define PLAYER_H

#include "geo.h"
#include "entities.h"

#include <stdio.h>
#include <stdlib.h>

typedef enum {PLAYER_HUNTER, PLAYER_PREY} PlayerType;

typedef struct Player Player;
struct Player
{
    float speed; //pas en global pour les futurs powerups
    unsigned int iD;
    Circle hitbox;
    float orientation;
    PlayerType type;
    float accel_coeff;
};

Player* p_player_create(PlayerType type, unsigned int iD, float speed, Circle *hitbox, unsigned long size);
int p_player_update_orientation(Player *player, Position *cursor);
void p_player_send_event_player_move(Player *player);
void p_player_move(Player *player, Position *cursor);

typedef struct PlayerHunter PlayerHunter;
struct PlayerHunter
{
    Player p;
    float paint_balls; //en float c'est plus facile pour la regen
    float paint_balls_max;
    float paint_per_s; //regen rate
};

void p_paint_regen(PlayerHunter *hunter);

PlayerHunter* p_player_hunter_create(unsigned int iD, float speed, Circle *hitbox, float paint_balls_max, float paint_per_sec);
void p_player_hunter_free(PlayerHunter *player);


typedef struct PlayerPrey PlayerPrey;
struct PlayerPrey
{
    Player p;
};

PlayerPrey* p_player_prey_create(unsigned int iD, float speed, Circle *hitbox);
void p_player_prey_free(PlayerPrey *player);

void p_player_paint_ball_shoot(Player *player);

#endif

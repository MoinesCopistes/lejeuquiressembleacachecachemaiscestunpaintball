#ifndef PLAYER_H
#define PLAYER_H

#include "geo.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct PlayerHunter PlayerHunter;
struct PlayerHunter
{
    float speed; //pas en global pour les futurs powerups
    unsigned int iD;
    Circle hitbox;
    float paint_balls; //en float c'est plus facile pour la regen
    float paint_balls_max;
    float paint_per_s; //regen rate
};

void p_paint_regen(PlayerHunter *hunter, float time);

typedef struct PlayerPrey PlayerPrey;
struct PlayerPrey
{
    float speed; //pas en global pour les futurs powerups
    unsigned int iD;
    Circle hitbox;
};

PlayerPrey* p_player_prey_create(unsigned int iD, float speed, Circle *hitbox);
void p_player_prey_free(PlayerPrey *player);
void p_player_prey_move(PlayerPrey *player, Position *cursor, float time);



#endif

#ifndef PLAYER_H
#define PLAYER_H

#include "geo.h"
#include <time.h>

typedef struct Player_hunter Player_hunter;
struct Player_hunter
{
    float speed; //pas en global pour les futurs powerups
    unsigned int iD;
    Circle hitbox;
    float paint_balls; //en float c'est plus facile pour la regen
    float paint_balls_max;
    float paint_per_s; //regen rate
};

void p_paint_regen(Player_hunter *hunter, clock_t time);

typedef struct Player_prey Player_prey;
struct Player_prey
{
    float speed; //pas en global pour les futurs powerups
    unsigned int iD;
    Circle hitbox;
};




#endif

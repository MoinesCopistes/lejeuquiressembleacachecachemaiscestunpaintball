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

float speed_pain_ball; //on stocke la vitesse de base d'une paint ball globalement
typedef struct Paint_ball Paint_ball;
struct Paint_ball
{
    float speed_x, speed_y; //on stocke dans la struct les vitesses horizontales et verticales (moins de calculs)
    Circle hitbox;
    float splash_radius;
    float dis_squared;
    float max_dis_squared;
    unsigned int player_id;
    unsigned int iD;
};

Paint_ball* p_paint_ball_create(Position *start, Position *cursor, unsigned int iD, unsigned int player_id, float speed_coeff, float radius, float splash_radius, float max_dis_squared);
void p_paint_ball_free(Paint_ball *ball); //au cas où il y a des pointeurs dedans plus tard
int p_paint_ball_update(Paint_ball *ball, clock_t time); //renvoie si la balle est "vivante" ou pas"

#endif

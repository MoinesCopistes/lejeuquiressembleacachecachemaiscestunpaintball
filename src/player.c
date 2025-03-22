#include "player.h"

void p_paint_regen(Player_hunter *hunter, clock_t time)
{
    hunter->paint_balls += hunter->paint_per_s * (time / CLOCKS_PER_SEC);
    if(hunter->paint_balls >= hunter->paint_balls_max)
        hunter->paint_balls = hunter->paint_balls_max;
}

Paint_ball* p_paint_ball_create(Position *start, Position *cursor, unsigned int iD, unsigned int player_id, float speed_coeff, float radius, float splash_radius, float max_dis_squared)
{
    Paint_ball *ball = (Paint_ball *) malloc(sizeof(Paint_ball));

    ball->iD = iD;
    ball->player_id = player_id;
    
    ball->hitbox.pos.x = start->x;
    ball->hitbox.pos.y = start->y;
    ball->hitbox.radius = radius;
    ball->splash_radius = splash_radius;

    ball->dis_squared = 0.0;
    ball->max_dis_squared = max_dis_squared;

    float normal = p_fast_inverse_sqrt((cursor->x - start->x) * (cursor->x - start->x) + (cursor->y - start->y) * (cursor->y - start->y));

    ball->speed_x = speed_pain_ball * speed_coeff * normal * (cursor->x - start->x);
    ball->speed_y = speed_pain_ball * speed_coeff * normal * (cursor->y - start->y);

    return ball;
}

void p_paint_ball_free(Paint_ball *ball)
{
    free(ball);
}

int p_paint_ball_update(Paint_ball *ball, clock_t time)
{
    float dx = ball->speed_x * (time / CLOCKS_PER_SEC);
    float dy = ball->speed_y * (time / CLOCKS_PER_SEC);
    ball->hitbox.pos.x += dx;
    ball->hitbox.pos.y += dy;
    ball->dis_squared += (dx * dx) + (dy * dy);    
    return ball->dis_squared < ball->max_dis_squared;
}

#include "player.h"

void p_paint_regen(Player_hunter *hunter, clock_t time)
{
    hunter->paint_balls += hunter->paint_per_s * (time / CLOCKS_PER_SEC);
    if(hunter->paint_balls >= hunter->paint_balls_max)
        hunter->paint_balls = hunter->paint_balls_max;
}



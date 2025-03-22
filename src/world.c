#include "defines.h"
#include "log.h"


void p_draw_player(float x, float y){
    //log_info("drawing player at pos %f, %f", x, y);
    DrawCircle(x, y, player_radius, player_color);
}

void p_draw_wall(float x, float y){
    //log_info("drawing wall at pos %f, %f", x, y);
    DrawRectangle(x, y, wall_size, wall_size, wall_color);
}


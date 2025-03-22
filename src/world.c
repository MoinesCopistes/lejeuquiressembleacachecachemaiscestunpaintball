#include "defines.h"
#include <stdbool.h>
#include "log.h"


bool p_draw_player(float x, float y){
    log_info("drawing player at pos %f, %f", x, y);
    DrawCircle(x, y, player_radius, player_color);
}



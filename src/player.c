#include "player.h"

void p_paint_regen(PlayerHunter *hunter, float time)
{
    hunter->paint_balls += hunter->paint_per_s * time;
    if(hunter->paint_balls >= hunter->paint_balls_max)
        hunter->paint_balls = hunter->paint_balls_max;
}

PlayerPrey* p_player_prey_create(unsigned int iD, float speed, Circle *hitbox)
{
    PlayerPrey *player = (PlayerPrey *) malloc(sizeof(PlayerPrey));
    player->iD = iD;
    player->speed = speed;
    player->hitbox.pos.x = hitbox->pos.x;
    player->hitbox.pos.y = hitbox->pos.y;
    player->hitbox.radius = hitbox->radius;
    return player;
}

void p_player_prey_free(PlayerPrey *player)
{
    free(player);
}

void p_player_prey_move(PlayerPrey *player, Position *cursor, float time)
{
    float normal = p_fast_inverse_sqrt((cursor->x - player->hitbox.pos.x) * (cursor->x - player->hitbox.pos.x) + (cursor->y - player->hitbox.pos.y) * (cursor->y - player->hitbox.pos.y));
    if(normal < 0.3)
    {
        player->hitbox.pos.x += player->speed * time * normal * (cursor->x - player->hitbox.pos.x);
        player->hitbox.pos.y += player->speed * time * normal * (cursor->y - player->hitbox.pos.y);
    }
}

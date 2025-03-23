#include "player.h"
#include "defines.h"
#include "networking.h"

Player* p_player_create(PlayerType type, unsigned int iD, float speed, Circle *hitbox, unsigned long size)
{
    Player *p = (Player *) malloc(size);
    p->speed = speed;
    p->iD = iD;
    p->hitbox.pos.x = hitbox->pos.x;
    p->hitbox.pos.y = hitbox->pos.y;
    p->hitbox.radius = hitbox->radius;
    p->type = type;
    p->accel_coeff = 0.2;
    p->orientation = 0.0;
    return p;
}

int p_player_update_orientation(Player *player, Position *cursor)
{
    float normal = p_fast_inverse_sqrt(
      (cursor->x - player->hitbox.pos.x) * (cursor->x - player->hitbox.pos.x) +
      (cursor->y - player->hitbox.pos.y) * (cursor->y - player->hitbox.pos.y));
    float new_orientation;
    if(normal * (cursor->y - player->hitbox.pos.y) > 0)
        new_orientation = acos(normal * (cursor->x - player->hitbox.pos.x)) * 57.2957; //radiants to degrees
    else
        new_orientation = 360.0 - (acos(normal * (cursor->x - player->hitbox.pos.x)) * 57.2957);
    //printf("%f\n",epm->orientation)  ;  */

    if(new_orientation == player->orientation)
        return 0;
    player->orientation = new_orientation;
    return 1;
}

void p_player_send_event_player_move(Player *player)
{
    EventPlayerMove *epm = (EventPlayerMove *)new_event(sizeof(EventPlayerMove),
                                                        EVENT_PLAYER_MOVE);
    epm->x = player->hitbox.pos.x;
    epm->y = player->hitbox.pos.y;
    epm->orientation = player->orientation;
    broadcast_event((Event *)epm, -1);
}

void p_player_move(Player *player, Position *cursor) { //peux pas modifier car je dois checker si le centre de ma hitbox n'est pas sur la souris
  float normal = p_fast_inverse_sqrt(
      (cursor->x - player->hitbox.pos.x) * (cursor->x - player->hitbox.pos.x) +
      (cursor->y - player->hitbox.pos.y) * (cursor->y - player->hitbox.pos.y));
  if (normal < 0.3) {
    player->hitbox.pos.x += player->speed * player->accel_coeff * dt * normal * (cursor->x - player->hitbox.pos.x);
    player->hitbox.pos.y += player->speed * player->accel_coeff * dt * normal * (cursor->y - player->hitbox.pos.y);
    player->accel_coeff += 0.1;
    if (player->accel_coeff > 1.0)
      player->accel_coeff = 1.0;
  }
  p_player_update_orientation(player,cursor);
  p_player_send_event_player_move(player);
}

void p_paint_regen(PlayerHunter *hunter) {
  hunter->paint_balls += hunter->paint_per_s * dt;
  if (hunter->paint_balls >= hunter->paint_balls_max)
    hunter->paint_balls = hunter->paint_balls_max;
}

PlayerHunter *p_player_hunter_create(unsigned int iD, float speed,
                                     Circle *hitbox, float paint_balls_max,
                                     float paint_per_sec) {
  PlayerHunter *player = (PlayerHunter *) p_player_create(PLAYER_HUNTER,iD,speed,hitbox,sizeof(PlayerHunter));
  player->paint_balls = paint_balls_max;
  player->paint_balls_max = paint_balls_max;
  player->paint_per_s = paint_per_sec;
  return player;
}

void p_player_hunter_free(PlayerHunter *player) { free(player); }

PlayerPrey *p_player_prey_create(unsigned int iD, float speed, Circle *hitbox) {
  PlayerPrey *player = (PlayerPrey *) p_player_create(PLAYER_PREY, iD, speed, hitbox, sizeof(PlayerPrey));
  return player;
}

void p_player_prey_free(PlayerPrey *player) { free(player); }

void p_player_paint_ball_shoot(Player *player)
{
    
    if(p_entity_tab_is_full())
    {
        printf("OBJECT LIMIT REACHED.\nNUKING.\n");
        exit(EXIT_FAILURE);
    }       
    
    Paint_ball* ball = p_paint_ball_create(&(player->hitbox.pos), player->orientation, player->iD, 1.0, 30,30, 90000.0);
    p_entity_tab_add((Entity*) ball);

    EventPlayerShootPaintBall *epm = (EventPlayerShootPaintBall *)new_event(sizeof(EventPlayerShootPaintBall),EVENT_PLAYER_SHOOT_PAINT_BALL);
    epm->start = player->hitbox.pos;
    epm->orientation = player->orientation;
    epm->player_id = player->iD;
    epm->speed_coeff = 1.0;
    epm->radius = 30;
    epm->splash_radius = 30;
    epm->max_dis_squared = 90000.0;

    broadcast_event((Event *)epm, -1);
}

#include "defines.h"
#include "networking.h"
void p_paint_regen(PlayerHunter *hunter) {
  hunter->paint_balls += hunter->paint_per_s * dt;
  if (hunter->paint_balls >= hunter->paint_balls_max)
    hunter->paint_balls = hunter->paint_balls_max;
}

PlayerHunter *p_player_hunter_create(unsigned int iD, float speed,
                                     Circle *hitbox, float paint_balls_max,
                                     float paint_per_sec) {
  PlayerHunter *player = (PlayerHunter *)malloc(sizeof(PlayerHunter));
  player->iD = iD;
  player->speed = speed;
  player->hitbox.pos.x = hitbox->pos.x;
  player->hitbox.pos.y = hitbox->pos.y;
  player->hitbox.radius = hitbox->radius;
  player->accel_coeff = 0.2;
  player->paint_balls = paint_balls_max;
  player->paint_balls_max = paint_balls_max;
  player->paint_per_s = paint_per_sec;
  return player;
}

void p_player_hunter_free(PlayerHunter *player) { free(player); }

void p_player_hunter_move(PlayerHunter *player, Position *cursor) {
  float normal = p_fast_inverse_sqrt(
      (cursor->x - player->hitbox.pos.x) * (cursor->x - player->hitbox.pos.x) +
      (cursor->y - player->hitbox.pos.y) * (cursor->y - player->hitbox.pos.y));
  if (normal < 0.3) {
    player->hitbox.pos.x += player->speed * player->accel_coeff * dt * normal *
                            (cursor->x - player->hitbox.pos.x);
    player->hitbox.pos.y += player->speed * player->accel_coeff * dt * normal *
                            (cursor->y - player->hitbox.pos.y);
    player->accel_coeff += 0.1;
    if (player->accel_coeff > 1.0)
      player->accel_coeff = 1.0;
  }
}

PlayerPrey *p_player_prey_create(unsigned int iD, float speed, Circle *hitbox) {
  PlayerPrey *player = (PlayerPrey *)malloc(sizeof(PlayerPrey));
  player->iD = iD;
  player->speed = speed;
  player->hitbox.pos.x = hitbox->pos.x;
  player->hitbox.pos.y = hitbox->pos.y;
  player->hitbox.radius = hitbox->radius;
  player->accel_coeff = 0.2;
  return player;
}

void p_player_prey_free(PlayerPrey *player) { free(player); }

void p_player_prey_move(PlayerPrey *player, Position *cursor) {
  float normal = p_fast_inverse_sqrt(
      (cursor->x - player->hitbox.pos.x) * (cursor->x - player->hitbox.pos.x) +
      (cursor->y - player->hitbox.pos.y) * (cursor->y - player->hitbox.pos.y));
  if (normal < 0.3) {
    player->hitbox.pos.x += player->speed * player->accel_coeff * dt * normal *
                            (cursor->x - player->hitbox.pos.x);
    player->hitbox.pos.y += player->speed * player->accel_coeff * dt * normal *
                            (cursor->y - player->hitbox.pos.y);
    player->accel_coeff += 0.1;
    if (player->accel_coeff > 1.0)
      player->accel_coeff = 1.0;

    EventPlayerMove *epm = (EventPlayerMove *)new_event(sizeof(EventPlayerMove),
                                                        EVENT_PLAYER_MOVE);
    epm->x = player->hitbox.pos.x;
    epm->y = player->hitbox.pos.y;
    broadcast_event((Event *)epm, -1);
  }
}

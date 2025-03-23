#include "player.h"
#include "defines.h"
#include "networking.h"
#include <stdio.h>

Player *p_player_create(PlayerType type, unsigned int iD, float speed,
                        Circle *hitbox, unsigned long size) {
  Player *p = (Player *)malloc(size);
  p->speed = speed;
  p->iD = iD;
  p->hitbox.pos.x = hitbox->pos.x;
  p->hitbox.pos.y = hitbox->pos.y;
  p->hitbox.radius = hitbox->radius;
  p->type = type;
  p->accel_coeff = 0.2;
  return p;
}


#include <math.h>

#define LERP_FACTOR 0.05f // Adjust for desired smoothness
#define LERP_THRESHOLD 0.5f // Rounding threshold to prevent flickering

void p_camera_follow() {
    Vector2 *offset = &world.offset;
    Rectangle rect = {
        offset->x + CAMERA_BOUNDARIES, offset->y + CAMERA_BOUNDARIES,
        screen_x - 2 * CAMERA_BOUNDARIES, screen_y - 2 * CAMERA_BOUNDARIES
    };

    Player *p = world.players[world.playerID];

    float target_x = offset->x;
    float target_y = offset->y;

    if (p->hitbox.pos.x > rect.x + rect.width) {
        target_x += (p->hitbox.pos.x - (rect.x + rect.width));
    }
    if (p->hitbox.pos.y > rect.y + rect.height) {
        target_y += (p->hitbox.pos.y - (rect.y + rect.height));
    }
    if (p->hitbox.pos.x < rect.x) {
        target_x += (p->hitbox.pos.x - rect.x);
    }
    if (p->hitbox.pos.y < rect.y) {
        target_y += (p->hitbox.pos.y - rect.y);
    }

    // Apply lerping
    offset->x += roundf((target_x - offset->x) * LERP_FACTOR);
    offset->y += roundf((target_y - offset->y) * LERP_FACTOR);

    // Snap to target if close enough to avoid flickering
    if (fabs(target_x - offset->x) < LERP_THRESHOLD) {
        offset->x = target_x;
    }
    if (fabs(target_y - offset->y) < LERP_THRESHOLD) {
        offset->y = target_y;
    }
}




void p_player_move(Player *player, Position *cursor) {
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

void p_paint_regen(PlayerHunter *hunter) {
  hunter->paint_balls += hunter->paint_per_s * dt;
  if (hunter->paint_balls >= hunter->paint_balls_max)
    hunter->paint_balls = hunter->paint_balls_max;
}

PlayerHunter *p_player_hunter_create(unsigned int iD, float speed,
                                     Circle *hitbox, float paint_balls_max,
                                     float paint_per_sec) {
  PlayerHunter *player = (PlayerHunter *)p_player_create(
      PLAYER_HUNTER, iD, speed, hitbox, sizeof(PlayerHunter));
  player->paint_balls = paint_balls_max;
  player->paint_balls_max = paint_balls_max;
  player->paint_per_s = paint_per_sec;
  return player;
}

void p_player_hunter_free(PlayerHunter *player) { free(player); }

PlayerPrey *p_player_prey_create(unsigned int iD, float speed, Circle *hitbox) {
  PlayerPrey *player = (PlayerPrey *)p_player_create(
      PLAYER_PREY, iD, speed, hitbox, sizeof(PlayerPrey));
  return player;
}

void p_player_prey_free(PlayerPrey *player) { free(player); }

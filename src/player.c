#include "player.h"
#include "defines.h"
#include "networking.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#define LERP_FACTOR 0.05f   // Adjust for desired smoothness
#define LERP_THRESHOLD 0.5f // Rounding threshold to prevent flickering

double last_event_time = 0;

void p_camera_follow() {
  Vector2 *offset = &world.offset;
  Rectangle rect = {
      offset->x + CAMERA_BOUNDARIES, offset->y + CAMERA_BOUNDARIES,
      screen_x - 2 * CAMERA_BOUNDARIES, screen_y - 2 * CAMERA_BOUNDARIES};

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
  p->orientation = 0.0;
  p->alive = 1;
  p->tagged = 0;
  p->timer = 0.0;
  return p;
}

int p_player_update_orientation(Player *player, Position *cursor) {
  float normal = p_fast_inverse_sqrt(
      (cursor->x - player->hitbox.pos.x) * (cursor->x - player->hitbox.pos.x) +
      (cursor->y - player->hitbox.pos.y) * (cursor->y - player->hitbox.pos.y));
  float new_orientation;
  if (normal * (cursor->y - player->hitbox.pos.y) > 0)
    new_orientation = acos(normal * (cursor->x - player->hitbox.pos.x)) *
                      57.2957; // radiants to degrees
  else
    new_orientation =
        360.0 - (acos(normal * (cursor->x - player->hitbox.pos.x)) * 57.2957);
  // printf("%f\n",epm->orientation)  ;  */

  if (new_orientation == player->orientation)
    return 0;
  player->orientation = new_orientation;
  return 1;
}

void p_player_send_event_player_move(Player *player) {
  double current_time = GetTime();
  double dt = current_time - last_event_time;
  if (dt >= 0.1) {
    last_event_time = current_time;
    EventPlayerMove *epm = (EventPlayerMove *)new_event(sizeof(EventPlayerMove),
                                                        EVENT_PLAYER_MOVE);
    epm->x = player->hitbox.pos.x;
    epm->y = player->hitbox.pos.y;
    epm->orientation = player->orientation;
    broadcast_event((Event *)epm, -1);
  }
}

void p_player_move(Player *player, Position *cursor, Map *map) {
  float normal = p_fast_inverse_sqrt(
      (cursor->x - player->hitbox.pos.x) * (cursor->x - player->hitbox.pos.x) +
      (cursor->y - player->hitbox.pos.y) * (cursor->y - player->hitbox.pos.y));
  if (normal < 0.3) {
    float added_dist_x;
    float added_dist_y;

    added_dist_x = player->speed * player->accel_coeff * dt * normal *
                   (cursor->x - player->hitbox.pos.x);
    added_dist_y = player->speed * player->accel_coeff * dt * normal *
                   (cursor->y - player->hitbox.pos.y);

    int next_tile_x_index_i = (player->hitbox.pos.x + added_dist_x) / tile_size;
    int next_tile_x_index_j = player->hitbox.pos.y / tile_size;
    int next_tile_y_index_i = player->hitbox.pos.x / tile_size;
    int next_tile_y_index_j = (player->hitbox.pos.y + added_dist_y) / tile_size;

    if (map->tiles[next_tile_x_index_j][next_tile_x_index_i].id == ' ') {
      player->hitbox.pos.x += added_dist_x;
    }
    if (map->tiles[next_tile_y_index_j][next_tile_y_index_i].id == ' ') {
      player->hitbox.pos.y += added_dist_y;
    }

    player->accel_coeff += 0.1;
    if (player->accel_coeff > 1.0)
      player->accel_coeff = 1.0;

    p_player_update_orientation(player, cursor);
    p_player_send_event_player_move(player);
  }
}

void p_player_update_tagged() {
  for (unsigned int i = 0; i < 4; ++i) {
    if (world.players[i] != NULL) {
      if (world.players[i]->tagged == 1) {
        world.players[i]->timer += dt;
        if (world.players[i]->timer >= 10.0) {
          world.players[i]->tagged = 0;
          world.players[i]->timer = 0.0;
        }
      }
    }
  }
}


void p_update_players() {
  for (int i = 0; i < world.playersNumber; i++) {
    if (i == world.playerID) continue;
    Player* p = world.players[i];
    p->hitbox.pos.y += (p->objectiveY - p->hitbox.pos.y) * 0.20;
    if (fabs(p->hitbox.pos.y - p->objectiveY) < 0.5) {
      p->hitbox.pos.y = p->objectiveY;
    }
    p->hitbox.pos.x += (p->objectiveX - p->hitbox.pos.x) * 0.20;
    if (fabs(p->hitbox.pos.x - p->objectiveX) < 0.5) {
      p->hitbox.pos.x = p->objectiveX;
    }
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

void p_player_paint_ball_shoot(Player *player) {

  if (player->type == PLAYER_PREY) {
    return;
  }
  if (p_entity_tab_is_full()) {
    printf("OBJECT LIMIT REACHED.\nNUKING.\n");
    exit(EXIT_FAILURE);
  }
  PlayerHunter* ph = ((PlayerHunter*)world.players[world.hunterID]);
  if (ph->paint_balls < 1) {
    return;
  }
  Paint_ball *ball =
      p_paint_ball_create(&(player->hitbox.pos), player->orientation,
                          player->iD, 1.0, 30, 30, 90000.0);
  p_entity_tab_add((Entity *)ball);

  EventPlayerShootPaintBall *epm = (EventPlayerShootPaintBall *)new_event(
      sizeof(EventPlayerShootPaintBall), EVENT_PLAYER_SHOOT_PAINT_BALL);
  epm->start = player->hitbox.pos;
  epm->orientation = player->orientation;
  epm->player_id = player->iD;
  epm->speed_coeff = 1.0;
  epm->radius = 30;
  epm->splash_radius = 100;
  epm->max_dis_squared = 90000.0;

  broadcast_event((Event *)epm, -1);
  ph->paint_balls --;
}

void p_stab_calculate_broadcast(int iD) {
  Circle stab_range;
  stab_range.pos.x = world.players[iD]->hitbox.pos.x;
  stab_range.pos.y = world.players[iD]->hitbox.pos.y;
  stab_range.radius = world.players[iD]->hitbox.radius * 1.5;
  for (unsigned int i = 0; i < 4; i++) {
    if (i != iD && world.players[i] != NULL) {
      if (p_circle_is_in_circle(&stab_range, &(world.players[i]->hitbox))) {
        float normal = p_fast_inverse_sqrt(
            (world.players[i]->hitbox.pos.x - world.players[iD]->hitbox.pos.x) *
                (world.players[i]->hitbox.pos.x -
                 world.players[iD]->hitbox.pos.x) +
            (world.players[i]->hitbox.pos.y - world.players[iD]->hitbox.pos.y) *
                (world.players[i]->hitbox.pos.y -
                 world.players[iD]->hitbox.pos.y)); // rofl
        float cosine =
            (world.players[i]->hitbox.pos.x - world.players[iD]->hitbox.pos.x) *
            normal;
        float sine =
            (world.players[i]->hitbox.pos.y - world.players[iD]->hitbox.pos.y) *
            normal;
        float orientation;
        if (sine > 0)
          orientation = acos(cosine) * 57.2957;
        else
          orientation = 360.0 - (acos(cosine) * 57.2957);

        if (cos((orientation - world.players[iD]->orientation) / 57.2957) >
            0.7071) {
          EventKillPlayer *ekp = (EventKillPlayer *)new_event(
              sizeof(EventKillPlayer), EVENT_KILL_PLAYER);
          ekp->victim_iD = i;
          broadcast_event((Event *)ekp, -1);
          world.players[i]->alive = 0;
        }
      }
    }
  }
}

void p_player_stab(Player *player) {
  if (isServer)
    p_stab_calculate_broadcast(player->iD);
  else {
    EventStab *es = (EventStab *)new_event(sizeof(EventStab), EVENT_STAB);
    es->stabber_id = player->iD;
    broadcast_event((Event *)es, -1);
  }
}

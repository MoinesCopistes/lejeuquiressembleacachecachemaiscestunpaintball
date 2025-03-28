#include "entities.h"
#include "defines.h"
#include "networking.h"
#include "raylib.h"
#include <stdio.h>

Entity *p_entity_create(EntityType type, unsigned int iD, unsigned long size) {
  Entity *e = (Entity *)malloc(size);
  e->type = type;
  e->alive = 1;
  e->iD = iD;
  return e;
}

void (*EntityUpdateFunctions[ENTITY_NUMBER])(Entity *entity) = {
    p_paint_ball_update};

void (*EntityFreeFunctions[ENTITY_NUMBER])(Entity *entity) = {
    p_paint_ball_free};

// Different entities

unsigned int balls_shot = 0;

Paint_ball *p_paint_ball_create(Position *start, float orientation,
                                unsigned int player_id, float speed_coeff,
                                float radius, float splash_radius,
                                float max_dis_squared) {

  Paint_ball *ball = (Paint_ball *)p_entity_create(
      ENTITY_PAINT_BALL, balls_shot, sizeof(Paint_ball));
  balls_shot++;
  ball->player_id = player_id;

  ball->hitbox.pos.x = start->x;
  ball->hitbox.pos.y = start->y;
  ball->hitbox.radius = radius;
  ball->splash_radius = splash_radius;

  ball->dis_squared = 0.0;
  ball->max_dis_squared = max_dis_squared;

  ball->speed_x = SPEED_PAINT_BALL * speed_coeff * cos(orientation / 57.2957);
  ball->speed_y = SPEED_PAINT_BALL * speed_coeff * sin(orientation / 57.2957);

  return ball;
}

void p_paint_ball_free(Entity *entity) {
  Paint_ball *ball = (Paint_ball *)entity;
  free(ball);
}

void p_paint_ball_update(Entity *entity) {
  Paint_ball *ball = (Paint_ball *)entity;
  float dx = ball->speed_x * dt;
  float dy = ball->speed_y * dt;
  ball->hitbox.pos.x += dx;
  ball->hitbox.pos.y += dy;
  ball->dis_squared += (dx * dx) + (dy * dy);
  if (isServer) {
    // max distance
    bool splashed = ball->dis_squared > ball->max_dis_squared;

    // players
    if (!splashed) {
      for (unsigned int i = 0; i < 4; ++i) {
        if (i != ball->player_id && world.players[i] != NULL) {
          if (p_circle_is_in_circle(&(ball->hitbox),
                                    &(world.players[i]->hitbox))) {
            splashed = true;
            break;
          }
        }
      }
    }
    // walls
    if (!splashed) {
      int row = (int)(ball->hitbox.pos.y / tile_size);
      int col = (int)(ball->hitbox.pos.x / tile_size);
      if (world.map->tiles[row][col].id != ' ') {
        splashed = true;
      }
    }

    if (splashed) {
      // kill
      ball->e.alive = 0;
      EventKillEntity *epm = (EventKillEntity *)new_event(
          sizeof(EventKillEntity), EVENT_KILL_ENTITY);
      epm->iD = ball->e.iD;
      broadcast_event((Event *)epm, -1);

      // tag
      Circle fallout;
      fallout.pos.x = ball->hitbox.pos.x;
      fallout.pos.y = ball->hitbox.pos.y;
      fallout.radius = ball->splash_radius;

      for (unsigned int i = 0; i < 4; ++i) {
        if (i != ball->player_id && world.players[i] != NULL) {
          if (p_circle_is_in_circle(&fallout, &(world.players[i]->hitbox))) {
            // update myself
            world.players[i]->tagged = 1;
            // send event to update others
            EventTagPlayer *etp = (EventTagPlayer *)new_event(
                sizeof(EventTagPlayer), EVENT_TAG_PLAYER);
            etp->tagged_iD = i;
            broadcast_event((Event *)etp, -1);
          }
        }
      }
    }
  }
}

// Entity tab

Entity *EntityTab[OBJECT_LIMIT];

void p_entity_tab_init() {
  for (unsigned int i = 0; i < OBJECT_LIMIT; ++i)
    EntityTab[i] = NULL;
}

void p_entity_all_tab_free() {
  for (unsigned int i = 0; i < OBJECT_LIMIT; ++i) {
    if (EntityTab[i] != NULL) {
      EntityFreeFunctions[EntityTab[i]->type](EntityTab[i]);
      EntityTab[i] = NULL;
    }
  }
}

void p_entity_tab_update() {
  for (unsigned int i = 0; i < OBJECT_LIMIT; ++i) {
    if (EntityTab[i] != NULL)
      EntityUpdateFunctions[EntityTab[i]->type](EntityTab[i]);
  }
}

int p_entity_tab_is_full() {
  for (unsigned int i = 0; i < OBJECT_LIMIT; ++i) {
    if (EntityTab[i] == NULL)
      return 0;
  }
  return 1;
}

unsigned int
p_entity_tab_size() // renvoie combien d'éléments sont dans le tableau
{
  unsigned int counter = 0;
  for (unsigned int i = 0; i < OBJECT_LIMIT; ++i) {
    if (EntityTab[i] != NULL)
      counter++;
  }
  return counter;
}

int p_entity_tab_add(Entity *entity) {
  for (unsigned int i = 0; i < OBJECT_LIMIT; ++i) {
    if (EntityTab[i] == NULL) {
      EntityTab[i] = entity;
      return 1;
    }
  }
  return 0;
}

void p_entity_tab_dead_free() {
  for (unsigned int i = 0; i < OBJECT_LIMIT; ++i) {
    if (EntityTab[i] != NULL) {
      if (EntityTab[i]->alive == 0) {
        EntityFreeFunctions[EntityTab[i]->type](EntityTab[i]);
        EntityTab[i] = NULL;
      }
    }
  }
}


void p_draw_paintball(Texture2D* texture, Rectangle destination, Color color) {
        Rectangle source = {0, 0, 160, 160};
        Vector2 v = {0, 0};
        DrawTexturePro(*texture, source, destination, v, 0, color);
  
}

void p_entity_tab_draw_paint_balls(Texture2D *texture) {
  for (unsigned int i = 0; i < OBJECT_LIMIT; ++i) {
    if (EntityTab[i] != NULL) {
      if (EntityTab[i]->type == ENTITY_PAINT_BALL) {
        Paint_ball *ball = (Paint_ball *)EntityTab[i];
        Rectangle destination = {(int)ball->hitbox.pos.x - world.offset.x - PROJECTILE_SIZE/2,
                                 (int)ball->hitbox.pos.y - world.offset.y - PROJECTILE_SIZE/2,
                                 PROJECTILE_SIZE, PROJECTILE_SIZE};
        p_draw_paintball(texture,  destination, WHITE);
      }
    }
  }
}

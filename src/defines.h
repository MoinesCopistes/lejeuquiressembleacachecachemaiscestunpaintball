#pragma once
#include "entities.h"
#include "player.h"
#include "raylib.h"

#define player_radius 10
#define player_color RED
#define wall_size 20
#define wall_color DARKGRAY
#define texture_size 16
#define texture_scale 4.0
#define tile_size texture_scale *texture_size
#define map_size_x 20
#define map_size_y 12
#define screen_x map_size_x *tile_size
#define screen_y map_size_y *tile_size
#define MAX_CLIENT_NUMBER 4

#define MACROVAR(name) CONCAT(name, __LINE__)
#define SCOPE(...)                                                             \
  for (int MACROVAR(_i_) = 0; !MACROVAR(_i_); ++MACROVAR(_i_), __VA_ARGS__)

enum game_states {
  IN_MENU,
  IN_SETTINGS,
  IN_GAME,
};

extern const int screenWidth;
extern const int screenHeight;
extern enum game_states game_state;

extern float dt;
typedef struct {
  Player *players[4];
  Entity *entities[512];
  int playerID;
  int playersNumber;
  Vector2 player_spawn_point;
} World;
extern World world;

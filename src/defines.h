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
#define tile_size (texture_scale * texture_size)
#define map_size_x 20
#define map_size_y 12
#define screen_x (map_size_x * tile_size)
#define screen_y (map_size_y * tile_size)
#define blank_chars "xwrdf"
#define wall_chars "e"
#define MAX_CLIENT_NUMBER 4
#define CAMERA_BOUNDARIES 400

#define MACROVAR(name) CONCAT(name, __LINE__)
#define SCOPE(...)                                                             \
  for (int MACROVAR(_i_) = 0; !MACROVAR(_i_); ++MACROVAR(_i_), __VA_ARGS__)

enum game_states { IN_MENU, IN_CLIENT, IN_GAME, IN_LOBBY };

enum music_states { MUSIC_CALM, MUSIC_PURSUIT};

extern const int screenWidth;
extern const int screenHeight;
extern enum game_states game_state;
extern char menuError[256];
extern float dt;
typedef struct {
  Player *players[4];
  Entity *entities[512];
  int playerID;
  int playersNumber;
  char *serverAddress;
  char *serverPort;
  Vector2 offset;
  Map* map;
} World;
extern World world;
extern Map *map;

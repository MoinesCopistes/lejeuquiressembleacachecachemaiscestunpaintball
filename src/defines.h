#pragma once
#include "player.h"
#include "raylib.h"

#define player_radius 10
#define player_color RED
#define wall_size 20
#define wall_color DARKGRAY
#define MAX_CLIENT_NUMBER 4

#define MACROVAR(name) CONCAT(name, __LINE__)
#define SCOPE(...)                                                             \
  for (int MACROVAR(_i_) = 0; !MACROVAR(_i_); ++MACROVAR(_i_), __VA_ARGS__)

extern PlayerPrey *players[4];

typedef struct {
  float x, y;
} Player;

typedef struct {
  Player *players;
} World;

enum game_states {
  IN_MENU,
  IN_SETTINGS,
  IN_GAME,
};

extern const int screenWidth;
extern const int screenHeight;
extern enum game_states game_state;
extern float dt;

#pragma once
#include "raylib.h"

#define player_radius 10
#define player_color RED
#define wall_size 20
#define wall_color DARKGRAY








#define MACROVAR(name)      CONCAT(name, __LINE__)
#define SCOPE(...)          for(int MACROVAR(_i_) = 0; !MACROVAR(_i_); ++MACROVAR(_i_), __VA_ARGS__)


typedef struct {
    float x, y;
} Player;

typedef struct {
    Player* players;
} World;


typedef struct {
    Texture2D sprite;
} Tile;

typedef struct {
    Tile* tiles;
} Map;
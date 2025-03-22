#pragma once

#define MACROVAR(name) CONCAT(name, __LINE__)
#define SCOPE(...)                                                             \
  for (int MACROVAR(_i_) = 0; !MACROVAR(_i_); ++MACROVAR(_i_), __VA_ARGS__)

typedef struct {
  float x, y;
} Player;

typedef struct {
  Player *players;
} World;

extern const int screenWidth;
extern const int screenHeight;

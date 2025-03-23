#pragma once
#include "defines.h"

void _draw_tile(Tile tile, TileSet* tileset, Vector2 offset);
Rectangle _init_tile_rect(char id);
Tile* _init_tile(char id, Coordinate pos);
TileSet* _init_tileset();
Tile** _get_tile_grid(int n_col, int n_row, const char grid[n_row][n_col], TileSet* tileset);
void p_draw_map(Map* map);
Map* p_load_map(const char* path);
void p_free_map(Map* map);

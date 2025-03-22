#pragma once
#include "defines.h"

Image _init_tile(const char* path, int x, int y, int deg);
Texture2D p_assemble_atlas(Map* map);
Image _get_image(char input);
Tile** _get_tile_grid(int n_col, int n_row, const char grid[n_row][n_col]);
Map* p_load_map(const char* path);
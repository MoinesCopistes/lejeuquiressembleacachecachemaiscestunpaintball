#include <stdlib.h>
#include "defines.h"
#include <string.h>
#include <stdio.h>
#include "log.h"

#define max(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })

int p_random_int(int min, int max) {
    return min + rand() % (max - min + 1);
}

void _draw_tile(Tile tile, TileSet* tileset){
    DrawTexturePro(tileset->texture, tile.rect, (Rectangle){tile.pos.x * tile_size, tile.pos.y * tile_size, tile_size, tile_size}, (Vector2){0,0}, 0, WHITE);
}

Rectangle _init_tile_rect(char id) {
    int x, y = 0;
    switch (id) {
        case ' ':
            x = p_random_int(6, 9);
            y = p_random_int(0, 2);
            return (Rectangle){x * 16, y * 16, texture_size, texture_size};
        case '1':
            return (Rectangle){0, 16, texture_size, texture_size};
        case '2':
            x = p_random_int(1, 4);
            return (Rectangle){x *16, 0, texture_size, texture_size};
        case '3':
            return (Rectangle){5 * 16, 0, texture_size, texture_size};
        case '4':
            y = p_random_int(1, 3);
            return (Rectangle){0, y * 16, texture_size, texture_size};
        case '5':
            return (Rectangle){8 * 16, 7 * 16, texture_size, texture_size};
        case '6':
            y = p_random_int(1, 3);
            return (Rectangle){5 * 16, y * 16, texture_size, texture_size};
        case '7':
            return (Rectangle){0, 4 * 16, texture_size, texture_size};
        case '8':
            x = p_random_int(1, 4);
            return (Rectangle){x * 16, 4 * 16, texture_size, texture_size};
        case '9':
            return (Rectangle){5 * 16, 4 * 16, texture_size, texture_size};
        case 'a':
            return (Rectangle){2 * 16, 0, texture_size, texture_size};
        case 'z':
            return (Rectangle){2 * 16, 0, texture_size, texture_size};
        case 'q':
            return (Rectangle){3 * 16, 5 * 16, texture_size, texture_size};
        case 's':
            return (Rectangle){0, 5 * 16, texture_size, texture_size};
        default:
            log_error("Unknown tile character: %c", id);
            return (Rectangle){0, 0, tile_size, tile_size};  // Default rectangle
    }
}
Tile _init_tile(char id, Coordinate pos) {
    Tile* tile = malloc(sizeof(Tile));
    tile->id = id;
    tile->pos = pos;
    tile->rect = _init_tile_rect(id);
    return *tile;
}

TileSet* _init_tileset(){
    const char* ids = " 123456789azqs";
    const char* path = "2D Pixel Dungeon Asset Pack v2.0/2D Pixel Dungeon Asset Pack/character and tileset/Dungeon_Tileset.png";
    int n_image = strlen(ids);
    TileSet* tileset = malloc(sizeof(TileSet));
    tileset->ids = ids;
    tileset->n_image = n_image;
    Image img = LoadImage(path);
    Texture2D texture = LoadTextureFromImage(img);
    UnloadImage(img);
    tileset->texture = texture;
    return tileset;
}



Tile** _get_tile_grid(int n_col, int n_row, const char grid[n_row][n_col], TileSet* tileset){
    Tile** tiles = (Tile**) malloc(sizeof(Tile*) * n_row);
    if (tiles == NULL){log_error("Couldn't malloc Tile** in map.c");}
    for (int i = 0 ; i < n_row ; i++){
        tiles[i] = (Tile*) malloc(sizeof(Tile) * n_col);
        if (tiles[i] == NULL){log_error("Couldn't malloc Tile* in map.c");}
    }
    for (int i = 0 ; i < n_row ; i++){
        for (int j = 0 ; j < n_col ; j++){
            tiles[i][j] = _init_tile(grid[i][j], (Coordinate){j, i});
        }
    }
    return tiles;
}

void p_draw_map(Map* map){
    for (int i = 0 ; i < map->rows; i++){
        for (int j = 0 ; j < map->cols ; j++){
            Tile tile = map->tiles[i][j];
            _draw_tile(tile, map->tileset);
        }
    }
}

Map* p_load_map(const char* path){
    FILE* file = fopen(path, "r");
    if (file == NULL) {log_error("File not found when trying to load the map");}
    int x = 0;
    int y = 0;
    int n_col = 0;
    int n_row = 0;
    int ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            y++;
            n_col = max(n_col, x);
            x = 0;
        } else {
            x++;
        }
    }
    fclose(file);
    n_row = y;
    log_info("Number of row parsed : %i", n_row);
    log_info("Number of col parsed : %i", n_col);
    char chars[n_row][n_col];
    file = fopen(path, "r");
    if (file == NULL) {log_error("File not found when trying to load the map");}
    x = 0;
    y = 0;
    ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            y++;
            x = 0;
        } else {
            chars[y][x] = ch;
            x++;
        }
    }
    fclose(file);
    TileSet* tileset = _init_tileset();
    Tile** tiles = _get_tile_grid(n_col, n_row, chars, tileset);
    Map* map = malloc(sizeof(Map));
    if (map == NULL){log_error("Cant malloc the map");}
    map->tiles = tiles;
    map->cols = n_col;
    map->rows = n_row;
    map->tileset = tileset;
    log_debug("Map loaded: cols = %d, rows = %d", map->cols, map->rows);
    return map;
}


void p_free_map(Map* map) {
    if (map == NULL) {
        return;
    }
    if (map->tiles != NULL) {
        for (int i = 0; i < map->rows; i++) {
            if (map->tiles[i] != NULL) {
                free(map->tiles[i]);
            }
        }
        free(map->tiles);
    }
    if (map->tileset != NULL) {
        UnloadTexture(map->tileset->texture);
        free(map->tileset);
    }
    free(map);
}
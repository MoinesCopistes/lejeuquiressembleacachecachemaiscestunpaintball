#include <stdlib.h>
#include "utils.c"
#include "defines.h"


Image ImageResizeNearestNeighbor(Image image, int newWidth, int newHeight) {
    Image result = GenImageColor(newWidth, newHeight, BLANK);

    float xRatio = (float)image.width / newWidth;
    float yRatio = (float)image.height / newHeight;

    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            int srcX = (int)(x * xRatio);
            int srcY = (int)(y * yRatio);
            Color pixel = GetImageColor(image, srcX, srcY);
            ImageDrawPixel(&result, x, y, pixel);
        }
    }

    return result;
}

/**
 * @param path the path to the png file
 * @param x the position in the tileset of the texture in x
 * @param y the position in the tileset of the texture in y
 * @param deg the rotation angle in degrees (trigonometric)
 * @return the image pointer loaded in RAM
*/
Image _init_tile(const char* path, int x, int y, int deg) {
    Image image = LoadImage(path);
    if (image.data == NULL) {
        log_error("Failed to load image: %s", path);
        return (Image){0};
    }
    ImageCrop(&image, (Rectangle){x, y, tile_size, tile_size});
    int newWidth = image.width * texture_scale;
    int newHeight = image.height * texture_scale;
    Image resizedImage = ImageResizeNearestNeighbor(image, newWidth, newHeight);
    UnloadImage(image);
    image = resizedImage;
    ImageRotate(&image, deg);
    return image;
}
/**
 * @param map the map to assemble
 * @return the texture in VRAM
 */
Texture2D p_assemble_atlas(Map* map) {
    if (map == NULL) {
        log_error("Map is NULL in p_assemble_atlas");
        return (Texture2D){0};
    }

    // Create a blank image for the atlas
    Image blank = GenImageColor(map->cols * tile_size, map->rows * tile_size, BLANK);
    if (blank.data == NULL) {
        log_error("Failed to create blank image");
        return (Texture2D){0};
    }

    // Draw each tile onto the blank image
    for (int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->cols; j++) {
            Tile tile = map->tiles[i][j];
            if (tile.sprite.data == NULL) {
                log_error("Tile at (%d, %d) has no sprite", i, j);
                continue;
            }
            ImageDraw(&blank,
                      tile.sprite,
                      (Rectangle){0, 0, tile.sprite.width, tile.sprite.height}, // Source rectangle
                      (Rectangle){tile_size * tile.pos.x, tile_size * tile.pos.y, tile.sprite.width, tile.sprite.height}, // Destination rectangle
                      WHITE);
        }
    }

    // Convert the image to a texture
    Texture2D texture = LoadTextureFromImage(blank);
    if (texture.id == 0) {
        log_error("Failed to load texture from image");
    }

    // Free the blank image
    UnloadImage(blank);

    return texture;
}

Image _get_image(char input){
    const char* path = "2D Pixel Dungeon Asset Pack v2.0/2D Pixel Dungeon Asset Pack/character and tileset/Dungeon_Tileset.png";
    log_debug("Calling get_image with %c", input);
    switch (input)
    {
    case ' ':
        return _init_tile(path, 6 * 16, 0, 0);
    case '1':
        return _init_tile(path, 0, 0, 0);
    case '2':
        return _init_tile(path, 16, 0, 0);
    case '3':
        return _init_tile(path, 5 * 16, 0, 0);
    case '4':
        return _init_tile(path, 0, 16, 0);
    case '5':
        return _init_tile(path, 6 *16, 8 * 16, 0);
    case '6':
        return _init_tile(path, 5 * 16, 16, 0);
    case '7':
        return _init_tile(path, 0, 4 * 16, 0);
    case '8':
        return _init_tile(path, 16, 4 * 16, 0);
    case '9':
        return _init_tile(path, 5 *16, 4* 16, 0);
    case '^':
        return _init_tile(path, 2 * 16, 0, 0);
    case '$':
        return _init_tile(path, 2 * 16, 0, 0);
    case 'ù':
        return _init_tile(path, 3 * 16, 5 * 16, 0);
    case 'µ':
        return _init_tile(path, 0, 5 * 16, 0);
    default:
        log_error("Char couldn't be parsed in map.c for %c",input);
        return _init_tile(path, 6 * 16, 0, 0);
        break;
    }
}
Tile** _get_tile_grid(int n_col, int n_row, const char grid[n_row][n_col]){
    Tile** tiles = (Tile**) malloc(sizeof(Tile*) * n_row);
    if (tiles == NULL){log_error("Couldn't malloc Tile** in map.c");}
    for (int i = 0 ; i < n_row ; i++){
        tiles[i] = (Tile*) malloc(sizeof(Tile) * n_row);
        if (tiles[i] == NULL){log_error("Couldn't malloc Tile* in map.c");}
    }
    for (int i = 0 ; i < n_row ; i++){
        for (int j = 0 ; j < n_col ; j++){
            tiles[i][j].pos = (Coordinate){j, i};
            Image img = _get_image(grid[i][j]);
            log_debug("New img received");
            tiles[i][j].sprite = img;
        }
    }
    return tiles;
}
void _print_map(Map* map){
    for (int i =0 ; i < map->rows ; i++){
        for (int j = 0 ; j < map->cols ; j++){
            log_debug("Tile at %i, %i is coor : %i,%i", j, i, map->tiles[i][j].pos.x,  map->tiles[i][j].pos.y);
        }
    }
}
void _print_chars(int n_col, int n_row, const char chars[n_col][n_row]){
    for (int i =0 ; i < n_row ; i++){
        for (int j = 0 ; j < n_col ; j++){
            log_debug("Char at %i, %i is coor : %c", j, i, chars[j][i]);
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
    _print_chars(n_col, n_row, chars);
    Tile** tiles = _get_tile_grid(n_col, n_row, chars);
    
    Map* map = malloc(sizeof(Map));
    if (map == NULL){log_error("Cant malloc the map");}
    map->tiles = tiles;
    map->cols = n_col;
    map->rows = n_row;
    log_debug("Map loaded: cols = %d, rows = %d", map->cols, map->rows);
    _print_map(map);
    return map;
}



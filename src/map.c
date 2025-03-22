#include "map.h"

/**
 * @param map the map to draw the textures
 */
void p_draw_map(Map map){
    for (int i = 0; i < map.n_tiles ; i++){
        p_draw_tile(map.tiles[i]);
    }
}

/**
 * @param tile the tile to draw
 */
void p_draw_tile(Tile tile){
    DrawTexture(tile.sprite, tile.pos.x, tile.pos.y, WHITE);
}

/**
 * @param path the path to the png file
 * @param x the position in the tileset of the texture in x
 * @param y the position in the tileset of the texture in y
 * @return the texture loaded in VRAM
*/
Texture2D p_load_image(const char* path, int x, int y){
    Image image = LoadImage(path);
    ImageCrop(&image, (Rectangle){ x, y, tile_size, tile_size });
    ImageResize(&image, image.width * tile_scale, image.height * tile_scale);
    Texture2D texture = LoadTextureFromImage(image); // Converts to texture in VRAM
    UnloadImage(image); // Unload from RAM
    return texture;
}
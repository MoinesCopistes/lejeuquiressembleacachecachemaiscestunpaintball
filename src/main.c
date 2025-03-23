#include "defines.h"
#include "geo.h"
#include "menu.h"
#include "world.h"
#include <log.h>
#include <networking.h>
#include <raylib.h>
#include <stdio.h>
#include "map.h"


/* GLOBAL VARIABLES */
World world = {
  .players = {NULL, NULL, NULL, NULL},
  .entities = {NULL},
  .playerID = 0,
  .playersNumber = 1
};

const int screenWidth = screen_x;
const int screenHeight = screen_y;
enum game_states game_state = IN_MENU;

float dt;


int main(int argc, char **argv) {

  SetTraceLogLevel(LOG_NONE);
  if (argc == 1) {
    printf("Starting a server...\n");
    p_start_server();
  } else {
    printf("Starting a client...\n");
    p_start_client();
  }
  while (!isConnected) {
  };

  init_multiplayer();

  Vector2 cursor = {0.0f, 0.0f};

  InitWindow(screenWidth, screenHeight, "paintball client");

  Button *menu_buttons = p_init_buttons();

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------
  Map* map = p_load_map("map.txt");
  
  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    cursor = GetMousePosition();
    Position cursor_nul_de_tristan = p_cast_vector_to_position(cursor);

    BeginDrawing();

    ClearBackground(RAYWHITE);

    dt = GetFrameTime();

    switch (game_state) {
    case IN_MENU:
      for (int i = 0; i < NUMBER_OF_MENU_BUTTONS; i++) {
        Button *button_ptr = &menu_buttons[i];
        p_menu_check_inputs(cursor, button_ptr);
        p_draw_button(button_ptr, WHITE, RED);
      }
      break;
    case IN_SETTINGS:

      break;
    case IN_GAME:
      p_draw_map(map);
      if (IsKeyDown(KEY_W)) {
        p_player_move(world.players[world.playerID], &cursor_nul_de_tristan);
      }
      // trucs

      // p_player_prey_move(players[1],&cursor,time);

      for (int i = 0; i < 4; i++) {
        if (world.players[i] != NULL) {
          DrawCircle((int)world.players[i]->hitbox.pos.x,
                     (int)world.players[i]->hitbox.pos.y, world.players[i]->hitbox.radius,
                     DARKBLUE);
        }
      }
      break;
    }

    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  free(menu_buttons);
  // p_player_prey_free(world.players[0]);
  // p_player_prey_free(world.players[1]);
  //--------------------------------------------------------------------------------------
  p_free_map(map);
  return 0;
}

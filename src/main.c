#include "defines.h"
#include "geo.h"
#include "map.h"
#include "menu.h"
#include "world.h"
#include <ctype.h>
#include <log.h>
#include <networking.h>
#include <raylib.h>
#include <stdio.h>

/* GLOBAL VARIABLES */
World world = {.players = {NULL, NULL, NULL, NULL},
               .entities = {NULL},
               .playerID = 0,
               .playersNumber = 1};

const int screenWidth = screen_x;
const int screenHeight = screen_y;
enum game_states game_state = IN_MENU;

float dt;

int main(int argc, char **argv) {

  SetTraceLogLevel(LOG_NONE);
  // if (argc == 1) {
  //   printf("Starting a server...\n");
  //   p_start_server();
  // } else {
  //   printf("Starting a client...\n");
  //   p_start_client();
  // }
  // while (!isConnected) {
  // };

  // init_multiplayer();

  Vector2 cursor = {0.0f, 0.0f};

  InitWindow(screenWidth, screenHeight, "paintball client");

  Button *menu_buttons = p_init_menu_buttons();
  Button *server_buttons = p_init_client_buttons();
  Input *server_inputs = p_init_client_inputs();
  int focused_server_input = 0;
  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------
  Map *map = p_load_map("map.txt");
  Texture2D text = p_assemble_atlas(map);
  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    cursor = GetMousePosition();
    Position cursor_nul_de_tristan = p_cast_vector_to_position(cursor);

    BeginDrawing();

    ClearBackground(RAYWHITE);
    DrawTexture(text, 0, 0, WHITE);
    dt = GetFrameTime();

    switch (game_state) {
    case IN_MENU:
      for (int i = 0; i < NUMBER_OF_MENU_BUTTONS; i++) {
        Button *button_ptr = &menu_buttons[i];
        p_button_check_inputs(cursor, button_ptr);
        p_draw_button(button_ptr, WHITE, RED);
      }
      break;
    case IN_CLIENT:
      for (int i = 0; i < NUMBER_OF_SERVER_BUTTONS; i++) {
        Button *button_ptr = &server_buttons[i];
        p_button_check_inputs(cursor, button_ptr);
        p_draw_button(button_ptr, WHITE, RED);
      }
      if (IsKeyPressed(KEY_TAB)) {
        focused_server_input =
            (focused_server_input + 1) % NUMBER_OF_SERVER_INPUTS;
      }
      if (IsKeyPressed(KEY_BACKSPACE)) {
        Input *i = &server_inputs[focused_server_input];
        i->content[i->cursor_pos] = 0;
        if (i->cursor_pos > -1)
          i->cursor_pos--;
      }

      int key = GetCharPressed();
      if (key > 0 && isprint(key)) {
        Input *i = &server_inputs[focused_server_input];
        i->cursor_pos++;
        if (i->cursor_pos >= 15) {
          i->cursor_pos--;
        } else {
          i->content[i->cursor_pos] = key;
        }
      }
      for (int i = 0; i < NUMBER_OF_SERVER_INPUTS; i++) {
        Input *input_ptr = &server_inputs[i];
        p_draw_input(input_ptr, focused_server_input == i);
      }

      break;
    case IN_SERVER:
      break;
    case IN_GAME:
      if (IsKeyDown(KEY_W)) {
        p_player_move(world.players[world.playerID], &cursor_nul_de_tristan);
      }
      // trucs

      // p_player_prey_move(players[1],&cursor,time);

      for (int i = 0; i < 4; i++) {
        if (world.players[i] != NULL) {
          DrawCircle((int)world.players[i]->hitbox.pos.x,
                     (int)world.players[i]->hitbox.pos.y,
                     world.players[i]->hitbox.radius, DARKBLUE);
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

  return 0;
}

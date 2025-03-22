#include "defines.h"
#include "geo.h"
#include "menu.h"
#include "player.h"
#include "world.h"
#include <log.h>
#include <networking.h>
#include <raylib.h>
#include <stdio.h>

const int screenWidth = 1000;
const int screenHeight = 600;
enum game_states game_state = IN_MENU;

PlayerPrey *players[4] = {NULL, NULL, NULL, NULL};
float dt;

// 0 is the server
// if this game is a client, the server will
// attribute a new playerID
int playerID = 0;
int playersNumber = 1;

int main(int argc, char **argv) {
  // Initialization
  //--------------------------------------------------------------------------------------
  // Removing raylib log
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
      if (IsKeyDown(KEY_W)) {
        p_player_prey_move(players[playerID], &cursor_nul_de_tristan);
      }
      // trucs

      // p_player_prey_move(players[1],&cursor,time);

      for (int i = 0; i < 4; i++) {
        if (players[i] != NULL) {
          DrawCircle((int)players[i]->hitbox.pos.x,
                     (int)players[i]->hitbox.pos.y, players[i]->hitbox.radius,
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
  p_player_prey_free(players[0]);
  p_player_prey_free(players[1]);
  free(menu_buttons);
  //--------------------------------------------------------------------------------------

  return 0;
}

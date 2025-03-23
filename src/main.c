#include "defines.h"
#include "geo.h"
#include "map.h"
#include "menu.h"
#include "player.h"
#include "sound.h"
#include "world.h"
#include <ctype.h>
#include <log.h>
#include <networking.h>
#include <raylib.h>
#include <stdio.h>
#include "sound.h"

/* GLOBAL VARIABLES */
World world = {.players = {NULL, NULL, NULL, NULL},
               .entities = {NULL},
               .playerID = 0,
               .playersNumber = 1,
               .offset = {0, 0}};

const int screenWidth = screen_x;
const int screenHeight = screen_y;
enum game_states game_state = IN_MENU;
char menuError[256] = {0};
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
  InitAudioDevice();
  Button *menu_buttons = p_init_menu_buttons();
  Button *server_buttons = p_init_client_buttons();
  Input *client_inputs = p_init_client_inputs();
  Button *lobby_buttons = p_init_lobby_buttons();
  int focused_server_input = 0;
  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------

  Map *map = p_load_map("map.txt");
  world.map = map;
  Sounds *sounds = p_init_sounds();


  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    cursor = GetMousePosition();
    Position cursor_pos_with_offset = cursor_with_offset(cursor);

    BeginDrawing();

    ClearBackground(PURPLE);

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
      DrawText(menuError, 440, 200, 25, RED);
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
        Input *i = &client_inputs[focused_server_input];
        i->content[i->cursor_pos] = 0;
        if (i->cursor_pos > -1)
          i->cursor_pos--;
      }

      int key = GetCharPressed();
      if (key > 0 && isprint(key)) {
        Input *i = &client_inputs[focused_server_input];
        i->cursor_pos++;
        if (i->cursor_pos >= 15) {
          i->cursor_pos--;
        } else {
          i->content[i->cursor_pos] = key;
        }
      }
      for (int i = 0; i < NUMBER_OF_SERVER_INPUTS; i++) {
        Input *input_ptr = &client_inputs[i];
        p_draw_input(input_ptr, focused_server_input == i);
      }

      break;
    case IN_LOBBY:
      for (int i = 0; i < world.playersNumber; i++) {
        DrawCircle(250 + 250 * i, 450, 50, BLUE);
        char t[2] = {48 + i, 0};
        DrawText(t, 240 + 250 * i, 510, 25, WHITE);
      }
      DrawText("Waiting for others to join...", 300, 600, 50, WHITE);
      if (isServer) {
        for (int i = 0; i < NUMBER_OF_LOBBY_BUTTONS; i++) {
          Button *button_ptr = &lobby_buttons[i];
          p_button_check_inputs(cursor, button_ptr);
          p_draw_button(button_ptr, WHITE, RED);
        }

        DrawText(menuError, 440, 200, 25, RED);
      }
      break;
    case IN_GAME:

      if (IsKeyDown(KEY_W)) {
        p_player_move(world.players[world.playerID], &cursor_pos_with_offset,
                      map);
      } else {
        if (p_player_update_orientation(world.players[world.playerID],
                                        (Position *)&cursor_pos_with_offset))
          p_player_send_event_player_move(world.players[world.playerID]);
      }

      p_player_update_tagged();

      p_camera_follow();
      p_draw_map(map);
      p_update_players();
      for (int i = 0; i < 4; i++) {

        if (world.players[i] != NULL) {
          if (!world.players[i]->alive)
            DrawCircle(world.players[i]->hitbox.pos.x - world.offset.x,
                       world.players[i]->hitbox.pos.y - world.offset.y,
                       world.players[i]->hitbox.radius, GREEN);
          else if (world.players[i]->tagged)
            DrawCircle(world.players[i]->hitbox.pos.x - world.offset.x,
                       world.players[i]->hitbox.pos.y - world.offset.y,
                       world.players[i]->hitbox.radius, YELLOW);
          else {
            if (world.players[i]->type == PLAYER_HUNTER) {
              DrawCircle(world.players[i]->hitbox.pos.x - world.offset.x,
                         world.players[i]->hitbox.pos.y - world.offset.y,
                         world.players[i]->hitbox.radius, RED);
            } else if (world.players[world.playerID]->type != PLAYER_HUNTER) {
              DrawCircle(world.players[i]->hitbox.pos.x - world.offset.x,
                         world.players[i]->hitbox.pos.y - world.offset.y,
                         world.players[i]->hitbox.radius, DARKBLUE);
            }
          }
        }
      }

      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        p_player_paint_ball_shoot(world.players[world.playerID]);
      }

      if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        p_player_stab(world.players[world.playerID]);
      }

      p_entity_tab_update();

      p_entity_tab_draw_paint_balls();

      p_entity_tab_dead_free();
      break;
    }

    EndDrawing();
    //----------------------------------------------------------------------------------
  }
  CloseAudioDevice();
  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  free(menu_buttons);
  // p_player_prey_free(world.players[0]);
  // p_player_prey_free(world.players[1]);
  //--------------------------------------------------------------------------------------
  p_free_map(map);
  p_free_sounds(sounds);
  return 0;
}

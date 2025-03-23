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

  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = screen_x;
  const int screenHeight = screen_y;

  InitWindow(screenWidth, screenHeight, "paintball client");

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------
  Map* map = p_load_map("map.txt");
  Texture2D text = p_assemble_atlas(map);
  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    ClearBackground(RAYWHITE);
    DrawTexture(text, 0, 0, WHITE);
    dt = GetFrameTime();
    // Update
    //----------------------------------------------------------------------------------
    // TODO: Update your variables here
    //----------------------------------------------------------------------------------

    Position cursor;
    cursor.x = (float)GetMouseX();
    cursor.y = (float)GetMouseY();
    // trucs

    if (IsKeyDown(KEY_W)) {
      p_player_move(world.players[world.playerID], &cursor);
    }
    else
    {
        if(p_player_update_orientation(world.players[world.playerID], &cursor))
            p_player_send_event_player_move(world.players[world.playerID]);
    }

    for (int i = 0; i < 4; i++) {
      if (world.players[i] != NULL) {
        if(world.players[i]->alive)
            DrawCircle((int)world.players[i]->hitbox.pos.x, (int)world.players[i]->hitbox.pos.y,world.players[i]->hitbox.radius, DARKBLUE);
        else
            DrawCircle((int)world.players[i]->hitbox.pos.x, (int)world.players[i]->hitbox.pos.y,world.players[i]->hitbox.radius, GREEN);
      }
    }

    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        p_player_paint_ball_shoot(world.players[world.playerID]);
    }
    
    p_entity_tab_update();

    p_entity_tab_draw_paint_balls();

    p_entity_tab_dead_free();

    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        p_player_stab(world.players[world.playerID]);
    }

    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  // p_player_prey_free(world.players[0]);
  // p_player_prey_free(world.players[1]);
  //--------------------------------------------------------------------------------------

  return 0;
}

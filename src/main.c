#include "menu.h"
#include <raylib.h>
#include <string.h>

enum game_states {
  IN_MENU,
  IN_SETTINGS,
  IN_GAME,
};

int main(void) {
  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = 800;
  const int screenHeight = 450;
  enum game_states game_state = IN_MENU;

  InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

  Texture2D test_button_texture =
      LoadTexture("resources/NIQUE_TOIIIIIII.png"); // Load button texture

  // Define frame rectangle for drawing
  float test_frame_height = (float)test_button_texture.height / 2;
  Rectangle test_source_rec = {0, 0, (float)test_button_texture.width,
                               test_frame_height};

  // Define button bounds on screen
  Rectangle test_btn_bounds = {
      screenWidth / 2.0f - test_button_texture.width / 2.0f,
      screenHeight / 2.0f - test_button_texture.height / 2.0f / 2.0f,
      (float)test_button_texture.width, test_frame_height};

  Button test_button = {test_button_texture,
                        test_source_rec,
                        test_btn_bounds,
                        test_frame_height,
                        0,
                        NULL};

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    switch (game_state) {
    case IN_MENU:

      break;
    case IN_SETTINGS:

      break;
    case IN_GAME:

      break;
    }

    BeginDrawing();

    ClearBackground(RAYWHITE);

    p_draw_button(&test_button);

    DrawText("Congrats! You created your first window!", 190, 200, 20,
             LIGHTGRAY);

    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}

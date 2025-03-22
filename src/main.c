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
  const int screenWidth = 1000;
  const int screenHeight = 500;
  enum game_states game_state = IN_MENU;
  Vector2 mousePoint = {0.0f, 0.0f};

  InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

  Texture2D test_button_texture =
      LoadTexture("resources/button_background.png"); // Load button texture

  // Define frame rectangle for drawing
  float test_frame_height = (float)test_button_texture.height / 3;
  Rectangle test_source_rec = {0, 0, (float)test_button_texture.width,
                               test_frame_height};

  // Define button bounds on screen
  Rectangle test_btn_bounds = {
      screenWidth / 2.0f - test_source_rec.width / 2.0f,
      screenHeight / 2.0f - test_frame_height / 2.0f,
      (float)test_button_texture.width, test_frame_height};

  char *test_string = "SKIBIDI TOILETTE";

  Button test_button = {
      test_button_texture,
      test_source_rec,
      test_btn_bounds,
      test_frame_height,
      0,
      4,
      test_string,
      p_play_sound_void_arg,
  };

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    mousePoint = GetMousePosition();

    BeginDrawing();

    ClearBackground(RAYWHITE);

    switch (game_state) {
    case IN_MENU:
      p_menu_check_inputs(mousePoint, &test_button);
      p_draw_button(&test_button, WHITE, RED);
      break;
    case IN_SETTINGS:

      break;
    case IN_GAME:
      break;
    }

    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}

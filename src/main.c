#include "defines.h"
#include "menu.h"
#include <raylib.h>
#include <stdio.h>

enum game_states {
  IN_MENU,
  IN_SETTINGS,
  IN_GAME,
};

const int screenWidth = 1000;
const int screenHeight = 600;

int main(void) {
  // Initialization
  //--------------------------------------------------------------------------------------
  enum game_states game_state = IN_MENU;
  Vector2 mousePoint = {0.0f, 0.0f};

  InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

  Button *menu_buttons = p_init_buttons();

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
      for (int i = 0; i < NUMBER_OF_MENU_BUTTONS; i++) {
        Button *button_ptr = &menu_buttons[i];
        p_menu_check_inputs(mousePoint, button_ptr);
        p_draw_button(button_ptr, WHITE, RED);
      }
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

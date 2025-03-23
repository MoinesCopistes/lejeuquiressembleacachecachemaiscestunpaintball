#include "menu.h"
#include "defines.h"
#include "networking.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void p_change_state_to_in_game() { game_state = IN_GAME; }
void p_change_state_to_in_menu() { game_state = IN_MENU; }
void p_change_state_to_in_client() { game_state = IN_CLIENT; }
void p_change_state_to_in_server() {
    p_start_server();
    init_multiplayer();
    game_state = IN_GAME;

}

Button *p_init_menu_buttons() {
  Button *buttons = malloc(sizeof(Button) * NUMBER_OF_MENU_BUTTONS);
  Texture2D button_background_texture =
      LoadTexture("resources/button_background.png"); // Load button texture

  // Define frame rectangle for drawing
  float button_background_frame_height =
      (float)button_background_texture.height / 3;
  Rectangle button_background_source_rectangle = {
      0, 0, (float)button_background_texture.width,
      button_background_frame_height};

  // Define button bounds on screen
  Rectangle start_button_bounds = {
      screenWidth / 2.0f - button_background_source_rectangle.width / 2.0f,
      screenHeight / 2.0f - button_background_frame_height / 2.0f,
      (float)button_background_texture.width, button_background_frame_height};

  Button start_button = {
      button_background_texture,
      button_background_source_rectangle,
      start_button_bounds,
      button_background_frame_height,
      0,
      4,
      "Explore solo",
      p_change_state_to_in_game,
  };

  buttons[0] = start_button;

  // Define button bounds on screen
  Rectangle settings_button_bounds = {
      screenWidth / 2.0f - button_background_source_rectangle.width / 2.0f,
      screenHeight / 2.0f - button_background_frame_height / 2.0f +
          button_background_frame_height * 1.7f,
      (float)button_background_texture.width, button_background_frame_height};

  Button server_button = {
      button_background_texture,
      button_background_source_rectangle,
      settings_button_bounds,
      button_background_frame_height,
      0,
      4,
      "Join a server",
      p_change_state_to_in_client,
  };
  buttons[1] = server_button;

  // Define button bounds on screen
  Rectangle IP_button_bounds = {
      screenWidth / 2.0f - button_background_source_rectangle.width / 2.0f,
      screenHeight / 2.0f - button_background_frame_height / 2.0f +
          button_background_frame_height * 3.4f,
      (float)button_background_texture.width, button_background_frame_height};

  Button client_button = {
      button_background_texture,
      button_background_source_rectangle,
      IP_button_bounds,
      button_background_frame_height,
      0,
      4,
      "Host a server",
      p_change_state_to_in_server,
  };
  buttons[2] = client_button;
  return buttons;
}

Button *p_init_client_buttons() {
  Button *buttons = malloc(sizeof(Button) * NUMBER_OF_SERVER_BUTTONS);
  Texture2D button_background_texture =
      LoadTexture("resources/button_background.png"); // Load button texture

  // Define frame rectangle for drawing
  float button_background_frame_height =
      (float)button_background_texture.height / 3;
  Rectangle button_background_source_rectangle = {
      0, 0, (float)button_background_texture.width,
      button_background_frame_height};

  // Define button bounds on screen
  Rectangle start_button_bounds = {
      screenWidth / 2.0f - button_background_source_rectangle.width / 2.0f,
      screenHeight / 2.0f - button_background_frame_height / 2.0f +
          button_background_frame_height * 1.7 * 2,

      (float)button_background_texture.width, button_background_frame_height};

  Button start_button = {
      button_background_texture,
      button_background_source_rectangle,
      start_button_bounds,
      button_background_frame_height,
      0,
      4,
      "Connect to the server !",
      p_change_state_to_in_game,
  };

  buttons[0] = start_button;
  // Define button bounds on screen
  Rectangle back_button_bounds = {
      screenWidth / 2.0f - button_background_source_rectangle.width / 2.0f,
      screenHeight / 2.0f - button_background_frame_height / 2.0f +
          button_background_frame_height * 1.7 * 3,
      (float)button_background_texture.width, button_background_frame_height};

  Button back_button = {
      button_background_texture,
      button_background_source_rectangle,
      back_button_bounds,
      button_background_frame_height,
      0,
      4,
      "Back to the menu",
      p_change_state_to_in_menu,
  };

  buttons[1] = back_button;
  return buttons;
}

Input *p_init_client_inputs() {
  Input *inputs = malloc(sizeof(Input) * NUMBER_OF_SERVER_INPUTS);
  Rectangle rect = {
      .height = 40,
      .width = 300,
      .x = 490,
      .y = 300,
  };
  Input ip_input = {.content = {'1', '2', '7', '.', '0', '.', '0', '.', '1', 0,
                                0, 0, 0, 0, 0},
                    .cursor_pos = 8,
                    .display = rect};

  Rectangle rect2 = {
      .height = 40,
      .width = 300,
      .x = 490,
      .y = 400,
  };
  Input port_input = {
      .content = {'8', '0', '8', '0', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      .cursor_pos = 3,
      .display = rect2};
  inputs[1] = port_input;
  inputs[0] = ip_input;
  return inputs;
}

void p_draw_input(Input *input, bool focused) {
  Color draw_color = {.r = 255, .g = 255, .b = 255, .a = 200};
  if (!focused) draw_color = WHITE;
  DrawRectangleRec(input->display, draw_color);
  Color text_color = BLACK;
  if (!focused) text_color = GRAY;
  DrawText(input->content, input->display.x + 15, input->display.y + 10, 25,
           text_color);
  if (focused) {
    if ((int)GetTime() % 2 >= 1) {

      int textWidht = MeasureText(input->content, 25);

      Rectangle cursor = {.x = input->display.x + textWidht + 20,
                          .y = input->display.y + 7,
                          .height = input->display.height - 10,
                          .width = 3};
      DrawRectangleRec(cursor, DARKGRAY);
    }
  }
}

void p_draw_button(Button *button, Color tint, Color text_color) {
  float scale_factor =
      button->button_bounds.width / button->source_rectangle.width;

  Rectangle dest = {button->button_bounds.x, button->button_bounds.y,
                    button->source_rectangle.width * scale_factor,
                    button->source_rectangle.height * scale_factor};

  Vector2 origin = {0.0f, 0.0f};
  button->source_rectangle.y = button->frame_height * button->current_frame;

  DrawTexturePro(button->button_texture, button->source_rectangle, dest, origin,
                 0.0f, tint);

  if (button->button_string != NULL) {
    int string_length = strlen(button->button_string);
    int font_size;

    int available_width =
        (button->button_texture.width - button->border_thickness * 2) /
        string_length;

    int available_height =
        button->button_texture.height / 3 - button->border_thickness * 2;

    // printf("available hei %i\n", available_height);
    // printf("available wi %i\n", available_width);

    if (available_width < available_height) {
      font_size = available_width;
    } else {
      font_size = available_height;
    }

    int text_pos_x =
        button->button_bounds.x +
        (button->button_bounds.width - string_length * font_size * 0.6) / 2;
    int text_pos_y = button->button_bounds.y +
                     (button->button_bounds.height - font_size * 0.7) / 2;

    DrawText(button->button_string, text_pos_x, text_pos_y, font_size,
             text_color);
  }
}

void p_button_check_inputs(Vector2 mouse_pos, Button *button) {
  // Check button state
  if (CheckCollisionPointRec(mouse_pos, button->button_bounds)) {

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
      button->current_frame = 2;
    else
      button->current_frame = 1;
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
      button->p_button_function();
    }

  } else
    button->current_frame = 0;
}

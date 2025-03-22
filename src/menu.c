#include "defines.h"
#include "menu.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Button *p_init_buttons() {
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
      "Start",
      NULL,
  };

  buttons[0] = start_button;

  // Define button bounds on screen
  Rectangle settings_button_bounds = {
      screenWidth / 2.0f - button_background_source_rectangle.width / 2.0f,
      screenHeight / 2.0f - button_background_frame_height / 2.0f +
          button_background_frame_height * 1.7f,
      (float)button_background_texture.width, button_background_frame_height};

  Button settings_button = {
      button_background_texture,
      button_background_source_rectangle,
      settings_button_bounds,
      button_background_frame_height,
      0,
      4,
      "Settings",
      NULL,
  };
  buttons[1] = settings_button;

  // Define button bounds on screen
  Rectangle IP_button_bounds = {
      screenWidth / 2.0f - button_background_source_rectangle.width / 2.0f,
      screenHeight / 2.0f - button_background_frame_height / 2.0f +
          button_background_frame_height * 3.4f,
      (float)button_background_texture.width, button_background_frame_height};

  Button IP_button = {
      button_background_texture,
      button_background_source_rectangle,
      IP_button_bounds,
      button_background_frame_height,
      0,
      4,
      "Look for IP",
      NULL,
  };
  buttons[2] = IP_button;
  return buttons;
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
      font_size = available_width * 1.6;
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

void p_menu_check_inputs(Vector2 mouse_pos, Button *button) {
  // Check button state
  if (CheckCollisionPointRec(mouse_pos, button->button_bounds)) {

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
      button->current_frame = 2;
    else
      button->current_frame = 1;

  } else
    button->current_frame = 0;

  if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    // button->p_button_function((void *)&fxButton);
  }
}

void *p_play_sound_void_arg(void *sound) { PlaySound(*(Sound *)sound); }

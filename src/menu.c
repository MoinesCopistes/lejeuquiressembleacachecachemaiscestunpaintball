#include "menu.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
      font_size = available_height * 1.6;
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
    Sound fxButton = LoadSound("resources/record_out.wav"); // Load button sound
    button->p_button_function((void *)&fxButton);
    UnloadSound(fxButton); // Unload sound
  }
}

void *p_play_sound_void_arg(void *sound) { PlaySound(*(Sound *)sound); }

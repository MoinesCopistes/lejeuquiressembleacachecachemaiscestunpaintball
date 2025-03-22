#include "menu.h"
#include <raylib.h>
#include <stdlib.h>
#include <string.h>

void p_draw_button(Button *button, Color tint, Color text_color) {
  float scale_factor =
      button->button_bounds.width / button->source_rectangle.width;
  Rectangle dest = {button->button_bounds.x, button->button_bounds.y,
                    button->source_rectangle.width * scale_factor,
                    button->source_rectangle.height * scale_factor};

  Vector2 origin = {0.0f, 0.0f};

  DrawTexturePro(button->button_texture, button->source_rectangle, dest, origin,
                 0.0f, tint);

  if (button->button_string != NULL) {
    int string_length = strlen(button->button_string);
    int font_size;

    int available_width =
        (button->button_texture.width - button->border_thickness * 2) /
        string_length;

    int available_height =
        button->button_texture.height - button->border_thickness * 2;

    if (available_width < available_height) {
      font_size = available_width;
    } else {
      font_size = available_height;
    }

    DrawText(button->button_string, button->button_bounds.x,
             button->button_bounds.y, font_size, text_color);
  }
}

#include "menu.h"
#include <raylib.h>

void p_draw_button(Button *button) {
  DrawTextureRec(button->button_texture, button->source_rectangle,
                 (Vector2){button->button_bounds.x, button->button_bounds.y},
                 WHITE);
}

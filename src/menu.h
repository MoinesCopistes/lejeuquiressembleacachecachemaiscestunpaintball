#pragma once
#include <raylib.h>

typedef struct {
  Texture2D button_texture; // contains a big file with all the button's frames
  Rectangle source_rectangle; // rect viewport on the texture
  Rectangle button_bounds;    // rect physical button
  float frame_height;         // the height of one button frame
  int current_frame;          // the current frame to display
  void *(*p_button_function)(void *arg1, void *arg2);
} Button;

void p_draw_button(Button *button); // draws a button

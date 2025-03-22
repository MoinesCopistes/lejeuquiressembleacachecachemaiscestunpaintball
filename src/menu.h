#pragma once
#include <raylib.h>

typedef struct {
  Texture2D button_texture; // contains a big file with all the button's frames
  Rectangle source_rectangle; // rect viewport on the texture
  Rectangle button_bounds;    // rect physical button
  float frame_height;         // the height of one button frame
  int current_frame;          // the current frame to display
  int border_thickness; // the thickness of the border of the texture (to draw
                        // in the intended center of the button with the right
                        // font size)
  char *button_string;  // the string that will be showed by the button
  void *(*p_button_function)(
      void *arg_struct); // function that is called by the button
} Button;

void p_draw_button(Button *button, Color tint,
                   Color text_color); // draws a button

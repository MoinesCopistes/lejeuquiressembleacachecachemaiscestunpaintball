#pragma once
#include <raylib.h>

#define NUMBER_OF_MENU_BUTTONS 3
#define NUMBER_OF_SERVER_BUTTONS 2
#define NUMBER_OF_SERVER_INPUTS 2

typedef struct {
  Texture2D button_texture; // contains a big file with all the button's frames
  Rectangle source_rectangle; // rect viewport on the texture
  Rectangle button_bounds;    // rect physical button
  float frame_height;         // the height of one button frame
  int current_frame;    // the current frame to display (0 : normal, 1 : mouse
                        // over, 2 : mouse click)
  int border_thickness; // the thickness of the border of the texture (to draw
                        // in the intended center of the button with the right
                        // font size)
  char *button_string;  // the string that will be showed by the button
  void (*p_button_function)(); // function that is called by the button
} Button;

typedef struct {
  char content[15];
  int cursor_pos;
  Rectangle display;
} Input;


void p_draw_button(Button *button, Color tint,
                   Color text_color); // draws a button
void p_button_check_inputs(Vector2 mouse_position,
                         Button *button); // check inputs for menu
Button *p_init_client_buttons();                 // init all server buttons
Button *p_init_menu_buttons();                 // init all menu buttons
Input *p_init_client_inputs();
void p_draw_input(Input *button, bool focused); // draws a button

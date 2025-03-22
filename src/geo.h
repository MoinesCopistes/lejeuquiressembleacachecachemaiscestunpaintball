#ifndef GEO_H
#define GEO_H

#include "raylib.h"
#include <stdlib.h>
#include <math.h>

//! Attention : les fonctions de distances prennent des pointeurs en arguments (comme ça on recopie pas le struct à chaque fois)

typedef struct Position Position;
struct Position
{
    float x, y;
};

float p_distance_position_square(Position *pos1, Position *pos2);
float p_distance_position(Position *pos1, Position *pos2);

typedef struct Circle Circle;
struct Circle
{
    Position pos;
    float radius;
};

int p_point_is_in_circle(Position *pos, Circle *circle);
int p_circle_is_in_circle(Circle *circle1, Circle *circle2);

typedef struct Rect Rect;
struct Rect
{
    Position pos; //TOP-LEFT CORNER
    float w, h;
};

int p_point_is_in_rect(Position *pos, Rect *rect);
int p_circle_is_in_rect(Circle *circle, Rect *rect);


float p_fast_inverse_sqrt(float number);

Position p_cast_vector_to_position(Vector2 vec);


#endif

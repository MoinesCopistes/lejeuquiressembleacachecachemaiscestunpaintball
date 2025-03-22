#ifndef GEO_H
#define GEO_H

#include <stdlib.h>
#include <math.h>

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

int p_is_in_circle(Position *pos, Circle *circle);
int p_circle_interset(Circle *circle1, Circle *circle2);


#endif

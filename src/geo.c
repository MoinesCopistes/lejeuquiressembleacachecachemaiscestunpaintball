#include "geo.h"

float p_distance_position_square(Position *pos1, Position *pos2)
{
    return (pos1->x - pos2->x)*(pos1->x - pos2->x) + (pos1->y - pos2->y)*(pos1->y - pos2->y);
}

float p_distance_position(Position *pos1, Position *pos2)
{
    return sqrt(p_distance_position_square(pos1,pos2));
}

int p_is_in_circle(Position *pos, Circle *circle)
{
    if(p_distance_position_square(pos,&(circle->pos)) <= circle->radius * circle->radius)
        return 1;
    return 0;
}

int p_circle_interset(Circle *circle1, Circle *circle2)
{
    if(p_distance_position(&(circle1->pos), &(circle2->pos)) < (circle1->radius + circle2->radius)*(circle1->radius + circle2->radius))
        return 1;
    return 0;
}

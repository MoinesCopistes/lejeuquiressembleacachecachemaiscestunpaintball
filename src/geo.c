#include "geo.h"

float p_distance_position_square(Position *pos1, Position *pos2)
{
    return ((pos1->x - pos2->x)*(pos1->x - pos2->x) + (pos1->y - pos2->y)*(pos1->y - pos2->y));
}

float p_distance_position(Position *pos1, Position *pos2)
{
    return sqrt(p_distance_position_square(pos1,pos2));
}

int p_point_is_in_circle(Position *pos, Circle *circle)
{
    if(p_distance_position_square(pos,&(circle->pos)) <= circle->radius * circle->radius)
        return 1;
    return 0;
}

int p_circle_is_in_circle(Circle *circle1, Circle *circle2)
{
    if(p_distance_position_square(&(circle1->pos), &(circle2->pos)) < (circle1->radius + circle2->radius)*(circle1->radius + circle2->radius))
        return 1;
    return 0;
}

int p_point_is_in_rect(Position *pos, Rect *rect)
{
    if((pos->x > rect->pos.x) && (pos->x < rect->pos.x + rect->w) && (pos->y > rect->pos.y) && (pos->y < rect->pos.y + rect->h))
        return 1;
    return 0;
}

int p_circle_is_in_rect(Circle *circle, Rect *rect)
{
    if(p_point_is_in_rect(&(circle->pos),rect))
        return 1;
    
    float Xn = fmax(rect->pos.x, fmin(circle->pos.x, rect->pos.x + rect->w));
    float Yn = fmax(rect->pos.y, fmin(circle->pos.y, rect->pos.y + rect->h));
    float Dx = Xn - circle->pos.x;
    float Dy = Yn - circle->pos.y;

    return (Dx * Dx) + (Dy * Dy) <= circle->radius * circle->radius;
}

float p_fast_inverse_sqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}


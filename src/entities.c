#include "entities.h"

//Different entities

Paint_ball* p_paint_ball_create(Position *start, Position *cursor, unsigned int iD, unsigned int player_id, float speed_coeff, float radius, float splash_radius, float max_dis_squared)
{
    Paint_ball *ball = (Paint_ball *) malloc(sizeof(Paint_ball));

    ball->type = ENTITY_PAINT_BALL;

    ball->iD = iD;
    ball->player_id = player_id;
    
    ball->hitbox.pos.x = start->x;
    ball->hitbox.pos.y = start->y;
    ball->hitbox.radius = radius;
    ball->splash_radius = splash_radius;

    ball->dis_squared = 0.0;
    ball->max_dis_squared = max_dis_squared;

    float normal = p_fast_inverse_sqrt((cursor->x - start->x) * (cursor->x - start->x) + (cursor->y - start->y) * (cursor->y - start->y));

    ball->speed_x = speed_pain_ball * speed_coeff * normal * (cursor->x - start->x);
    ball->speed_y = speed_pain_ball * speed_coeff * normal * (cursor->y - start->y);

    return ball;
}

void p_paint_ball_free(Paint_ball *ball)
{
    free(ball);
}

void p_paint_ball_update(Paint_ball *ball, clock_t time)
{
    float dx = ball->speed_x * (time / CLOCKS_PER_SEC);
    float dy = ball->speed_y * (time / CLOCKS_PER_SEC);
    ball->hitbox.pos.x += dx;
    ball->hitbox.pos.y += dy;
    ball->dis_squared += (dx * dx) + (dy * dy);    
    ball->alive = ball->dis_squared < ball->max_dis_squared;
}

//Entity tab

Entity* EntityTab[OBJECT_LIMIT];

void p_entity_tab_init()
{
    for(unsigned int i = 0; i < OBJECT_LIMIT; ++i)
        EntityTab[i] = NULL;
}

void p_entity_all_tab_free()
{
    for(unsigned int i = 0; i < OBJECT_LIMIT; ++i)
    {
        if(EntityTab[i] != NULL)
        {
            switch(EntityTab[i]->type)
            {
                case ENTITY_PAINT_BALL:
                    Paint_ball* ball = (Paint_ball*) EntityTab[i];
                    p_paint_ball_free(ball);
                    EntityTab[i] = NULL;
                    break;
                default :
                    printf("This entity type is not handled by p_entity_tab_free. PlsFix.\n");
                    break;
            }
        }
    }
}

void p_entity_tab_update(clock_t time)
{
    for(unsigned int i = 0; i < OBJECT_LIMIT; ++i)
    {
        if(EntityTab[i] != NULL)
        {
            switch(EntityTab[i]->type)
            {
                case ENTITY_PAINT_BALL:
                    Paint_ball* ball = (Paint_ball*) EntityTab[i];
                    p_paint_ball_update(ball,time);
                    break;
                default :
                    printf("This entity type is not handled by p_entity_update.\n");
                    break;
            }
        }
    }
}

int p_entity_tab_is_full()
{
    for(unsigned int i = 0; i < OBJECT_LIMIT; ++i)
    {
        if(EntityTab[i] == NULL)
            return 0;
    }
    return 1;
}

int p_entity_tab_add(Entity* entity)
{
    for(unsigned int i = 0; i < OBJECT_LIMIT; ++i)
    {
        if(EntityTab[i] == NULL)
        {
            EntityTab[i] = entity;
            return 1;
        }
    }
    return 0;
}

void p_entity_tab_dead_free()
{
    for(unsigned int i = 0; i < OBJECT_LIMIT; ++i)
    {
        if(EntityTab[i] != NULL)
        {
            if(EntityTab[i]->alive == 0)
            {
                switch(EntityTab[i]->type)
                {
                    case ENTITY_PAINT_BALL:
                        Paint_ball* ball = (Paint_ball*) EntityTab[i];
                        p_paint_ball_free(ball);
                        EntityTab[i] = NULL;
                        break;
                    default :
                        printf("This entity type is not handled by p_entity_tab_dead_free(). PlsFix.\n");
                        break;
                }
            }
        }
    }
}

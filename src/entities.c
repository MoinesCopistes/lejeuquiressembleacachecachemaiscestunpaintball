#include "entities.h"

Entity* p_entity_create(EntityType type, unsigned long size)
{
    Entity *e = (Entity *) malloc(size);
    e->type = type;
    e->alive = 1;
    return e;
}

void (*EntityUpdateFunctions[ENTITY_NUMBER])(Entity *entity) = {
    p_paint_ball_update
};

void (*EntityFreeFunctions[ENTITY_NUMBER])(Entity *entity) = {
    p_paint_ball_free
};

//Different entities

Paint_ball* p_paint_ball_create(Position *start, Position *cursor, unsigned int iD, unsigned int player_id, float speed_coeff, float radius, float splash_radius, float max_dis_squared)
{
    Paint_ball *ball = (Paint_ball *) p_entity_create(ENTITY_PAINT_BALL,sizeof(Paint_ball));

    ball->iD = iD;
    ball->player_id = player_id;
    
    ball->hitbox.pos.x = start->x;
    ball->hitbox.pos.y = start->y;
    ball->hitbox.radius = radius;
    ball->splash_radius = splash_radius;

    ball->dis_squared = 0.0;
    ball->max_dis_squared = max_dis_squared;

    float normal = p_fast_inverse_sqrt((cursor->x - start->x) * (cursor->x - start->x) + (cursor->y - start->y) * (cursor->y - start->y));

    ball->speed_x = SPEED_PAINT_BALL * speed_coeff * normal * (cursor->x - start->x);
    ball->speed_y = SPEED_PAINT_BALL * speed_coeff * normal * (cursor->y - start->y);

    return ball;
}

void p_paint_ball_free(Entity *entity)
{
    Paint_ball *ball = (Paint_ball *) entity;
    free(ball);
}

void p_paint_ball_update(Entity *entity)
{
    Paint_ball *ball = (Paint_ball*) entity;
    float dx = ball->speed_x * dt;
    float dy = ball->speed_y * dt;
    ball->hitbox.pos.x += dx;
    ball->hitbox.pos.y += dy;
    ball->dis_squared += (dx * dx) + (dy * dy);    
    ball->e.alive = ball->dis_squared < ball->max_dis_squared;
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
            EntityFreeFunctions[EntityTab[i]->type](EntityTab[i]);
    }
}

void p_entity_tab_update()
{
    for(unsigned int i = 0; i < OBJECT_LIMIT; ++i)
    {
        if(EntityTab[i] != NULL)
            EntityUpdateFunctions[EntityTab[i]->type](EntityTab[i]);
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
                EntityFreeFunctions[EntityTab[i]->type](EntityTab[i]);
        }
    }
}

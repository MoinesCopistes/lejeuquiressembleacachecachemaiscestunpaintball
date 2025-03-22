#ifndef ENTITIES_H
#define ENTITIES_H

#define OBJECT_LIMIT 256

#include "geo.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum {ENTITY_PAINT_BALL,ENTITY_NONE} EntityType;

typedef struct Entity Entity;
struct Entity
{
    //--- Universel
    EntityType type;
    int alive;
    //---    
};

//Different entities

float speed_pain_ball; //on stocke la vitesse de base d'une paint ball globalement
typedef struct Paint_ball Paint_ball;
struct Paint_ball
{
    //--- Universel
    EntityType type;
    int alive;
    //---    
    float speed_x, speed_y; //on stocke dans la struct les vitesses horizontales et verticales (moins de calculs)
    Circle hitbox;
    float splash_radius;
    float dis_squared;
    float max_dis_squared;
    unsigned int player_id;
    unsigned int iD;
};

Paint_ball* p_paint_ball_create(Position *start, Position *cursor, unsigned int iD, unsigned int player_id, float speed_coeff, float radius, float splash_radius, float max_dis_squared);
void p_paint_ball_free(Paint_ball *ball); //au cas où il y a des pointeurs dedans plus tard
void p_paint_ball_update(Paint_ball *ball, clock_t time); //renvoie si la balle est "vivante" ou pas"

//Entity tab

extern Entity* EntityTab[OBJECT_LIMIT]; //UN ELEMENT VIDE DOIT ETRE ABSLUMENT NULL
void p_entity_tab_init();
void p_entity_tab_all_free(); //Pour free tout le tab. ON FREE -> ON MET NULL 
void p_entity_tab_update(clock_t time);
int p_entity_tab_is_full();
int p_entity_tab_add(Entity* entity);
void p_entity_tab_dead_free(); //Pour free les éléments label en dead. ON FREE -> ON MET NULL 

#endif

#ifndef ENTITIES_H
#define ENTITIES_H

#define OBJECT_LIMIT 256
#define SPEED_PAINT_BALL 1500.0

#include "geo.h"
#include <math.h>
#include <raylib.h>

#define PROJECTILE_SIZE  40.0
typedef enum {ENTITY_PAINT_BALL, ENTITY_NUMBER} EntityType;

typedef struct Entity Entity;
struct Entity
{
    //--- Universel
    EntityType type;
    int alive;
    unsigned int iD;
    //---    
};

Entity* p_entity_create(EntityType type, unsigned int iD, unsigned long size);

extern void (*EntityUpdateFunctions[ENTITY_NUMBER])(Entity *entity);
extern void (*EntityFreeFunctions[ENTITY_NUMBER])(Entity *entity);

//Different entities

extern unsigned int balls_shot;
typedef struct Paint_ball Paint_ball;
struct Paint_ball
{
    //--- Universel
    Entity e;
    //---    
    float speed_x, speed_y; //on stocke dans la struct les vitesses horizontales et verticales (moins de calculs)
    Circle hitbox;
    float splash_radius;
    float dis_squared;
    float max_dis_squared;
    unsigned int player_id;
};


Paint_ball* p_paint_ball_create(Position *start, float orientation, unsigned int player_id, float speed_coeff, float radius, float splash_radius, float max_dis_squared);
void p_paint_ball_free(Entity *entity); //au cas où il y a des pointeurs dedans plus tard
void p_paint_ball_update(Entity *entity); //renvoie si la balle est "vivante" ou pas"

//Entity tab

//TODO : Toutes les fonctions en dessous qui ont un switch ont besoin d'être modifiées à chaque fois qu'une nouvelle entité est codée

extern Entity* EntityTab[OBJECT_LIMIT]; //UN ELEMENT VIDE DOIT ETRE ABSLUMENT NULL
void p_entity_tab_init();
void p_entity_tab_all_free(); //Pour free tout le tab. ON FREE -> ON MET NULL 
void p_entity_tab_update();
int p_entity_tab_is_full();
unsigned int p_entity_tab_size(); //renvoie combien d'éléments sont dans le tableau
int p_entity_tab_add(Entity* entity);
void p_entity_tab_dead_free(); //Pour free les éléments label en dead. ON FREE -> ON MET NULL 
void p_entity_tab_draw_paint_balls(Texture2D* texture);

void p_draw_paintball(Texture2D* texture, Rectangle destination, Color color);

#endif


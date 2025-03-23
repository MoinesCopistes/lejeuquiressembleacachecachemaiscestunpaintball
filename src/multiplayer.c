#include "defines.h"
#include "entities.h"
#include "geo.h"
#include "log.h"
#include "map.h"
#include <networking.h>
#include <player.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/*
You MUST call this function to create any events.

Usage:

MyEvent* event = (MyEvent*) new_event(sizeof(MyEvent), MYEVENT_TYPE)

This will set all mandatory fields on the event.

*/
Event *new_event(unsigned long size, enum EventType type) {
  Event *e = malloc(size);
  e->magic = 69;
  e->playerID = world.playerID;
  e->type = type;
  pthread_mutex_init(&e->memberCountMutex, NULL);
  e->memberCount = 0;
  e->dont_free = 0;
  return e;
}

/*
This function will send HELLO event to the server
and wait for the server to respond with a player id for our new client.
This player id tells us how many players are already there
so we can populate players[i] with those players + ourself.
*/
void init_multiplayer() {
  // Wait for the server to get a playerID
  if (!isServer) {
    Event *hello = new_event(sizeof(Event), EVENT_HELLO);
    broadcast_event(hello, -1);
    log_info("Waiting for the server to get a playerID");
    while (!world.playerID) {
    }
    world.playersNumber = world.playerID + 1;
  }

  printf("Creating %d players\n", world.playersNumber);
  int *already_chosen = malloc(sizeof(int) * world.playersNumber);
  for (int i = 0; i <= world.playerID; i++) {
    Circle c;
    printf("Map doesn't have a spawn point\n");
    Circle temp = {{200 + 100 * i, 200}, 30};
    c = temp;
    world.players[i] = (Player *)p_player_prey_create(i, 400, &c);
  }
  free(already_chosen);
}

/*
This function is called whenever a player (server or client)
receives an event.

However, clientID will always be -1 for clients as clients can only
send messages to the server.

But the server will have clientID set, allowing it to send events
to particular clients in response of an event (cf EVENT_HELLO)
*/
void p_handle_event(Event *event, int clientID) {
  if (event->type == EVENT_HELLO && isServer) {
    log_info("New player detected. Giving a new playerID");
    EventAssignId *eai =
        (EventAssignId *)new_event(sizeof(EventAssignId), EVENT_ASSIGN_ID);
    eai->id = world.playersNumber;
    send_event((Event *)eai, clientID);
  }

  if (event->type == EVENT_HELLO) {
    int newClientId = world.playersNumber;
    log_info("Adding the player %d\n", newClientId);
    world.playersNumber++;
    Circle c = {{200 + 100 * newClientId, 200}, 30};
    world.players[newClientId] =
        (Player *)p_player_prey_create(newClientId, 400, &c);
  }

  if (event->type == EVENT_ASSIGN_ID) {
    EventAssignId *eai = (EventAssignId *)event;
    world.playerID = eai->id;
    log_info("Got the id %d ", world.playerID);
  }

  if (event->type == EVENT_PLAYER_MOVE) {
    EventPlayerMove *epm = (EventPlayerMove *)event;
    world.players[epm->e.playerID]->objectiveX = epm->x;
    world.players[epm->e.playerID]->objectiveY = epm->y;
    world.players[epm->e.playerID]->orientation = epm->orientation;
  }

  if (event->type == EVENT_PLAYER_SHOOT_PAINT_BALL) {
    EventPlayerShootPaintBall *epm = (EventPlayerShootPaintBall *)event;
    Paint_ball *ball = p_paint_ball_create(
        &(epm->start), epm->orientation, epm->player_id, epm->speed_coeff,
        epm->radius, epm->splash_radius, epm->max_dis_squared);
    p_entity_tab_add((Entity *)ball);
  }

  if (event->type == EVENT_KILL_ENTITY) {
    EventKillEntity *epm = (EventKillEntity *)event;
    for (unsigned int i = 0; i < OBJECT_LIMIT; ++i) {
      if (EntityTab[i] != NULL) {
        if (EntityTab[i]->iD == epm->iD)
          EntityTab[i]->alive = 0;
      }
    }
  }

  if (event->type == EVENT_KILL_PLAYER) {
    EventKillPlayer *ekp = (EventKillPlayer *)event;
    world.players[ekp->victim_iD]->alive = 0;
  }

  if (event->type == EVENT_TAG_PLAYER) {
    EventTagPlayer *etp = (EventTagPlayer *)event;
    world.players[etp->tagged_iD]->tagged = 1;
  }

  if (event->type == EVENT_STAB && isServer) {
    EventStab *es = (EventStab *)event;
    p_stab_calculate_broadcast(es->stabber_id);
  }

  if (event->type == EVENT_START) {
    int *already_chosen = malloc(sizeof(int) * world.playersNumber);
    Circle c;
    for (int i = 0; i < world.playersNumber; i++) {
      if (world.map->spawn_points_n > 0) {
        int random_spawn_index = p_random_int(0, world.map->spawn_points_n - 1);
        int times_already_chosen = 0;
        for (int j = 0; j < i; j++) {
          if (already_chosen[j] == random_spawn_index) {
            times_already_chosen++;
          }
        }
        already_chosen[i] = random_spawn_index;
        Circle temp = {
            {world.map->spawn_points[random_spawn_index].x * tile_size +
                 40 * times_already_chosen + 30,
             world.map->spawn_points[random_spawn_index].y * tile_size + 30},
            30};
        c = temp;
      } else {
        printf("Map doesn't have a spawn point\n");
        Circle temp = {{200 + 100 * i, 200}, 30};
        c = temp;
      }
      world.players[i] = (Player *)p_player_prey_create(i, 400, &c);
      world.players[i]->hitbox = c;
    }
    free(already_chosen);
    game_state = IN_GAME;
  }

  if (event->type == EVENT_SET_HUNTER) {
    int hunter = event->playerID;
    log_info("Player %d is the hunter\n", hunter);
    free((PlayerPrey *)world.players[hunter]);
    Circle c = {{0, 0}, 0};
    world.players[hunter] =
        (Player *)p_player_hunter_create(hunter, 500, &c, 20, 20);
  }
}

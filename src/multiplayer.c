#include "defines.h"
#include "log.h"
#include <networking.h>
#include <player.h>
#include <stdio.h>

Event *new_event(unsigned long size, enum EventType type) {
  Event *e = malloc(size);
  e->magic = 69;
  e->playerID = playerID;
  e->type = type;
  return e;
}

void init_multiplayer() {
  // Wait for the server to get a playerID
  if (!isServer) {
    Event *hello = new_event(sizeof(Event), EVENT_HELLO);
    broadcast_event(hello, -1);
    log_info("Waiting for the server to get a playerID");
    while (!playerID) {
    }
    playersNumber = playerID + 1;
  }

  for (int i = 0; i <= playerID; i++) {
    Circle c = {{200 + 100 * i, 200}, 30};
    players[i] = p_player_prey_create(i, 200, &c);
  }
}

void p_handle_event(Event *event, int clientID) {
  printf("Handling event type %d sent from client %d\n", event->type, clientID);
  if (event->type == EVENT_HELLO && isServer) {
    log_info("New player detected. Giving a new playerID");
    EventAssignId *eai =
        (EventAssignId *)new_event(sizeof(EventAssignId), EVENT_ASSIGN_ID);
    eai->id = playersNumber;
    send_event((Event *)eai, clientID);
  }

  if (event->type == EVENT_HELLO) {
    int newClientId = playersNumber;
    printf("Adding the %d player\n", newClientId);
    playersNumber++;
    Circle c = {{200 + 100 * newClientId, 200}, 30};
    players[newClientId] = p_player_prey_create(newClientId, 200, &c);
  }

  if (event->type == EVENT_ASSIGN_ID) {
    EventAssignId *eai = (EventAssignId *)event;
    playerID = eai->id;
    log_info("Got the id %d ", playerID);
  }

  if (event->type == EVENT_PLAYER_MOVE) {
    EventPlayerMove* epm = (EventPlayerMove*) event;
    players[epm->e.playerID]->hitbox.pos.x = epm->x;
    players[epm->e.playerID]->hitbox.pos.y = epm->y;
  }
}

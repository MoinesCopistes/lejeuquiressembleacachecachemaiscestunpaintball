#include <networking.h>
#include <stdio.h>

void handle_event(Event* event, int clientID) {
  printf("Handling event type %d sent from client %d\n", event->type, clientID);
}

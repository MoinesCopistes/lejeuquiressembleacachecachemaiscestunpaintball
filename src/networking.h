#include <chan.h>
#include <defines.h>
int p_start_server();
int p_start_client();

extern int clients[MAX_CLIENT_NUMBER];
extern int connectedClients;
extern chan_t *clientsChan[MAX_CLIENT_NUMBER];

enum EventType { EVENT_HELLO = 0, EVENT_PLAYER_MOVE };

typedef struct {
  int magic; // always 69
  enum EventType type;
} Event;

typedef struct {
  int magic; // always 69
  enum EventType type;
  float x;
  float y;
} EventPlayerMove;

void p_handle_event(Event *event, int clientID);

/*
@args event is the event to broadcast
@args senderID is the source of the event.
      should be -1 except if we are proxying event from client
      (unix_networking only)
*/
void send_event(Event *event, int clientID);

/*
@args event is the event to broadcast
@args senderID is the source of the event.
      should be -1 except if we are proxying event from client
      (unix_networking only)
*/
void broadcast_event(Event *event, int senderID);

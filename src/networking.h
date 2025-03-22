#include <chan.h>
#include <defines.h>
int p_start_server();
int p_start_client();

extern int clients[MAX_CLIENT_NUMBER];
extern int connectedClients;
extern chan_t *clientsChan[MAX_CLIENT_NUMBER];
extern int isConnected;
extern int isServer;
extern int playersNumber;
extern int playerID;

enum EventType {
  EVENT_HELLO = 0,
  EVENT_ASSIGN_ID,
  EVENT_PLAYER_MOVE
};

typedef struct {
  int magic; // always 69
  enum EventType type;
  int playerID;
  int memberCount;
  bool dont_free;
} Event;

typedef struct {
  Event e;
  int id;
} EventAssignId;

typedef struct {
  Event e;
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
void init_multiplayer();
Event *new_event(unsigned long size, enum EventType type);

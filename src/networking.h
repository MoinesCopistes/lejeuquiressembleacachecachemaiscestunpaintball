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
  // asks for an id and also tells other clients a new one is here
  EVENT_HELLO = 0,
  // message from server to client to give it an ID
  EVENT_ASSIGN_ID,

  // message for player movement
  EVENT_PLAYER_MOVE,

  EVENT_PLAYER_SHOOT_PAINT_BALL,

  EVENT_KILL_ENTITY, 

  EVENT_STAB, 

  EVENT_KILL_PLAYER,

  EVENT_TAG_PLAYER
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
  float orientation;
} EventPlayerMove;

typedef struct {
  Event e;
  Position start;
  float orientation;
  int player_id; //rofl
  float speed_coeff;
  float radius;
  float splash_radius;
  float max_dis_squared;
} EventPlayerShootPaintBall;

typedef struct {
  Event e;
  int iD;
} EventKillEntity;

typedef struct {
  Event e;
  int victim_iD;
} EventKillPlayer;

typedef struct {
  Event e;
  int tagged_iD;
} EventTagPlayer;

typedef struct {
  Event e;
  int stabber_id;
} EventStab;



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

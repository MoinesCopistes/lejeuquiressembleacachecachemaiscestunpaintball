#include <arpa/inet.h>
#include <chan.h>
#include <defines.h>
#include <log.h>
#include <netinet/in.h>
#include <networking.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

/* GLOBAL VARIABLES */

int clients[MAX_CLIENT_NUMBER];
int connectedClients = 0;
chan_t *clientsChan[MAX_CLIENT_NUMBER];

chan_t *ss_chan;

int isServer = 0;

int isConnected = 0;

/* EVENT HANDLING */

Event *parse_event(void *data) {
  Event *e = (Event *)data;
  if (e->magic != 69) {
    return NULL;
  }
  return e;
}

void send_event(Event *event, int client_id) {
  chan_t *chan;
  if (isServer) {
    chan = clientsChan[client_id];
  } else {
    chan = ss_chan;
  }
  chan_send(chan, event);
}

void broadcast_event(Event *event, int senderID) {
  if (isServer) {
    event->memberCount = connectedClients;
    if (senderID != -1) {
      event->memberCount--;
    }
    for (int i = 0; i < connectedClients; i++) {
      if (i == senderID)
        continue;
      send_event(event, i);
    }
  } else {
    send_event(event, -1);
  }
}

/* SERVER HANDLERS  */

typedef struct {
  chan_t *chan;
  int socket;
  int client_id;
} HandleArgs;

void *handle_send(void *arg) {
  HandleArgs *hsa = (HandleArgs *)arg;
  chan_t *chan = hsa->chan;
  int client_socket = hsa->socket;

  isConnected = 1;
  Event *msg;
  while (1) {
    chan_recv(chan, (void*)&msg);
    send(client_socket, msg, 1023, 0);

    msg->memberCount--;
    if (msg->memberCount == 0 && !msg->dont_free) {
      // message sent one so we can free the event
      free(msg);
    }
  }

  return NULL;
}

void *handle_recv(void *arg) {

  HandleArgs *hsa = (HandleArgs *)arg;
  chan_t *chan = hsa->chan;
  int client_socket = hsa->socket;
  int client_id = hsa->client_id;

  char buffer[1024];

  while (1) {
    // Receive data from the client
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
      // Client disconnected or error occurred
      if (bytes_received == 0) {
        log_error("Client disconnected\n");
      } else {
        log_error("recv failed");
      }
      break;
    }

    Event *e = parse_event((void *)buffer);
    if (e == NULL) {
      perror("Invalid event received. Ignoring it.");
      continue;
    } else {
      e->dont_free = 1;
      p_handle_event(e, client_id);
      if (isServer) {
        broadcast_event(e, client_id);
      }
    }
  }

  // Close the client socket
  close(client_socket);
  log_error("Client connection closed\n");
  return NULL;
}

void *server_main() {
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  // Set SO_REUSEADDR option
  int opt = 1;
  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    perror("setsockopt failed");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(8080);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(server_socket, (struct sockaddr *)&server_addr,
           sizeof(server_addr)) < 0) {
    perror("Bind failed");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  if (listen(server_socket, 5) < 0) {
    perror("Listen failed");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  log_info("Server is listening on port 8080...\n");

  isConnected = 1;
  while (1) {
    int client_socket = accept(server_socket, NULL, NULL);
    if (client_socket < 0) {
      perror("Accept failed");
      continue; // Continue to accept other connections
    }

    log_info("New client connected\n");
    clients[connectedClients] = client_socket;
    clientsChan[connectedClients] = chan_init(0);
    HandleArgs *hsa = malloc(sizeof(HandleArgs));
    hsa->chan = clientsChan[connectedClients];
    hsa->socket = clients[connectedClients];
    hsa->client_id = connectedClients;

    pthread_t thread;
    if (pthread_create(&thread, NULL, handle_recv, (void *)hsa) != 0) {
      perror("Thread creation failed");
      close(client_socket); // Close the client socket if thread creation fails
      continue;
    }
    pthread_detach(thread); // Detach the thread to handle its own cleanup
    if (pthread_create(&thread, NULL, handle_send, (void *)hsa) != 0) {
      perror("Thread creation failed");
      close(client_socket); // Close the client socket if thread creation fails
      continue;
    }

    pthread_detach(thread); // Detach the thread to handle its own cleanup
    connectedClients++;
  }

  close(server_socket);
}

int p_start_server() {
  isServer = 1;
  pthread_t thread;
  if (pthread_create(&thread, NULL, server_main, NULL) != 0) {
    perror("Server thread creation failed");
    return -1;
  }

  pthread_detach(thread); // Detach the thread to handle its own cleanup
  return 0;
}

/* CLIENT HANDLING */

void *client_main(void *data) {
  int sockfd, connfd;
  struct sockaddr_in servaddr, cli;

  // socket create and verification
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    log_error("socket creation failed...\n");
    exit(0);
  } else
    log_info("Socket successfully created..\n");
  bzero(&servaddr, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  printf("Trying to connect %s on port %s\n", world.serverAddress, world.serverPort);
  servaddr.sin_addr.s_addr = inet_addr(world.serverAddress);
  servaddr.sin_port = htons(atoi(world.serverPort));

  if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
    log_error("connection with the server failed...\n");
    isConnected = -1;
    return NULL;
  } else
    log_info("connected to the server..\n");

  HandleArgs *hsa = malloc(sizeof(HandleArgs));
  ss_chan = chan_init(0);
  hsa->chan = ss_chan;
  hsa->socket = sockfd;
  hsa->client_id = -1;

  pthread_t thread;
  if (pthread_create(&thread, NULL, handle_recv, (void *)hsa) != 0) {
    perror("Thread creation failed");
    close(sockfd); // Close the client socket if thread creation fails
    exit(0);
  }
  pthread_detach(thread); // Detach the thread to handle its own cleanup
  if (pthread_create(&thread, NULL, handle_send, (void *)hsa) != 0) {
    perror("Thread creation failed");
    close(sockfd); // Close the client socket if thread creation fails
    exit(0);
  }

  pthread_detach(thread); // Detach the thread to handle its own cleanup

  // close(sockfd);
  return 0;
}

int p_start_client() {

  pthread_t thread;
  if (pthread_create(&thread, NULL, client_main, NULL) != 0) {
    perror("Client thread creation failed");
    return -1;
  }

  pthread_detach(thread); // Detach the thread to handle its own cleanup
  return 0;
}

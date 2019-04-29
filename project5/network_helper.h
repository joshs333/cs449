#ifndef NETWORK_HELPER_H
#define NETWORK_HELPER_H
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include "pthread_list.h"

#ifdef _REENTRANT
  #include <pthread.h>
#endif

#define PORT 50822
#define MAX_CONNECTIONS 10

struct NET_DATA {
  int socket_descriptor;
};

struct user_data_t {
  int connection_descriptor;
  void* user_data;
};

struct SPAWN {
  void (*user_function)(struct user_data_t*);
  int connection_descriptor;
  struct pthread_link_t* thread_link;
  void* user_data;
};

enum NETWORK_ERROR {
  NET_ERROR_NONE,
  NET_ERROR_LISTENING,
  NET_ERROR_ACCEPTING,
  NET_ERROR_BINDING,
  NET_ERROR_SOCKET,
  NET_ERROR_COUNT
};

static char* NETWOR_ERROR_STRING[] = {
  "No network error",
  "Error listening",
  "Error accepting",
  "Error binding",
  "Error making socket",
  "Invalid network error"
};

int create_bound_socket(struct NET_DATA* net_data);
int thread_spawner(void (*func)(struct user_data_t*), void* args, struct NET_DATA* net_data);
void stop_server();
void speak();
void unspeak();
int flagged_to_stop();

#endif

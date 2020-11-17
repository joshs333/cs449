#include "network_helper.h"

pthread_mutex_t stop_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t connections_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;
int stop_flag = 0;
int connections = 0;

void stop_server() {
  pthread_mutex_lock(&stop_mutex);
  stop_flag = 1;
  pthread_mutex_unlock(&stop_mutex);
}

void speak() {
  pthread_mutex_lock(&print_mutex);
}

void unspeak() {
  pthread_mutex_unlock(&print_mutex);
}

int flagged_to_stop() {
  int result = 0;
  pthread_mutex_lock(&stop_mutex);
  result = stop_flag;
  pthread_mutex_unlock(&stop_mutex);
  return result;
}

void* run_user_wrapper( void* m_data) {
  struct SPAWN* data = (struct SPAWN*) m_data;
  pthread_mutex_lock(&connections_mutex);
  ++connections;
  pthread_mutex_unlock(&connections_mutex);
  struct user_data_t user_data;
  user_data.connection_descriptor = data->connection_descriptor;
  user_data.user_data = data->user_data;
  (*data->user_function)(&user_data);
  close(data->connection_descriptor);
  mark_joinable(data->thread_link);
  free(data);
  pthread_mutex_lock(&connections_mutex);
  --connections;
  pthread_mutex_unlock(&connections_mutex);
  return NULL;
}

int create_socket(struct NET_DATA* net_data) {
    net_data->socket_descriptor = socket(PF_INET, SOCK_STREAM, 0);
    if(net_data->socket_descriptor < 0) {
        printf("Error: Unable to make socket!\n");
        return NET_ERROR_SOCKET;
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons( PORT );
    addr.sin_addr.s_addr = INADDR_ANY; //automatically find IP
    if(bind(net_data->socket_descriptor, (struct sockaddr *)&addr, sizeof(addr)) < 0 ) {
        close(net_data->socket_descriptor);
        return NET_ERROR_BINDING;
    }
    return NET_ERROR_NONE;
}

int thread_spawner(void (*func)(struct user_data_t*), void* args, struct NET_DATA* net_data) {
  if(listen(net_data->socket_descriptor, 10) < 0) {
      printf("Error listening!\n");
      close(net_data->socket_descriptor);
      return NET_ERROR_LISTENING;
  }
  while(!flagged_to_stop()) {
    if(connections >= MAX_CONNECTIONS) {
      speak();
      printf("Stopping because at max connections %d\n", MAX_CONNECTIONS);
      unspeak();
    }
    int connfd = accept(net_data->socket_descriptor, NULL, NULL);
    if(connfd < 0) {
      close(net_data->socket_descriptor);
      printf("Error accepting\n");
      return NET_ERROR_ACCEPTING;
    } else {
      speak();
      printf("Accepted a connection!");
      unspeak();
      struct pthread_link_t *t_data = get_pthread();
      struct SPAWN *s_data = malloc(sizeof(struct SPAWN));
      s_data->user_function = func;
      s_data->connection_descriptor = connfd;
      s_data->thread_link = t_data;
      s_data->user_data = args;
      pthread_create(&t_data->data, NULL, &run_user_wrapper, (void *)s_data);
    }
  }
  return NET_ERROR_NONE;
}

int main(int argc, char** argv) {
  return 1;
}

#include "pthread_list.h"

struct pthread_link_t* head = NULL;

struct pthread_link_t* get_pthread() {
  // base case of no head
  if(head == NULL) {
    head = malloc(sizeof(struct pthread_link_t));
    memset(head, 0, sizeof(struct pthread_link_t));
    head->next = NULL;
    head->joined = 1;
    head->joinable = 0;
  }
  // find next empty pthread link
  struct pthread_link_t* current = head;
  while(current->joined == 0 && current->next != NULL) {
    current = current->next;
  }
  // we finish traversing the list and the one we have is not joined then
  // we are at the end and need to append another
  if(current->joined == 0) {
    current->next = malloc(sizeof(struct pthread_link_t));
    memset(current, 0, sizeof(struct pthread_link_t));
    current = current->next;
    current->joined = 1;
    current->next = NULL;
    current->joinable = 0;
  }
  // by now current is a node that is joined
  // so we unjoin it and return the pthread
  memset(current, 0, sizeof(struct pthread_link_t));
  return current;
}

void free_all() {
  join_all();
  struct pthread_link_t* current = head;
  while(current != NULL) {
    struct pthread_link_t* freeable = current;
    current = current->next;
    free(current);
  }
  // we just freed all so we will set head to NULL and return to base case
  head = NULL;
  return;
}

void join_all() {
  struct pthread_link_t* current = head;
  while(current != NULL) {
    join(current);
    current = current->next;
  }
  return;
}

void check_joins() {
  struct pthread_link_t* current = head;
  while(current != NULL) {
    if(current->joinable == 1) {
      join(current);
    }
    current = current->next;
  }
  return;
}

void mark_joinable(struct pthread_link_t* link) {
  if(link != NULL) {
    link->joinable = 1;
  }
}

void join(struct pthread_link_t* link) {
  if(link->joined == 1) {
    return;
  }
  pthread_join(link->data, NULL);
  link->joined = 1;
  return;
}

int count() {
  int count = 0;
  struct pthread_link_t* current = head;
  while(current != NULL) {
    ++count;
    current = current->next;
  }
  return count;
}

#ifndef PTHREAD_LIST_H
#define PTHREAD_LIST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#ifdef _REENTRANT
  #include <pthread.h>
#endif

struct pthread_link_t {
  struct pthread_link_t* next;
  int joined;
  int joinable;
  pthread_t data;
};

struct pthread_link_t* get_pthread();
void free_all();
void join_all();
void join(struct pthread_link_t* link);
int count();



#endif

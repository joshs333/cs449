#include <stdio.h>

#ifdef _REENTRANT
// 1. Include header file with pthread function declarations
#include <pthread.h>
// 2. Define mutex variable for locking (Part 2)
pthread_mutex_t global_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

#define MAX 10000000

int global = 0;

void *foo(void *p) {
  int i;
  int local = 0;
  for(i = 0; i < MAX; i++) {
    if(local % 200 == 0) {
      pthread_mutex_lock(&global_mutex);
      global++;
      pthread_mutex_unlock(&global_mutex);
    }
    local++;
  }
  pthread_mutex_lock(&global_mutex);
  printf("local=%d, global=%d\n", local, global);
  pthread_mutex_unlock(&global_mutex);
  return  NULL;
}

int main()
{
#ifdef _REENTRANT
  // 1. Declare two pthread_t variables to store thread IDs
  pthread_t thread_1;
  pthread_t thread_2;
  // 2. Create two threads using pthread_create() using above variables
  pthread_create(&thread_1, NULL, &foo, NULL);
  pthread_create(&thread_2, NULL, &foo, NULL);
  // 3. Wait for the two threads to finish before returning using pthread_join()
  pthread_join(thread_1, NULL);
  pthread_join(thread_2, NULL);
#else
  foo(NULL);
  foo(NULL);
#endif
  return 0;
}

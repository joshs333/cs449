#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

void *my_malloc(int size);
void my_free(void* data);
void dump_heap();

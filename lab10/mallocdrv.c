#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "malloc.h"
#include "tree.h"
//include your code

//define DUMP_HEAP() to be the dump_heap() function that you write
#define DUMP_HEAP()

//Whether to turn on verbose heap debug output
#define HEAP_DEBUG

//You can adjust how many things are allocated
#define TIMES 100

//If you want to make times bigger than 100, remove the call to qsort and do something else.
//Then remove this check.
#if TIMES >= 1000
	#error "TIMES is too big, qsort will switch to mergesort which requires a temporary malloc()ed array"
#endif

void test1()
{
	int i;
 	
	struct tree *head = (struct tree *)MALLOC(sizeof(struct tree));
	head->data = 0;
	head->left = NULL;
	head->right = NULL;

	for(i=1;i<TIMES;i++)
	{
		struct tree *new = (struct tree *)MALLOC(sizeof(struct tree));
		new->data = i;
		new->left = NULL;
		new->right = NULL;
		randominsert(head, new);
	}

#ifdef HEAP_DEBUG
        printf("heap content after test1:\n");
        DUMP_HEAP();
#endif

	printtree(head);
	freetree(head);

#ifdef HEAP_DEBUG
        printf("heap content after test1 free:\n");
        DUMP_HEAP();
#endif
}

int comp(const void *a, const void *b)
{
	return *((int *)a) - *((int *)b);
}

void test2()
{
	int *a;
	int i;

	a = (int *)MALLOC(TIMES * sizeof(int));

	for(i=0;i<TIMES;i++)
	{
		a[i] = rand()%TIMES + 1;
	}

	qsort(a, TIMES, sizeof(int), comp);

#ifdef HEAP_DEBUG
        printf("heap content after test2:\n");
        DUMP_HEAP();
#endif

	for(i=0;i<TIMES;i++)
	{
		printf("%d\n", a[i]);
	}

	FREE(a);

#ifdef HEAP_DEBUG
        printf("heap content after test2 free:\n");
        DUMP_HEAP();
#endif
}

int main() 
{
	srand((unsigned int)time(NULL));
#ifdef HEAP_DEBUG
        printf("heap content at beginning:\n");
        DUMP_HEAP();
#endif
	test1();
	test2();
	return 0;
}

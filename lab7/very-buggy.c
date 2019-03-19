#include <stdlib.h>
#include <stdio.h>

// removed since it is no longer used
// to prevent compiler errors from the warning
//static void* return_arg(void* p);
int frame3 ( void )
{
  // increase malloc size to prevent buffer overflow
  int *a = malloc(11 * sizeof(int));
  int i = 0;
  // Initialise all values in a to 42 (to make the answer 42 :)
  // need to do this to remove uninitialised value error
  while(i < 11) {
    a[i] = 42;
    ++i;
  }
  int n = a[10];

  if (a[5] == 42) {
    printf("hello from frame3().  The answer is 42.\n");
  } else {
    printf("hello from frame3().  The answer is not 42.\n");
  }

  n = a[  a[0] & 7  ];

  free(a);
  // illegal frees was here
  //free(a) - a was already freed
  //free(return_arg(&n)) - attempts to free n which is in the stack
  //                       not to mention we need that value to return

  a = malloc(99 * sizeof(int));
  // added free statement
  free(a); // - need to free a to prevent memory leak

  return n;
}

int frame2 ( void )
{
  return frame3() - 1;
}

int frame1 ( void )
{
  return frame2() + 1;
}

int main ( void )
{
  return frame1() - 1;
}

// removed to prevent errors from -Wall -Werror
// since this is no longer used
//static void* return_arg(void* p)
//{
//   return p;
//}


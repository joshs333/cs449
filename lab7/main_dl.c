#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

int main() {
  void *handle;
  void (*my_str_copy)(char *, char *); char *error;
  void (*my_str_cat)(char*, char*);
  handle = dlopen("libmystr.so", RTLD_LAZY);
  if(!handle) { //handle == NULL
    printf("%s\n", dlerror()); //dlerror gives us a string with the error
    exit(1);
  }
  dlerror(); // Clear any existing error
  my_str_copy = dlsym(handle, "my_strcpy"); //lookup the function by name
  my_str_cat = dlsym(handle, "my_strcat"); // lookup the cat function
  if ((error = dlerror()) != NULL) {
    printf ("%s\n", error);
    exit(1);
  }
  //Let's test it.
  char dest[100];
  char *src = "Hello World!";
  char *cat = "Hello Again!";
  my_str_copy(dest, src);
  my_str_cat(dest, cat);
  printf ("%s\n", dest);
  dlclose(handle);
  return 0;
}

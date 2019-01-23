
/**
 * File: Lab2a.c
 * Author: Joshua Spisak (joshs333@live.com)
 *
 * Requirements:
 * - Print size of types listed below
 *
 * Date: 1/23/2019
 **/
#include <stdio.h>

/**
 * print_size()
 * 
 * Inputs:
 *     char* type: name of type
 *     int size: size of said type
 * 
 * Notes: I split this into a function so I could easily change the format.
 *     without having to change 9 more lines of code multiple times.
 **/
void print_size(char* type, int size) {
    printf("%-12s %d bytes\n", type, size);
}


int main() {
    // print sizes of strings as specified in lab document
    print_size("int", sizeof(int));    
    print_size("short", sizeof(short));
    print_size("long", sizeof(long));
    print_size("long long", sizeof(long long));
    print_size("unsigned int", sizeof(unsigned int));
    print_size("char", sizeof(char));
    print_size("float", sizeof(float));
    print_size("double", sizeof(double));
    print_size("long double", sizeof(long double));
    return 0;
}

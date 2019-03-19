/**
 * @author jjs231@pitt.edu
 * @file mystr.c a simple implementation of the strcpy and strcat function
 **/
#include <stdio.h>

/**
 * @brief my_strcpy() copies the string at src to dest
 * 
 * @param [in] dest a pointer to the char array to copy into
 * @param [in] dest a pointer to the char array to copy from
 **/
void my_strcpy(char* dest, char* src) {
    while(*dest++ = *src++);
}

/**
 * @brief my_strcat() concatentates a string to the end of an existing string
 * 
 * @param [in] str1 a pointer to the str array that will be contatenated to
 * @param [in] str2 a pointer to the str array that will be the contactended to str1
 **/
void my_strcat(char* str1, char* str2) {
    while(*str1 != 0) str1++; // traverse to end of str1
    while(*str1++ = *str2++); // append str2 to end of str1

    /* NOTE: typically I don't enjoy terse line such as these because they are
             are often cryptic. But since the assignment started it by giving
             a strcpy implementation in this format, I'll follow suite.
    */
}

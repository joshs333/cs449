/**
 * File: search.c
 * Author: Joshua Spisak (joshs333@live.com)
 *
 * Requirements:
 * - Takes the searched-for-value (or the needle) as a command line argument
 * - Generate an integer array of length 10 that contains the powers of 2 starting from 1
 *     (identical to the array shown in an above example)
 * - Implement a recursive function with the following prototype:
 *     int* search(int* begin, int* end, int needle);
 * - Use the above recursive function to search for the needle value, and if found print
 *     the Index in the array where the value is found.
 * - Use pointer arithmetic to imlement the above function. You can use the subscript
 *     operator (the [] operator) as an alternative but you will find using pointer
 *     arithmetic more expressive and intuitive.
 *
 * Date: 1/23/2019
 **/
#include<stdio.h>
#include<stdlib.h>

/**
 * search()
 * 
 * Inputs:
 *     int* begin: pointer to first value of array.
 *     int* end: pointer to last value of array.
 *     int needle: value in array to find
 *
 * Finds needle in an array starting at begin and ending at end.
 * Returns pointer to needle in array, else returns 0.
 **/
int* search(int* begin, int* end, int needle);

int main(int argc, char** argv) {
    int num;
    int nums[10], i;
    int* found = NULL;

    if(argc != 2) {
        printf("Usage: search <number>\n");
        return 1;
    }
    num = atoi(argv[1]);
    
    for(i = 0; i < 10; i++) {
        nums[i] = 1 << i;
    }

    found = search(nums, &nums[9], num);
    if(found) {
        printf("Number %d found in index %ld.\n", num, found - nums);
    } else {
        printf("Number %d was not found.\n", num);
    }
    return 0;
}

int* search(int* begin, int* end, int needle) {
    int* middle = begin + (end - begin)/2;
    //check middle
    if(*middle == needle)
        return middle;
    //middle has reached the begining
    //note that middle cannot == end
    //because int division floors to begin
    //before it can equal end
    if(middle == begin) {
        //check last value
        if(*end == needle)
            return end;
        return NULL;
    }
    // if greater than  middle search above middle
    if(needle > *middle)
       	return search(middle + 1, end, needle);
    // if less than middle search below middle
    if(needle < *middle)
       	return search(begin, middle - 1, needle);
    // if we somehow get here... exit safely
    return NULL;
}

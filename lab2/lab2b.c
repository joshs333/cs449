/**
 * File: Lab2b.c
 * Author: Joshua Spisak (joshs333@live.com)
 * 
 * Requirements:
 * - Asks the user to input a weight (on Earth)
 * - Displays that weight scaled appropriately on each of the other 7 planets
 * 
 * Date: 1/23/2019
 **/
#include <stdio.h>

//number of planets
static int planet_count = 7;

//indices of planet name in c-string format
static char* planets[] = {
    "Mercury",
    "Venus",
    "Mars",
    "Jupiter",
    "Saturn",
    "Uranus",
    "Neptune"
};

// indices of relative gravitational strength
static double weight_multiplier[] = {
    0.38,
    0.91,
    0.38,
    2.54,
    1.08,
    0.91,
    1.19
};

/**
 * print_planet()
 * Inputs:
 *   int planet_id: index of planet in indices
 *   int user_weight: weigth of user
 * 
 *  Prints users weigth on the planet referenced by planet_id
 **/
void print_planet(int planet_id, int user_weight) {
    if(planet_id > planet_count)
        return;

    printf("%s\t%d lbs\n", planets[planet_id], (int) (weight_multiplier[planet_id]*user_weight));
}


int main() {
    int user_weight = 0;

    // Input User weight
    printf("Please enter the weight on earth: ");
    scanf("%d", &user_weight);
    printf("Here is your weight on other planets:\n\n");

    // Print user weight for all planets
    int i = -1;
    while(++i < planet_count)
        print_planet(i, user_weight);

    // extra line at end, return
    printf("\n");   
    return 0;
}
    

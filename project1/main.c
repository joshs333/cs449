#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "game.h"

void print_splash() {
    printf("            / \\   |           \n");
    printf("            | |   ^           \n");
    printf("           /    /             \n");
    printf("           ||  |              \n");
    printf("            //\\\\            \n");
    printf("            ||||              \n");
    printf("  _    __  _______   __    __      ______          \n");
    printf(" | |  / / |__   __| |  \\   | |    / ____ \\                               \n");
    printf(" | | / /     | |    |   \\  | |   / /    \\_\\                       \n");
    printf(" | |/ /      | |    | |\\ \\ | |  / /    ______                                 \n");
    printf(" | |\\ \\      | |    | | \\ \\| |  \\ \\   |__  __|                       \n");
    printf(" | | \\ \\   __| |__  | |  \\   |   \\ \\____/ /                        \n");
    printf(" |_|  \\_\\ |______|  |_|   \\__|    \\______/                      \n");
    printf("  ________       __         _         _          \n");
    printf(" |  ______|     /  \\       | |       | |                 \n");
    printf(" | |______     / /\\ \\      | |       | |               \n");
    printf(" |  ______|   / /__\\ \\     | |       | |                       \n");
    printf(" | |         / _____  \\    | |       | |                \n");
    printf(" | |        / /     \\  \\   | |____   | |____                \n");
    printf(" |_|       / /       \\  \\  |______|  |______|                 \n");
    return;
}

void print_armour() {
    int i = 0;
    // Generates a format string based on the longest length of the armour string
    char format_string1[40];
    sprintf(format_string1, "[idx] %%-%ds : Damage\n", Armour_String_Length - 2);
    printf(format_string1, "Armour");

    // prints a buffer of === between the header and the table of armour
    for(i = 0; i < strlen(format_string1) + Armour_String_Length - 7; ++i)
        printf("=");
    printf("\n");

    // generate another formatted string and print weapons table
    char format_string2[25];
    sprintf(format_string2, "[%%d] %%-%ds : %%d\n", Armour_String_Length);
    for(i = 0; i < AM_COUNT; ++i)
        printf(format_string2, i, Armour_String[i], Armour_Class[i]);

    return;
}

void print_weapons() {
    int i = 0;
    // Generates a format string based on the longest length of the weapon string
    char format_string1[40];
    sprintf(format_string1, "[idx] %%-%ds : Damage\n", Weapon_String_Length - 2);
    printf(format_string1, "Weapon");

    // prints a buffer of === between the header and the table of weapons
    for(i = 0; i < strlen(format_string1) + Weapon_String_Length - 7; ++i)
        printf("=");
    printf("\n");

    // generate another formatted string and print weapons table
    char format_string2[25];
    sprintf(format_string2, "[%%d] %%-%ds : %%dd%%d\n", Weapon_String_Length);
    for(i = 0; i < AM_COUNT; ++i)
        printf(format_string2, i, Weapon_String[i], Weapon_Die[i][0], Weapon_Die[i][1]);

    return;
}

void print_players(struct player* players, int player_count) {
    //generate format string based on length of the armour string
    char format_string1[40];
    sprintf(format_string1, "Player : Health : %%-%ds : Weapon\n", Armour_String_Length);
    printf(format_string1, "Armour");

    //print a buffer of === between the header and the table data
    int i;
    for(i = 0; i < strlen(format_string1) + Armour_String_Length; ++i)
        printf("=");
    printf("\n");

    //generate format string for table data and print data
    char format_string2[25];
    sprintf(format_string2, "%%-6d : %%-6d : %%-%ds : %%-%ds\n", Armour_String_Length, Weapon_String_Length);
    for(i = 0; i < player_count; ++i) {
        printf(format_string2, players[i].id, players[i].health, Armour_String[players[i].armour], Weapon_String[players[i].weapon]);
    }
}

void print_player(struct player* player_) {
    printf("[Player %d, Health: %d, Armour: %s, Weapon: %s]\n", (*player_).id, (*player_).health, Armour_String[(*player_).armour], Weapon_String[(*player_).weapon]);
}

/**
 * safely_get_int
 * input:
 *   char* request: string to print for	user before scanning for integer
 *   int lower_bound: low bound for valid integer value (inclusive)
 *   int higher_bound: high bound for valid integer value (exclusive)
 *   int* value: pointer to location to return	value
 *   int retries: number of tries for user to enter valid integer before failing
 * returns:
 *   int: 0 if fails to gets character, 1 if successful
 *
 * about:
 *   safely gets an integer from a user, checks it is within bound. If bounds
 *   are the same then it does not check bounds and simply gets any integer
 *   value.
 **/
int safely_get_int(char* request, int lower_bound, int higher_bound, int* value, int retries) {
    int ch;

    // gets an int from scanf safely (checking bounds && input)
    do {
        // get a valid value
        printf("%s", request);
        if(scanf("%d", value)) {
            // if bounds are the same assume no bound
            if(lower_bound == higher_bound) {
                //clear buffer
                while ((ch = getchar()) != '\n' && ch != EOF)
                    continue;
                return 1; // return
            } else {
                //check bounds
                if(*value >= lower_bound && *value < higher_bound) {
                    // clear buffer
                    while ((ch = getchar()) != '\n' && ch != EOF)
                        continue;
                    return 1; // return if valid
                }
            }
        }
        // if we reach here we did not pass valid condition test
        --retries;
        if(retries) {
            //print error only if more retries. Nested if for form of try
            printf("[Error] Please enter an integer from %d to %d inclusive. (%d more %s)\n", lower_bound, higher_bound - 1, retries, retries > 1 ? "tries" : "try");

        }
        // clear buffer for next input
        while ((ch = getchar()) != '\n' && ch != EOF)
            continue;

    } while(retries > 0);
    //we got here so we never got a good value
    return 0;
}

/**
 * safely_get_char
 * input:
 *   char* request: string to print for	user before scanning for char
 *   char* value: pointer to location to return	value
 *   int retries: number of tries for user to enter char before	failing
 * returns:
 *   int: 0 if fails to gets character, 1 if successful
 *
 * about:
 *   safely gets a character from the user, clears the buffer
 *   there is no way that there	will be	a failure in getting a char
 *   but whatever
 **/
int safely_get_char(char* request, char* value, int retries) {
    // gets an int from scanf safely (checking bounds && input)
    do {
        int ch;
        // get a valid value
        char buffer[255];
        printf("%s", request);
        if(scanf("%s", buffer)) {
            *value = buffer[0];
            while ((ch = getchar()) != '\n' && ch != EOF)
                continue;
            return 1;
        }
        // if we reach here we did not pass valid condition test
        --retries;
        if(retries) {
            //print error only if more retries. Nested if for form of try
            printf("[Error] Please enter a valid character. (%d more %s)\n", retries, retries > 1 ? "tries" : "try");
        }
        while ((ch = getchar()) != '\n' && ch != EOF)
            continue;

    } while(retries > 0);
    //we got here so we never got a good value
    return 0;
}

void run_round(struct player* player_1, struct player* player_2) {
    int attack_1 = rand() % (20) + 1;
    if(attack_1 >= Armour_Class[(*player_2).armour]) {
        int die;
        int damage = 0;
        for(die = 0; die < Weapon_Die[(*player_1).weapon][0]; ++die) {
            damage += rand() % Weapon_Die[(*player_1).weapon][1] + 1;
        }
        printf("Player %d inflicts %d damage on player %d. (attack roll %d)\n", (*player_1).id, damage, (*player_2).id, attack_1);
        (*player_2).health -= damage;
    } else {
        printf("Player %d misses player %d. (attack roll %d)\n", (*player_1).id, (*player_2).id, attack_1);
    }

    int attack_2 = rand() % (20) + 1;
    if(attack_2 >= Armour_Class[(*player_1).armour]) {
        int die;
        int damage = 0;
        for(die = 0; die < Weapon_Die[(*player_2).weapon][0]; ++die) {
            damage += rand() % Weapon_Die[(*player_2).weapon][1] + 1;
        }
        printf("Player %d inflicts %d damage on player %d. (attack roll %d)\n", (*player_2).id, damage, (*player_1).id, attack_2);
        (*player_1).health -= damage;
    } else {
        printf("Player %d misses player %d. (attack roll %d)\n", (*player_2).id, (*player_1).id, attack_2);
    }
}

int run_match(struct player* player_1, struct player* player_2) {
    char request_string[40];
    char y_n_char;
    sprintf(request_string, "Players %d and %d, Ready to fight? (Y/N): ", (*player_1).id, (*player_2).id);
    if(!safely_get_char(request_string, &y_n_char, 3)) {
        printf("Unable to get confirmation. Exitting.\n");
        return 0;
    }
    if(y_n_char != 'Y' && y_n_char!= 'y') {
        printf("Well.. you aren't ready to fight.. fine. Goodbye!\n");
        return 0;
    }

    int round = 1;
    while(1) {
        printf("************************ Round %d ************************\n", round);
        run_round(player_1, player_2);
        printf("\n");
        if((*player_1).health <= 0 || (*player_2).health <= 0) {
            return 1;
        } else {
            print_player(player_1);
            print_player(player_2);
            printf("\n");
        }

        ++round;
    }
}

int main(int argc, char** argv) {
    int player_count = 2;
    srand((unsigned int)time(NULL));

    printf("Greetings adventurer!\n");
    print_splash();

    if(!safely_get_int("Enter number of players: ", 1,11, &player_count, 3)) {
        printf("Unable to get player count. Exiting.\n");
        return 1;
    }
    player_count = 2;
    struct player players[player_count];

    printf("\nPlayers, select your armour!\n");
    print_armour();
    printf("\n");
    int i;
    char am_select_string[25];
    for(i = 0; i < player_count; ++i) {
        int selection = 0;
        sprintf(am_select_string, "Player %d, Select armour(0~%d): ", i + 1, AM_COUNT - 1);
        if(!safely_get_int(am_select_string, 0, AM_COUNT, &selection, 3)) {
            printf("Unable to get armour selection. Exiting.\n");
            return 1;
        }
        // make weapon selection and init other player variables
        players[i].id = i + 1;
        players[i].health = 20;
        players[i].armour = selection;
    }

    printf("\nPlayers, select your weapon!\n");
    print_weapons();
    printf("\n");
    char wp_select_string[25];
    for(i = 0; i < player_count; ++i) {
        int selection = 0;
        sprintf(wp_select_string, "Player %d, Select weapon(0~%d): ", i + 1, WP_COUNT - 1);
        if(!safely_get_int(wp_select_string, 0, WP_COUNT, &selection, 3)) {
            printf("Unable to get weapon selection. Exiting.\n");
            return 1;
        }
        players[i].weapon = selection;
    }

    print_players(players, player_count);

    if(!run_match(&players[0], &players[1])) {
        printf("Error in match between player %d and %d. Exitting.", players[0].id, players[1].id);
        return 0;
    }

    if(players[0].health <= 0 && players[1].health <= 0) {
        printf("Draw between player %d [Health: %d] and player %d [Health: %d].\n", players[0].id, players[0].health, players[1].id, players[1].health);
    } else if(players[0].health <= 0) {
        printf("Player %d [Health: %d] beats player %d [Health: %d].\n", players[1].id, players[1].health, players[0].id, players[0].health);
    } else if(players[1].health <= 0) {
        printf("Player %d [Health: %d] beats player %d [Health: %d].\n", players[0].id, players[0].health, players[1].id, players[1].health);
    }

    char exit_char;
    char* request_string = "Please enter any character to exit: ";
    safely_get_char(request_string, &exit_char, 1);

    print_splash();
    printf("Play again soon!\n");
    // engage in fights between sets of players 1 with 2    3 with 4
    // into match => for each round that match gets processed if it's active.
    // i need to think more about match style

    return 0;
}

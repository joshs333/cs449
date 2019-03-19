/**
 * File: rpg.c
 * Details: simple command line rpg game
 * 
 * Notes: I made it fancy.. then realized I needed to conform to the autograder.. whoops.
 **/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

/**** Game Structs and static definitions ****/
// struct decribing the player
struct player {
    int id;
    int health;
    int armour;
    int weapon;
};

// enum of armour choices
enum Armours {
    AM_CLOTH = 0,
    AM_LEATHER = 1,
    AM_RING_MAIL = 2,
    AM_CHAIN_MAIL = 3,
    AM_PLATE = 4,
    AM_COUNT = 5
};

// parallel indices to armour choices indicating armour class
static int Armour_Class[] = {
    10, // Cloth
    12, // Leather
    14, // Ring Mail
    16, // Chain Mail
    18 // Plate
};

// length of longest armour string
static int Armour_String_Length = 15;
//parallel indices to armour choices giving printable string
static char* Armour_String[] = {
    "cloth",
    "studded leather",
    "ring mail",
    "chain mail",
    "plate"
};

// enum of available weapons
enum Weapons {
    WP_DAGGER = 0,
    WP_SHORT_SWORD = 1,
    WP_LONG_SWORD = 2,
    WP_GREAT_SWORD = 3,
    WP_AXE = 4,
    WP_COUNT
};

// parallel indices to weapons indicating weapon strength
static int Weapon_Die[5][2] = {
    { 1, 4 }, // Dagger - 1 die of 4
    { 1, 6 }, // Short Sword - 1 die of 6
    { 1, 8 }, // Long Sword - 1 die of 8
    { 2, 6 }, // Great Sword - 2 die of 6
    { 1, 12 } // Axe - 1 die of 12
};

// length of longest weapon string
static int Weapon_String_Length = 11;
// parallel indices to weaponse giving printable strings of each weapon
static char* Weapon_String[] = {
    "dagger",
    "short sword",
    "long sword",
    "great sword",
    "great axe"
};

/**** Game Data Printing functions ****/
/**
 * @brief print_splash() prints a splash screen for the game
 **/
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

/**
 * @brief print_armour() prints armour selections from indices
 **/
void print_armour() {
    int i = 0;
    // Generates a format string based on the longest length of the armour string
    //char format_string1[40];
    //sprintf(format_string1, "[idx] %%-%ds : Damage\n", Armour_String_Length - 2);
    //printf(format_string1, "Armour");
    printf("List of available armors:\n");

    // prints a buffer of === between the header and the table of armour
    //while(i < strlen(format_string1) + Armour_String_Length - 7) {
    //    printf("=");
    //    ++i;
    //}
    //printf("\n");

    // generate another formatted string and print weapons table
    //char format_string2[25];
    //sprintf(format_string2, "[%%d] %%-%ds : %%d\n", Armour_String_Length);
    char* format_string2 = "%d: %s (AC=%d)\n";
    i = 0;
    while(i < AM_COUNT) {
        printf(format_string2, i, Armour_String[i], Armour_Class[i]);
        ++i;
    }

    return;
}

/**
 * @brief print_weapons() prints weapons selections from indices
 **/
void print_weapons() {
    int i = 0;
    // Generates a format string based on the longest length of the weapon string
    //char format_string1[40];
    //sprintf(format_string1, "[idx] %%-%ds : Damage\n", Weapon_String_Length - 2);
    //printf(format_string1, "Weapon");
    printf("List of available weapons:\n");

    // prints a buffer of === between the header and the table of weapons
    //while(i < strlen(format_string1) + Weapon_String_Length - 7) {
    //    printf("=");
    //    ++i;
    //}
    //printf("\n");

    // generate another formatted string and print weapons table
    //char format_string2[25];
    //sprintf(format_string2, "[%%d] %%-%ds : %%dd%%d\n", Weapon_String_Length);
    i = 0;
    char* format_string2 = "%d: %s (damage=%dd%d)\n";
    while(i < AM_COUNT) {
        printf(format_string2, i, Weapon_String[i], Weapon_Die[i][0], Weapon_Die[i][1]);
        ++i;
    }
    return;
}

/**
 * @brief print_players() prints players and their stats
 * @param [in] struct player* players = array of current players in game
 * @param [in] int player_count = number of structs in array
 **/
void print_players(struct player* players, int player_count) {
    //generate format string based on length of the armour string
    char format_string1[40];
    sprintf(format_string1, "Player : Health : %%-%ds : Weapon\n", Armour_String_Length);
    printf(format_string1, "Armour");

    //print a buffer of === between the header and the table data
    int i = 0;
    while(i < strlen(format_string1) + Armour_String_Length) {
        printf("=");
        ++i;
    }
    printf("\n");

    //generate format string for table data and print data
    char format_string2[25];
    sprintf(format_string2, "%%-6d : %%-6d : %%-%ds : %%-%ds\n", Armour_String_Length, Weapon_String_Length);
    i = 0;
    while(i < player_count) {
        printf(format_string2, players[i].id, players[i].health, Armour_String[players[i].armour], Weapon_String[players[i].weapon]);
        ++i;
    }
}

/**
 * @brief print_player() prints a succinct info line about a player
 * @param [in] struct player* player_ = the player to print info about
 **/
void print_player(struct player* player_) {
    printf("[Player %d: hp=%d, armor=%s, weapon=%s]\n", (*player_).id, (*player_).health, Armour_String[(*player_).armour], Weapon_String[(*player_).weapon]);
}

/**
 * @brief safely_get_int() safely gets and integer from the user
 *
 * @param [in] char* request: string to print for	user before scanning for integer
 * @param [in] int lower_bound: low bound for valid integer value (inclusive)
 * @param [in] int higher_bound: high bound for valid integer value (exclusive)
 * @param [in] int* value: pointer to location to return	value
 * @param [in] int retries: number of tries for user to enter valid integer before failing
 *
 * @return int: 0 if fails to gets character, 1 if successful
 *
 * @details safely gets an integer from a user, checks it is within bound. If
 *   bounds are the same then it does not check bounds and simply gets any
 *   integer value.
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
 * @brief safely_get_char() safely gets a char from the user
 *
 * @param [in] char* request: string to print for	user before scanning for char
 * @param [in] char* value: pointer to location to return	value
 * @param [in] int retries: number of tries for user to enter char before	failing
 *
 * @return int: 0 if fails to gets character, 1 if successful
 *
 * @details safely gets a character from the user, clears the buffer. There is
 *   no way that there	will be	a failure in getting a char but whatever.
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

/**
 * @brief run_attack() makes a player attack another
 *
 * @param [in] struct player* player_1 = the attacker
 * @param [in] struct player* player_2 = the player being attacked
 **/
void run_attack(struct player* player_1, struct player* player_2) {
    // attack from player 1 to 2
    int attack = rand() % (20) + 1;
    // if the attack is greater than player 2's
    if(attack >= Armour_Class[(*player_2).armour]) {
        int die = 0;
        int damage = 0;
        // rolls n dies according to weapon strength
        while(die < Weapon_Die[(*player_1).weapon][0]) {
            // rolls a number between 1 and the number specced for that weapon
            damage += rand() % Weapon_Die[(*player_1).weapon][1] + 1;
            ++die;
        }
        // prints results, inflicts damage
        printf("Player %d hits Player %d for %d damage (attack roll %d)\n", (*player_1).id, (*player_2).id, damage, attack);
        (*player_2).health -= damage;
    } else {
        // misses!
        printf("Player %d misses Player %d (attack roll %d)\n", (*player_1).id, (*player_2).id, attack);
    }
    return;
}

/**
 * @brief run_match() runs a full round between two players
 *
 * @param [in] struct player* player_1 = a player in the match
 * @param [in] struct player* player_2 = the other player
 *
 * @return int: 0 if match failed, 1 if match succeeded
 **/
int run_match(struct player* player_1, struct player* player_2) {
    // generates a request string based on player id's
    //char request_string[40];
    char y_n_char;
    //sprintf(request_string, "Players %d and %d, Ready to fight? (Y/N): ", (*player_1).id, (*player_2).id);
    // players agree to play
    char* request_string = "Fight? (y/n):  ";
    if(!safely_get_char(request_string, &y_n_char, 3)) {
        printf("Unable to get confirmation. Exitting.\n");
        return 0;
    }
    if(y_n_char != 'Y' && y_n_char!= 'y') {
        // don't agree... exit round
        // printf("Well.. you aren't ready to fight.. fine. Goodbye!\n");
        return 0;
    }

    int round = 1;
    // players engage in rounds until a player loses
    while(1) {
        printf("----- Round %d ------\n", round);
        run_attack(player_1, player_2);
        run_attack(player_2, player_1);
        print_player(player_1);
        print_player(player_2);

        if((*player_1).health <= 0 || (*player_2).health <= 0) {
            // return a successful round and let main determing what happens
            return 1;
        }

        ++round;
    }
    return 1;
}

void help() { }

int main(int argc, char** argv) {
    unsigned int seed = (unsigned int)time(NULL);
    if(argc != 1 && argc != 3) {
        help();
        return 0;
    }
    if(argc == 3) {
        if(strcmp(argv[1], "-seed") != 0) {
            help();
            return 0;
        }
        seed = (unsigned int)atoi(argv[2]);
    }

    int player_count = 2;
    srand(seed);

    //printf("Greetings adventurer!\n");
    //print_splash();

    //if(!safely_get_int("Enter number of players: ", 1, 11, &player_count, 3)) {
    //    printf("Unable to get player count. Exiting.\n");
    //    return 1;
    //}
    struct player players[player_count];

    //printf("\nPlayers, select your armour!\n");
    print_armour();
    printf("\n");
    int i = 0;
    char am_select_string[40];
    while(i < player_count) {
        int selection = 0;
        sprintf(am_select_string, "Choose Player %d Armor (0~%d): ", i + 1, AM_COUNT - 1);
        if(!safely_get_int(am_select_string, 0, AM_COUNT, &selection, 3)) {
            printf("Unable to get armour selection. Exiting.\n");
            return 1;
        }

        // make weapon selection and init other player variables
        players[i].id = i + 1;
        players[i].health = 20;
        players[i].armour = selection;
        ++i;
    }

    //printf("\nPlayers, select your weapon!\n");
    printf("\n");
    print_weapons();
    printf("\n");
    char wp_select_string[40];
    i = 0;
    while(i < player_count) {
        int selection = 0;
        sprintf(wp_select_string, "Choose Player %d Weapon(0~%d): ", i + 1, WP_COUNT - 1);
        if(!safely_get_int(wp_select_string, 0, WP_COUNT, &selection, 3)) {
            printf("Unable to get weapon selection. Exiting.\n");
            return 1;
        }
        players[i].weapon = selection;
        ++i;
    }

    printf("\nPlayer setup complete:\n");
    i = 0;
    while(i < player_count) {
        print_player(players + i);
        ++i;
    }
    printf("\n");

    //infinite loop until a match exits
    while(1) {
        if(!run_match(&players[0], &players[1])) {
            //printf("Error in match between player %d and %d. Exitting.", players[0].id, players[1].id);
            //match exits, return
            return 0; 
        }

        if(players[0].health <= 0 && players[1].health <= 0) {
            //printf("Draw between player %d [Health: %d] and player %d [Health: %d].\n", players[0].id, players[0].health, players[1].id, players[1].health);
            printf("\nDraw!\n\n"); 
        } else if(players[0].health <= 0) {
            //printf("Player %d [Health: %d] beats player %d [Health: %d].\n", players[1].id, players[1].health, players[0].id, players[0].health);
            printf("\nPlayer %d WINS!\n\n", players[1].id);
        } else if(players[1].health <= 0) {
            //printf("Player %d [Health: %d] beats player %d [Health: %d].\n", players[0].id, players[0].health, players[1].id, players[1].health);
            printf("\nPlayer %d WINS!\n\n", players[0].id);
        }
 
        // reset player health for next match
        players[0].health = 20;
        players[1].health = 20;
    }

    // we will never get here... an artifact of conformation to the grader :P
    char exit_char;
    char* request_string = "Please enter any character to exit: ";
    safely_get_char(request_string, &exit_char, 1);

    print_splash();
    printf("Play again soon!\n");
    // engage in fights between sets of players 1 with 2    3 with 4
    // into match => for each round that match gets processed if it's active.
    // i need to think more about match style
    /*


    */

    return 0;
}

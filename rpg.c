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

/********************* Protected IO Functions *********************************/
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
int safely_get_yn(char* request, char* value, int retries) {
    // gets an int from scanf safely (checking bounds && input)
    do {
        int ch;
        // get a valid value
        char buffer[2];
        printf("%s", request);
        fgets(value, 2, stdin);
        if(*value == 'y' || *value == 'n' || *value == 'Y' || *value == 'N') {
            while ((ch = getchar()) != '\n' && ch != EOF)
                continue;
            return 1;
        }
        // if we reach here we did not pass valid condition test
        --retries;
        if(retries) {
            //print error only if more retries. Nested if for form of try
            printf("[Error] Please enter y/n/Y/N. (%d more %s)\n", retries, retries > 1 ? "tries" : "try");
        }
        while ((ch = getchar()) != '\n' && ch != EOF)
            continue;

    } while(retries > 0);
    //we got here so we never got a good value
    return 0;
}
/********************* Game Structs and static definitions ********************/
enum ENTITY_TYPE {
    ET_USER = 0,
    ET_ORC = 1,
    ET_SAURON = 2,
    ET_GOLLUM = 3,
    ET_COUNT
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

// parallel indices to weaponse giving printable strings of each weapon
static char* Weapon_String[] = {
    "dagger",
    "short sword",
    "long sword",
    "great sword",
    "great axe"
};

// struct decribing the player
struct Entity {
    char name[20];
    enum ENTITY_TYPE type;
    int health;
    int armour;
    int weapon;
    int level;
    int experience;
};

struct GameState {
    int entity_count;
    struct Entity* entities;
    struct Entity user;
};

/********************* Game Data Printing functions ***************************/
/**
 * @brief print_armour() prints armour selections from indices
 **/
void print_armours() {
    printf("List of available armors:\n");

    char* format_string2 = "%d: %s (AC=%d)\n";
    int i = 0;
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
    printf("List of available weapons:\n");

    int i = 0;
    char* format_string2 = "%d: %s (damage=%dd%d)\n";
    while(i < AM_COUNT) {
        printf(format_string2, i, Weapon_String[i], Weapon_Die[i][0], Weapon_Die[i][1]);
        ++i;
    }
    return;
}

/**
 * @brief print_player() prints a succinct info line about a player
 * @param [in] struct player* player_ = the player to print info about
 **/
void print_entity(struct Entity* player_) {
    printf("[%s: hp=%d, armor=%s, weapon=%s, level=%d, xp=%d]\n", player_->name, player_->health, Armour_String[player_->armour], Weapon_String[player_->weapon], player_->level, player_->experience);
}

/********************* Entity Generation Functions ****************************/
int generate_player(struct Entity* player, char* name) {
    printf("What is your name?\n");
    fgets(player->name, 20, stdin);
    int i = 0;
    while(i < 20) {
        if(player->name[i] == '\n')
            player->name[i] = 0;
        ++i;
    }
    printf("\n");
    print_armours();
    printf("\n");
    char am_select_string[40];

    int selection = 0;
    sprintf(am_select_string, "Choose %s's Armour (0~%d): ", player->name, AM_COUNT - 1);
    if(!safely_get_int(am_select_string, 0, AM_COUNT, &selection, 3)) {
        printf("Unable to get armour selection. Exiting.\n");
        return 0;
    }
    player->armour = selection;

    //printf("\nPlayers, select your weapon!\n");
    printf("\n");
    print_weapons();
    printf("\n");
    char wp_select_string[40];
    sprintf(wp_select_string, "Choose %s's Weapon(0~%d): ", player->name, WP_COUNT - 1);
    if(!safely_get_int(wp_select_string, 0, WP_COUNT, &selection, 3)) {
        printf("Unable to get weapon selection. Exiting.\n");
        return 0;
    }
    player->weapon = selection;

    // make weapon selection and init other player variables
    player->health = 20;
    player->type = ET_USER;
    player->level = 1;
    player->experience = 2000;

    printf("\nPlayer setting complete:\n");
    print_entity(player);
    printf("\n");
}

void generate_orc(struct Entity* player, int max_level) {
    static int orc_count = 1;
    player->armour = rand() % (AM_COUNT);
    player->weapon = rand() % (WP_COUNT);

    // make weapon selection and init other player variables
    if(max_level < 1) {
        sprintf(player->name, "%s %d", "Orc", orc_count);
        ++orc_count;
        max_level = 1;
    }
    player->health = 20;
    player->type = ET_ORC;
    player->level = rand() % (max_level) + 1;
    player->experience = (1 << (player->level - 1)) * 2000;
}

void generate_sauron(struct Entity* player) {
    player->armour = 4;
    player->weapon = 4;

    // make weapon selection and init other player variables
    strcpy(player->name, "Sauron");
    player->type = ET_SAURON;
    player->health = 115;
    player->level = 20;
    player->experience = 1048576000;
}

void generate_gollum(struct Entity* player) {
    player->armour = 0;
    player->weapon = 0;

    // make weapon selection and init other player variables
    strcpy(player->name, "Gollum");
    player->type = ET_GOLLUM;
    player->health = 10;
    player->level = 1;
    player->experience = 2000;
}

void regenerate_entity(struct Entity* entity, int user_level) {
    printf("Respawning %s ...\n", entity->name);
    switch(entity->type) {
        case ET_GOLLUM:
            generate_gollum(entity);
            break;
        case ET_SAURON:
            generate_sauron(entity);
            break;
        case ET_ORC:
            generate_orc(entity, user_level);
            break;
        default:
            printf("Error: unknown entity type %d, cannot regenerate.", entity->type);
            break;
    }
}

/********************* Accessory Game Play Functions **************************/
/**
 * @brief run_attack() makes a player attack another
 *
 * @param [in] struct player* player_1 = the attacker
 * @param [in] struct player* player_2 = the player being attacked
 **/
void run_attack(struct Entity* player_1, struct Entity* player_2) {
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
        printf("%s hits %s for %d damage (attack roll %d)\n", (*player_1).name, (*player_2).name, damage, attack);
        (*player_2).health -= damage;
    } else {
        // misses!
        printf("%s misses %s (attack roll %d)\n", (*player_1).name, (*player_2).name, attack);
    }
    return;
}

/********************* Different commands to be run ***************************/
/**
 * @brief runs the fight command
 **/
void command_fight(struct GameState* game, int opponent) {
    while(1) {
        run_attack(&game->entities[opponent], &game->user);
        run_attack(&game->user, &game->entities[opponent]);

        // if the user along or both die, both get respawned boringly
        if(game->user.health <= 0 && game->entities[opponent].health <= 0) {
            printf("\n%s and %s kill each other.\n", game->entities[opponent].name, game->user.name);
            regenerate_entity(&game->entities[opponent], game->user.level);
            printf("Respawing %s ...\n", game->user.name);
            game->user.experience = (1 << (game->user.level - 1)) * 2000;
            game->user.health = (game->user.level - 1) * 5 + 20;
            return;
        } else if(game->user.health <= 0) {
            printf("\n%s is killed by %s.\n", game->user.name, game->entities[opponent].name);
            printf("Respawing %s ...\n", game->user.name);
            game->user.experience = (1 << (game->user.level - 1)) * 2000;
            game->user.health = (game->user.level - 1) * 5 + 20;
            print_entity(&game->user);
            return;
        } else if(game->entities[opponent].health <= 0){
            // opponent is killed
            printf("\n%s is killed by %s.\n", game->entities[opponent].name, game->user.name);
            /* Switch Weapons */
            char ask_string[100];
            sprintf(ask_string, "Get %s's %s, exchanging %s's current armour %s (y/n)? ", game->entities[opponent].name, Armour_String[game->entities[opponent].armour], game->user.name, Armour_String[game->user.armour]);
            char yn_char;
            if(safely_get_yn(ask_string, &yn_char, 3) && yn_char == 'Y' || yn_char == 'y') {
                game->user.armour = game->entities[opponent].armour;
            }
            sprintf(ask_string, "Get %s's %s, exchanging %s's current weapon %s (y/n)? ", game->entities[opponent].name, Weapon_String[game->entities[opponent].weapon], game->user.name, Weapon_String[game->user.weapon]);
            if(safely_get_yn(ask_string, &yn_char, 3) && yn_char == 'Y' || yn_char == 'y') {
                game->user.weapon = game->entities[opponent].weapon;
            }
            /* Level Up User */
            game->user.experience += 2000 * game->entities[opponent].level;
            int prior_level = game->user.level;
            while(game->user.experience >= (1 << (game->user.level)) * 2000) {
                ++game->user.level;
            }
            game->user.health = (game->user.level - 1) * 5 + 20;
            if(game->user.level != prior_level) {
                printf("%s levels up to level %d!\n", game->user.name, game->user.level);
                print_entity(&game->user);
            }
            /* Regenerate Opponent */
            regenerate_entity(&game->entities[opponent], game->user.level);
            print_entity(&game->entities[opponent]);
            return;
        }
    }
    return;
}

/**
 * @brief command_look prints all entities and users
 **/
void command_look(struct GameState *game) {
    printf("All is peaceful in the land of Mordor.\n");
    printf("Sauron and his minions are blissfully going about their business:\n");
    int i = 0;
    while(i < game->entity_count) {
        printf("%d: ", i);
        print_entity(&game->entities[i]);
        ++i;
    }
    printf("Also at the scene are some adventurers looking for trouble:\n");
    printf("0: ");
    print_entity(&game->user);
}

/**
 * @brief command_look prints all entities and users
 **/
void command_stats(struct GameState *game) {
    print_entity(&game->user);
}

int loop(struct GameState* game) {
    printf("Command >> ");
    char command[255];
    fgets(command, 255, stdin);
    // tokenize the read in arguments
    char* argument = strtok(command, " \n");
    if(argument) {
        // perform look command
        if(strcmp(argument, "look") == 0) {
            command_look(game);
        // perform stats command
        } else if(strcmp(argument, "stats") == 0) {
            command_stats(game);
        // perform exit command
        } else if(strcmp(argument, "quit") == 0) {
            return 0;
        // perform fight command
        } else if(strcmp(argument, "fight") == 0) {
            // next token
            char* argument = strtok(NULL, " \n");
            // make sure there is an actual argument there...
            if(argument != NULL) {
                // convert to int and check range
                int entity = atoi(argument);
                if(entity < 0 || entity >= game->entity_count) {
                    printf("Error: please enter entity number within 0 and %d\n", game->entity_count - 1);
                } else {
                    // range checks out, perform command
                    command_fight(game, entity);
                }
            } else {
                printf("Error: command format- fight <entity ID> (find entity ID's with look command)\n");
            }
        } else if(strcmp(argument, "armours") == 0) {
            print_armours();
        } else if(strcmp(argument, "weapons") == 0) {
            print_weapons();
        } else {
            printf("Unknown Command.\n");
        }
    } else {
        printf("No argument given. Error.\n");
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
    srand(seed);

    struct GameState game;
    game.entity_count = 10;
    game.entities = (struct Entity*) malloc(sizeof(struct Entity) * game.entity_count);
    generate_sauron(&game.entities[0]);
    int i = 1;
    while(i < 9) {
        generate_orc(&game.entities[i], -1);
        ++i;
    }
    generate_gollum(&game.entities[i]);
    generate_player(&game.user, "Frodo");
    command_look(&game);

    while(loop(&game));

    // TODO: Save game

    free(game.entities);
    return 0;
}

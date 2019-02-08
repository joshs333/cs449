#ifndef GAME_H
#define GAME_H

struct player {
    int id;
    int health;
    int armour;
    int weapon;
};

enum Armours {
    AM_CLOTH = 0,
    AM_LEATHER = 1,
    AM_RING_MAIL = 2,
    AM_CHAIN_MAIL = 3,
    AM_PLATE = 4,
    AM_COUNT = 5
};

static int Armour_Class[] = {
    10, // Cloth
    12, // Leather
    14, // Ring Mail
    16, // Chain Mail
    18 // Plate
};

static int Armour_String_Length = 15;
static char* Armour_String[] = {
    "cloth",
    "studded leather",
    "ring mail",
    "chain mail",
    "plate"
};

enum Weapons {
    WP_DAGGER = 0,
    WP_SHORT_SWORD = 1,
    WP_LONG_SWORD = 2,
    WP_GREAT_SWORD = 3,
    WP_AXE = 4,
    WP_COUNT
};

static int Weapon_Die[5][2] = {
    { 1, 4 }, // Dagger - 1 die of 4
    { 1, 6 }, // Short Sword - 1 die of 6
    { 1, 9 }, // Long Sword - 1 die of 9
    { 2, 6 }, // Great Sword - 2 die of 6
    { 1, 12 } // Axe - 1 die of 12
};

static int Weapon_String_Length = 11;
static char* Weapon_String[] = {
    "dagger",
    "short sword",
    "long sword",
    "great sword",
    "great axe"
};


#endif // GAME_H

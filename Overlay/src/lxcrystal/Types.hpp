#ifndef TYPES_H
#define TYPES_H

#define PLAYER_COUNT 30

typedef struct
{
    // Spatial information
    float headPos[3];
    float feetPos[3];
    float frontVector[3];

    // Generic information
    unsigned int primaryAmmo;
    unsigned int secondaryAmmo;
    unsigned int health;
    const char name[25];
} Player_t;

#endif
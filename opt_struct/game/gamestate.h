#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <stdbool.h>

typedef struct  {
    const char *name;
    const char *description;
    unsigned    targetIndex[2];
    bool        locked;
} Door;

typedef struct {
    const char *name;
    const char *description;
} Objekt;


typedef struct {
    const char *name;
    const char *description;
} Person;

#define max_exits 4
#define max_items 6
#define max_persons 3

typedef struct {
    const char *tag;
    const char *description;

    const Door   *doors[max_exits];
    unsigned doorCount;

    const Objekt *objects[max_items];
    unsigned objectCount;

    const Person *persons[max_persons];
    unsigned personCount;
    
    unsigned currentProxy;

} Location;


typedef struct  {
    Location *locations;
    unsigned numLocations;
    unsigned currentLocation;
    bool     running;
} GameState;


void gameInit(GameState *gs);
Location *gameCurrentLocation(GameState *gs);


#endif
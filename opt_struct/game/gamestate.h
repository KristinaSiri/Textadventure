#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <stdbool.h>

typedef struct GameState GameState;





typedef void (*ActionFunc)(GameState *gs, void* target, void* token);

typedef enum {
    TYPE_DOOR, 
    TYPE_OBJECT, 
    TYPE_PERSON,
    TYPE_OBJ_COUNT
} ObjektType;

typedef enum {
    ITM_TYPE_TEXT,
    ITM_TYPE_COUNT
} ItemType;

typedef struct {
    const char *name;
    const char *description;

    ItemType itemType;
    union {
        char *text
    };

} Item;



typedef struct {
    char *name;
    ActionFunc execute;
    void* token;

} Action;

typedef struct {
    char *name;
    bool locked;

    Item *items[3];
    int itemCount

} Compartment;


typedef struct  {
    const char *name;
    const char *description;

    unsigned    targetIndex[2];
    bool        locked;

    Action      actions[3];
    unsigned    actionCount;
} Door;

typedef struct {
    const char *name;
    const char *description;

    Action      actions[3];
    unsigned    actionCount;

    Compartment *compartmens[3];
    unsigned    compaCount;
} Objekt;


typedef struct {
    const char *name;
    const char *description;

    Action      actions[3];
    unsigned    actionCount;
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


typedef struct GameState {
    Location *locations;
    unsigned numLocations;
    unsigned currentLocation;
    bool     running;
};


void gameInit(GameState *gs);
Location *gameCurrentLocation(GameState *gs);


#endif
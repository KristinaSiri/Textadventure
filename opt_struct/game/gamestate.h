#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <stdbool.h>

typedef struct GameState GameState;







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
    ObjektType  *prxyTyp;
    void*       Tkns[3];

} Token;

typedef void (*ActionFunc)(GameState *gs, void* prxy, Token* token);


typedef struct {
    char        *name;
    ActionFunc  execute;
    Token       token;

} Action;


typedef struct {
    
    int     level;
    Action  satActions[3];
    int     satActionCount;

} Satisfier;

typedef struct {
    const char *name;
    const char *description;

    ItemType    itemType;

    union {
        char    *text;
    };

    Action      itmActions[2];
    unsigned    itmActionCount;

} Item;

typedef struct {
    char    *name;
    bool    locked;

    Item    *items[3];
    int     itemCount;


} Compartment;





typedef struct  {
    const char *name;
    const char *description;

    unsigned    targetIndex[2];
    bool        locked;

    Action      actions[3];
    unsigned    actionCount;

    Item        *wants[3];
    unsigned    wantsCount;

    Satisfier   sat;
} Door;

typedef struct {
    const char *name;
    const char *description;

    Action      actions[3];
    unsigned    actionCount;

    Compartment *compartmens[3];
    unsigned    compaCount;

    Item        *wants[3];
    unsigned    wantsCount;

    Satisfier   sat;
} Objekt;


typedef struct {
    const char *name;
    const char *description;

    Action      actions[3];
    unsigned    actionCount;

    Item        *wants[3];
    unsigned    wantsCount;

    const char  *dialog[2];
    const char  *choiceA[2];
    Action      dialogActionsA[2];
    const char  *choiceB[2];
    Action      dialogActionsB[2];
    unsigned    diaCount;


    Satisfier   sat;
} Person;






#define max_exits 4
#define max_items 6
#define max_persons 3

typedef struct {
    const char      *tag;
    const char      *description;

    const Door      *doors[max_exits];
    unsigned        doorCount;

    const Objekt    *objects[max_items];
    unsigned        objectCount;

    const Person    *persons[max_persons];
    unsigned        personCount;
    
    unsigned        currentProxy;
} Location;




typedef struct GameState {
    Location    *locations;
    unsigned    numLocations;
    unsigned    currentLocation;

    Item        *itemsInv[10];
    unsigned    itemsInvCount;

    bool        running;
};


void gameInit(GameState *gs);
Location *gameCurrentLocation(GameState *gs);


#endif
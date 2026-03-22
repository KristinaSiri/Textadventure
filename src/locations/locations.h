#ifndef LOCATION_H
#define LOCATION_H

typedef struct Location {
    int id;
    char *description;
    char *tag;
    int start;
} Location;

typedef struct Description {
    char *text;
    char *name;
} Description;

typedef struct Passage {
    int id;
    char *description;
    char *tag;
    struct Location source;
    struct Location destination;
} Passage;

typedef struct GameState {
    Location currentLocation;
    Location *worldLocations;
    int numberOfLocations;
    Passage *worldPassages;
    int numberOfPassages;
} GameState;

extern void generateWorld(GameState *gameState);
extern void getPassages(GameState *gameState, Location from, Passage *passagesFound, int *amountFound);
extern void executeLook(GameState *gameState, const char *noun);
extern int executeGo(GameState *gameState, const char *tag);
extern int listPassagesAtLocation(GameState *gameState, Location location);
extern void findPassageByTag(GameState *gameState, const char *tag, Passage *passagesFound, int *amountFound);
extern void freeWorld(GameState *gameState);
extern void printWorld(GameState *gameState);

#endif
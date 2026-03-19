

#include <stdio.h>
#include <string.h>
#include <gamestate.h>

enum {
    LOC_START = 0,
    LOC_CHAMBER,
    LOC_COUNT
};

enum { DOOR_START_CHAMBER, DOOR_COUNT };
enum { OBJ_TORCH, OBJ_STONE, OBJ_COUNT };
enum { NPC_MINER, NPC_GUARD, NPC_COUNT };

static const Door doorPool[DOOR_COUNT] = {
    [DOOR_START_CHAMBER] = {
        .name = "stone arch",
        .description = "A rough archway connects cave and chamber.",
        .targetIndex = { LOC_START, LOC_CHAMBER },
        .locked = false
    }
};

static const Objekt objectPool[OBJ_COUNT] = {
    [OBJ_TORCH] = { "torch", "A flickering wall torch." },
    [OBJ_STONE] = { "stone", "A damp cave stone." }
};

static const Person personPool[NPC_COUNT] = {
    [NPC_MINER] = { "miner", "An old miner watches you silently." },
    [NPC_GUARD] = { "guard", "A guard keeps his distance." }
};

static Location worldData[LOC_COUNT] = {
    [LOC_START] = {
        .tag = "start",
        .description = "a dark cave",
        .doors = { &doorPool[DOOR_START_CHAMBER] },
        .doorCount = 1,
        .objects = { &objectPool[OBJ_STONE] },
        .objectCount = 1,
        .persons = { &personPool[NPC_MINER] },
        .personCount = 1
    },
    [LOC_CHAMBER] = {
        .tag = "chamber",
        .description = "a torch lit chamber",
        .doors = { &doorPool[DOOR_START_CHAMBER] }, /* gleiche Tür, andere Seite */
        .doorCount = 1,
        .objects = { &objectPool[OBJ_TORCH] },
        .objectCount = 1,
        .persons = { &personPool[NPC_GUARD] },
        .personCount = 1
    }
};




void gameInit(GameState *gs) {
    gs->locations = worldData;
    gs->numLocations = sizeof(worldData) / sizeof(worldData[0]);
    gs->currentLocation = 0;
    gs->running = true;
}


Location *gameCurrentLocation(GameState *gs) {
    if (gs == NULL) return NULL;
    if (gs->currentLocation >= gs->numLocations) return NULL;
    return &gs->locations[gs->currentLocation];
}





void handleLook(GameState *gs) {
    Location *current = &gs -> locations[gs->currentLocation];
    
    printf("\nYou look around and see:\n");

    unsigned i;
    for (i = 0 ; i < current -> personCount; i++) {
        printf("%d. %s\n", i + 1, current -> persons[i] -> name);
    }

    unsigned j;
    for ( j = 0 ; j < current -> doorCount; j++) {
        printf("%d. %s\n", i + j +1 , current -> doors[j] -> name);
    }

    unsigned k;
    for ( k = 0 ; k < current -> objectCount; k++) {
        printf("%d. %s\n", i + j + k + 1, current -> objects[k] -> name);
    }
    



    printf("\nWhat do you want to look at?\n");
    printf("Your Choice: ");
    
    int choice;
    scanf("%d", &choice);
    
    
    (choice <= i) ? printf("\n%s\n", current -> persons[choice-1] -> description) :
    (choice <= i + j) ?  printf("\n%s\n", current -> doors[choice - i - 1] -> description) :
    (choice <= i + j + k) ? printf("\n%s\n", current -> objects[choice - i - j - 1] -> description) :
    printf("invalid choice");
    
}


void handleInteract(GameState *gs) {

}




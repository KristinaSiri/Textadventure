#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locations.h>

#define maxNumberOfLocations 7
#define maxNumberOfPassages (maxNumberOfLocations-1)
#define passageProb 100
#define decProb 25

struct Description descriptionTemplates[2] = {
    {"You are in a dark cave", "start"},
    {"You are in a torch litt chamber", "chamber"}
};

void findPassageByTag(GameState *gameState, const char *tag, Passage *passagesFound, int *amountFound) {
    *amountFound = 0;
    
    for (int j = 0; j < gameState->numberOfPassages; j++) {
        Passage currentPassage = gameState->worldPassages[j];

        if ((strcmp(currentPassage.tag, tag) == 0) && (gameState->currentLocation.id == currentPassage.source.id)) {
            passagesFound[*amountFound] = currentPassage;
            (*amountFound)++;
        }
    }
}

void getPassages(GameState *gameState, Location location, Passage *passagesFound, int *amountFound) {
    *amountFound = 0;
    
    for (int j = 0; j < gameState->numberOfPassages; j++) {
        Passage currentPassage = gameState->worldPassages[j];

        if ((currentPassage.source.id == location.id) || (currentPassage.destination.id == location.id)) {
            passagesFound[*amountFound] = currentPassage;
            (*amountFound)++;
        }
    }
}

void generateWorld(GameState *gameState) {
    Location *worldLocations = (Location *)malloc(3 * sizeof(Location));
    Passage *worldPassages = (Passage *)malloc(2 * sizeof(Passage));

    // Location 1
    worldLocations[0].id = 1;
    worldLocations[0].description = (char *)malloc(strlen("You are in a dark cave") + 1);
    strcpy(worldLocations[0].description, "You are in a dark cave");
    worldLocations[0].tag = (char *)malloc(strlen("start") + 1);
    strcpy(worldLocations[0].tag, "start");
    worldLocations[0].start = 1;

    // Location 2
    worldLocations[1].id = 2;
    worldLocations[1].description = (char *)malloc(strlen("You are in a torch litt chamber") + 1);
    strcpy(worldLocations[1].description, "You are in a torch litt chamber");
    worldLocations[1].tag = (char *)malloc(strlen("chamber") + 1);
    strcpy(worldLocations[1].tag, "chamber");
    worldLocations[1].start = 0;

    // Location 3
    worldLocations[2].id = 3;
    worldLocations[2].description = (char *)malloc(strlen("You are in a treasure filled tomb") + 1);
    strcpy(worldLocations[2].description, "You are in a treasure filled tomb");
    worldLocations[2].tag = (char *)malloc(strlen("tomb") + 1);
    strcpy(worldLocations[2].tag, "tomb");
    worldLocations[2].start = 0;

    // Passage 1
    worldPassages[0].id = 1;
    worldPassages[0].description = (char *)malloc(strlen("You see a faintly lit tunnel") + 1);
    strcpy(worldPassages[0].description, "You see a faintly lit tunnel");
    worldPassages[0].tag = (char *)malloc(strlen("tunnel") + 1);
    strcpy(worldPassages[0].tag, "tunnel");
    worldPassages[0].source = worldLocations[0];
    worldPassages[0].destination = worldLocations[1];

    // Passage 2
    worldPassages[1].id = 2;
    worldPassages[1].description = (char *)malloc(strlen("You see a massive stone door") + 1);
    strcpy(worldPassages[1].description, "You see a massive stone door");
    worldPassages[1].tag = (char *)malloc(strlen("door") + 1);
    strcpy(worldPassages[1].tag, "door");
    worldPassages[1].source = worldLocations[1];
    worldPassages[1].destination = worldLocations[2];

    gameState->worldLocations = worldLocations;
    gameState->numberOfLocations = 3;
    gameState->worldPassages = worldPassages;
    gameState->numberOfPassages = 2;
    gameState->currentLocation = gameState->worldLocations[0];
    
    /*
    struct Location worldLocations[numberOfLocations];
    Passage worldPassages[maxNumberOfPassages];
    Passage passagesFound[maxNumberOfPassages];


    for (size_t i = 0; i < numberOfLocations; i++) {
        int descriptionIndex = rand() % (sizeof(descriptionTemplates) / sizeof(descriptionTemplates[0]));
    
        worldLocations[i].id = i + 1;
        strcpy(worldLocations[i].description, descriptionTemplates[descriptionIndex].text);
        strcpy(worldLocations[i].tag, descriptionTemplates[descriptionIndex].name);
        worldLocations[i].start = i == 0;

        if(worldLocations[i].start) {
            gameState.currentLocation = worldLocations[i];
        }
    }

    gameState.worldLocations = worldLocations;
    
    for (size_t i = 0; i < numberOfLocations; i++) {
        for (size_t j = i; j < numberOfLocations-1; j++) {
            int foundPassages;
            getPassages(gameState, worldLocations[i], passagesFound, &foundPassages);
            int spawnPassProb = passageProb - (foundPassages * decProb);
            if ((rand() % 100)> (100 - spawnPassProb)) {
                // TODO Create Passage from location i to location j and save it in the gamestate
            }
            
        }
        
    }

    // TODO Validate generated World 
    // Valid Condition
    // Every Location has a passage leading into it
    // Max 1 Spawnroom
    // Depth of World min X locations
    */
}

int listPassagesAtLocation(GameState *gameState, Location location){
    int count = 0;
    Passage passagesFound[gameState->numberOfPassages];
    getPassages(gameState, location, passagesFound, &count);
    for (int i = 0; i < count; i++) {
        printf("%s\n", passagesFound[i].description);
    }
   return count;
}

void executeLook(GameState *gameState, const char *noun){
    if (noun != NULL && strcmp(noun, "around") == 0){
        printf("%s\n", gameState->currentLocation.description);
        listPassagesAtLocation(gameState, gameState->currentLocation);
    }
    else{
        printf("I don't understand what you want to see!\n");
    }
}

int executeGo(GameState *gameState, const char *tag) {
    int count = 0;
    Passage passagesFound[gameState->numberOfPassages];
    findPassageByTag(gameState, tag, passagesFound, &count);
    if(count > 0) {
        gameState->currentLocation = passagesFound[0].destination;
        executeLook(gameState, "around");
    }
}

void freeWorld(GameState *gameState) {
    for (int i = 0; i < gameState->numberOfLocations; i++) {
        free(gameState->worldLocations[i].description);
        free(gameState->worldLocations[i].tag);
    }
    for (int i = 0; i < gameState->numberOfPassages; i++) {
        free(gameState->worldPassages[i].description);
        free(gameState->worldPassages[i].tag);
    }
    free(gameState->worldLocations);
    free(gameState->worldPassages);
}

void printWorld(GameState *gameState) {
    printf("\n========== WORLD MAP ==========\n");
    
    // Print all locations
    printf("\n--- LOCATIONS ---\n");
    for (int i = 0; i < gameState->numberOfLocations; i++) {
        Location loc = gameState->worldLocations[i];
        printf("[%d] %s (tag: '%s') %s\n", 
            loc.id, 
            loc.description, 
            loc.tag, 
            loc.start ? "[START]" : "");
    }
    
    // Print all passages
    printf("\n--- PASSAGES ---\n");
    for (int i = 0; i < gameState->numberOfPassages; i++) {
        Passage pass = gameState->worldPassages[i];
        printf("[%d] %s (tag: '%s')\n", 
            pass.id, 
            pass.description, 
            pass.tag);
        printf("    From: %s (ID:%d) -> To: %s (ID:%d)\n", 
            pass.source.tag,
            pass.source.id,
            pass.destination.tag,
            pass.destination.id);
    }
    
    // Print current location
    printf("\n--- CURRENT LOCATION ---\n");
    printf("You are at: %s (ID: %d)\n", 
        gameState->currentLocation.tag, 
        gameState->currentLocation.id);
    
    printf("\n==============================\n\n");
}
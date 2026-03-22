#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <parsexec.h>
#include <locations.h>
#include <time.h>

static char input[100] = "look around";

static bool getInput(void){
    printf("\n-->");
    return fgets(input, sizeof input, stdin) != NULL;
}

int main(void){
    GameState *gameState = (GameState *)malloc(sizeof(GameState));
    srand(time(NULL));
    generateWorld(gameState);
    printf("[Startnachricht oder Titel].\n");
    while(parseAndExecute(gameState, input) && getInput());
    printf("\n[Abschiedsnachricht]\n");
    freeWorld(gameState);
    free(gameState);
    return 0;
}


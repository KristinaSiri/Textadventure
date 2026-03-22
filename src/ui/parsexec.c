#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <locations.h>

bool parseAndExecute(GameState *gameState,char *input){
    char *verb = strtok(input, " \n");
    char *noun = strtok(NULL, " \n");

    if (verb != NULL){
        if (strcmp(verb, "quit") == 0){
            return false;
        }
        else if (strcmp(verb, "look") == 0){
            executeLook(gameState,noun);
        }
        else if (strcmp(verb,"go") == 0){
            executeGo(gameState,noun);
        }
        else{
            printf("I don't know how to '%s'.\n", verb);
        }
    }
    return true;
}

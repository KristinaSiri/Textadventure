#include <stdbool.h>
#include <stdio.h>
#include <string.h>

bool parseAndExecute(char *input){
    char *verb = strtok(input, "\n");
    char *noun = strtok(NULL, "\n");

    if (verb != NULL){
        if (strcmp(verb, "quit") == 0){
            return 0;
        }else if (strcmp(verb, "look") == 0){
        printf("It is very dark in here.\n");
        }
        else if (strcmp(verb,"go") == 0){
            printf("You can't see where you're going!\n");
        }
        else{
            printf("I don't know how to '%s'.\n", verb);
        }
    }
}

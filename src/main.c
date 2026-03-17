#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <parsexec.h>

static char input[100] = "look around";

static bool getInput(void)
{
    printf("\n-->");
    return fgets(input, sizeof input, stdin) != NULL;
}

int main(void)
{
    printf("[Startnachricht oder Titel].\n");
    while(parseAndExecute(input) && getInput());
    printf("\n[Abschiedsnachricht]\n");
    return 0;
}

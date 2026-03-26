#include <stdio.h>
#include "gamestate.h"
#include "functions.h"

int main(void)
{
    GameState gs;
    SpielUI(&gs);
    gameInit(&gs);

    while (gs.running) {
        env_handleLook(&gs);
    }

    printf("Goodbye!\n");
    return 0;
}

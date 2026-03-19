#include <stdio.h>
#include <gamestate.h>
#include <functions.h>

int main(void)
{
    GameState gs;
    gameInit(&gs);
    
    printf("Welcome to Text Adventure!\n");
    
    while (gs.running) {
        showMenu(&gs);
    }
    
    printf("Goodbye!\n");
    return 0;
}
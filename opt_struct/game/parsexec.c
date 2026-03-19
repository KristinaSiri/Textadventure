#include <stdio.h>
#include <functions.h>

void showMenu(GameState *gs)
{
    printf("\n=== ACTION MENU ===\n");
    printf("1. Look\n");
    printf("2. Go\n");
    printf("0. Quit\n");
    printf("Choice: ");
    
    int choice;
    scanf("%d", &choice);
    getchar();
    
    if (choice == 1) {
        handleLook(gs);
    } else if (choice == 2) {
        handleGo(gs);
    } else if (choice == 0) {
        gs->running = false;
    } else {
        printf("Invalid choice!\n");
    }
}

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <gamestate.h>

void gameInit(GameState *gs);
Location *gameCurrentLocation(GameState *gs);
void handleLook(GameState *gs);
void handleGo(GameState *gs);

#endif
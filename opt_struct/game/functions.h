#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <gamestate.h>

void gameInit(GameState *gs);
Location *gameCurrentLocation(GameState *gs);
void env_handleLook(GameState *gs);
void env_handleInteract(GameState *gs, void *prxy , ObjektType typ);


#endif
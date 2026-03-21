

#include <stdio.h>
#include <string.h>
#include <gamestate.h>

enum {

    LOC_START = 0,
    LOC_CHAMBER,
    LOC_COUNT
};

enum { 

    DOOR_START_CHAMBER, 
    DOOR_COUNT 
};

enum { 

    OBJ_TABLE, 
    OBJ_STONE, 
    OBJ_COUNT 
};

enum { 

    NPC_MINER, 
    NPC_GUARD, 
    NPC_COUNT 
};

enum {

    ITM_TEST_ZETTEL,
    ITM_COUNT
};

enum {

    COMP_TEST,
    COMP_COUNT
};


    
enum {
    S_ACT_OPEN_DOOR,
    S_ACT_COUNT
};




void openDoor(GameState *gs, void* prxy, void* token);
void openComp(GameState *gs, void* prxy, void* token);

// INTERAKTIONS/CHOICE TOKEN LOGIK: Token Interaktionen immer zuerst in actions definieren. Dann standard actions.
// Zum Beispiel für: open compart 1./2./3., give/use Item 1./2./3. oder ähnliches.
// Sehr lose...







static Door doorPool[DOOR_COUNT] = {
    [DOOR_START_CHAMBER] = {
        .name = "stone arch",
        .description = "A rough archway connects cave and chamber.",
        .targetIndex = { LOC_START, LOC_CHAMBER },
        .actions    = {
            {"open door", openDoor}
        },
        .actionCount = 1,
        .locked = false
    }
};

static  Item itemPool[ITM_COUNT] = {
    [ITM_TEST_ZETTEL] = {
        .name = "old letter",
        .itemType = ITM_TYPE_TEXT,
        .description = "This is a test letter. Hope you are seeing this!"
    }
};

static  Compartment compPool[COMP_COUNT] = {
    [COMP_TEST] = {
        .name = "Broken drawer",
        .items = {&itemPool[ITM_TEST_ZETTEL]},
        .itemCount = 1,
        .locked = false
    }
};

static Objekt objectPool[OBJ_COUNT] = {
    [OBJ_TABLE] = { 
        .name = "Table", 
        .description = "A abondoned, very much messy desk.",
        .compartmens = {&compPool[COMP_TEST]},
        .compaCount = 1,
        .actions = {
            {"open drawer", openComp, &compPool[COMP_TEST]}
            
        },
        .actionCount = 1
        
        
    },

    [OBJ_STONE] = { 
        .name = "stone", 
        .description = "A damp cave stone."
    }
};

static const Person personPool[NPC_COUNT] = {
    [NPC_MINER] = { "miner", "An old miner watches you silently." },
    [NPC_GUARD] = { "guard", "A guard keeps his distance." }
};



static Location worldData[LOC_COUNT] = {
    [LOC_START] = {
        .tag = "start",
        .description = "a dark cave",
        .doors = { &doorPool[DOOR_START_CHAMBER] },
        .doorCount = 1,
        .objects = { &objectPool[OBJ_STONE] },
        .objectCount = 1,
        .persons = { &personPool[NPC_MINER] },
        .personCount = 1
    },

    [LOC_CHAMBER] = {
        .tag = "chamber",
        .description = "a torch lit chamber",
        .doors = { &doorPool[DOOR_START_CHAMBER] },
        .doorCount = 1,
        .objects = { &objectPool[OBJ_TABLE] },
        .objectCount = 1,
        .persons = { &personPool[NPC_GUARD] },
        .personCount = 1
    }
};




void gameInit(GameState *gs) {
    gs->locations = worldData;
    gs->numLocations = sizeof(worldData) / sizeof(worldData[0]);
    gs->currentLocation = 0;
    gs->running = true;
}


Location *gameCurrentLocation(GameState *gs) {
    if (gs == NULL) return NULL;
    if (gs->currentLocation >= gs->numLocations) return NULL;
    return &gs->locations[gs->currentLocation];
}





void handleInteract(GameState *gs, void *prxy , ObjektType typ);

void handleLook(GameState *gs) {
    Location *current = &gs -> locations[gs->currentLocation];
    
    printf("\nYou look around and see:\n");

    unsigned i;
    for (i = 0 ; i < current -> personCount; i++) {
        printf("%d. %s\n", i + 1, current -> persons[i] -> name);
    }

    unsigned j;
    for ( j = 0 ; j < current -> doorCount; j++) {
        printf("%d. %s\n", i + j +1 , current -> doors[j] -> name);
    }

    unsigned k;
    for ( k = 0 ; k < current -> objectCount; k++) {
        printf("%d. %s\n", i + j + k + 1, current -> objects[k] -> name);
    }
    



    printf("\nWhat do you want to look at?\n");
    printf("Your Choice: ");
    
    int choice;
    scanf("%d", &choice);
    
    
    if (choice <= i) {

        const Person *p = current->persons[choice-1];
        printf("\n%s\n", p->description);
        handleInteract(gs, (void*)p, TYPE_PERSON);

    } else if (choice <= i + j) {

        const Door *d = current->doors[choice - i - 1];
        printf("\n%s\n", d->description);
        handleInteract(gs, (void*)d, TYPE_DOOR);

    } else if (choice <= i + j + k) {

        const Objekt *o = current->objects[choice - i - j - 1];
        printf("\n%s\n", o->description);
        handleInteract(gs, (void*)o, TYPE_OBJECT);

    } else {

        printf("Ungültige Wahl.\n");
    }


}




void handleInteract(GameState *gs, void *prxy , ObjektType typ) {
    switch (typ) {
        int choice;
        int i;
        case TYPE_DOOR: {

            Door *d = (Door*)prxy;
            printf("\n What do you want to do?\n");


            for (i = 0 ; i < d -> actionCount; i++) {
                printf("%d. %s\n", i+1, d -> actions[i].name);
            }


            printf("\nYour Choice: ");

            
            scanf("%d", &choice);

            if (choice == 0) {
                printf("\n\n\nback to looking...\n\n\n");
                return;
            }

            choice <= i+1 ? d -> actions[choice - 1].execute(gs, d, d -> actions[choice - 1].token) :

            printf("invalid");
            }
            break;




        

        case TYPE_PERSON: {

            Person *p = (Person*)prxy;
            printf("\nWhat do you want to do?\n");

            for (i = 0 ; i < p -> actionCount; i++) {
                printf("%d. %s\n", i+1, p -> actions[i].name);
            }



            printf("\nYour Choice: ");

            
            scanf("%d", &choice);

            if (choice == 0) {
                printf("\n\n\nback to looking...\n\n\n");
                return;
            }

            choice <= i+1 ? p -> actions[choice - 1].execute(gs, p, p -> actions[choice - 1].token) :

            printf("invalid");
            }
            break;
        
        




        case TYPE_OBJECT: {

            Objekt *o = (Objekt*)prxy;
            printf("\nWhat do you want to do?\n");

            
            for (i = 0 ; i < o -> actionCount; i++) {
                
                printf("%d. %s\n", i+1, o -> actions[i].name);
            }



            printf("\nYour Choice: ");

            
            scanf("%d", &choice);

            if (choice == 0) {
                printf("\n\n\nback to looking...\n\n\n");
                return;
            }

            choice <= i+1 ? o -> actions[choice - 1].execute(gs, o, o -> actions[choice - 1].token) :
            
            printf("invalid");
            }
            break;

        
        
        default:
            printf("jibbet nich");
            break;
    }

}






void openDoor(GameState *gs, void* prxy, void* token) {

    Door *d = (Door*)prxy;
    if (d -> locked) {
        printf("\n\n\nThis door is locked.\nTry using a key...\n\n\n");
        return;
    }
    else {
        gs -> currentLocation == d -> targetIndex[0] ? gs -> currentLocation = d -> targetIndex[1], handleLook(gs) :
        gs -> currentLocation == d -> targetIndex[1] ? gs -> currentLocation = d -> targetIndex[0], handleLook(gs) :
        printf("You are not supposed to be here! (or get there?)");
    }
}





void openComp(GameState *gs, void* prxy, void* token) {

    Objekt *o = (Objekt*)prxy;
    Compartment *c = (Compartment*)token;
    
    if (c -> locked) {
        printf("\n\n\nThis compartment is locked.\nTry using a key...\n\n\n");
        return;
    }
    else {

        for (int i = 0; i < c -> itemCount; i++) {
            switch (c -> items[i] -> itemType) {

                case ITM_TYPE_TEXT:
                    
                    printf("%s", c -> items[i] -> description);

                    break;
                
                default:
                    break;
                }
        }
                
    }

    


}
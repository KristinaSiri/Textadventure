

#include <stdio.h>
#include <string.h>
#include <gamestate.h>


void act_openDoor(GameState *gs, void* prxy, void* token);
void act_openComp(GameState *gs, void* prxy, void* token);

itm_satisfy(GameState *gs, void* prxy, void* token);

void act_giveItem(GameState *gs, void* prxy, void* token);

void env_handleInteract(GameState *gs, void *prxy , ObjektType typ);

Item* env_openInventory(GameState *gs);

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
    ITM_LEER = 0,
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





// INTERAKTIONS/CHOICE TOKEN LOGIK: Token Interaktionen immer zuerst in actions definieren. Dann standard actions.
// Zum Beispiel für: open compart 1./2./3., give/use Item 1./2./3. oder ähnliches.
// Sehr lose...




static  Person personPool[NPC_COUNT];


static Door doorPool[DOOR_COUNT] = {
    [DOOR_START_CHAMBER] = {
        .name = "stone arch",
        .description = "A rough archway connects cave and chamber.",
        .targetIndex = { LOC_START, LOC_CHAMBER },
        .actions    = {
            {.name = "open door", .execute = act_openDoor}
        },
        .actionCount = 1,
        .locked = false
    }
};

static  Item itemPool[ITM_COUNT] = {
    [ITM_LEER] = {
        .name = "leer"
    },

    [ITM_TEST_ZETTEL] = {
        .name = "old letter",
        .itemType = ITM_TYPE_TEXT,
        .text = "This is a test letter. Hope you are seeing this!",
        .itmActions = {
            {.name = "satisfy", .execute = itm_satisfy, .token = &personPool[NPC_MINER].sat}
        },
        .actionCount = 1
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
            {.name = "open drawer", .execute = act_openComp, .token = &compPool[COMP_TEST]}
            
        },
        .actionCount = 1
        
        
    },

    [OBJ_STONE] = { 
        .name = "stone", 
        .description = "A damp cave stone."
    }
};

static  Person personPool[NPC_COUNT] = {
    [NPC_MINER] = { 
        .name = "miner", 
        .description = "An old miner watches you silently.",
        .wants = {&itemPool[ITM_TEST_ZETTEL], NULL, NULL},
        .wantsCount = 1,
        .actions = {
            {.name = "give item", .execute = act_giveItem, .token = &personPool[NPC_MINER].wants}
        },
        .actionCount = 1,
        .sat = {
            .level = 1, 
            .satActions = {
                {
                    .execute = act_openComp, 
                    .token = &compPool[COMP_TEST]
                }
            }, 
            .actionCount = 1
        }
    },  
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
    gs->itemsInvCount = 0;
    
    for (int i = 0; i < 10; i++) {           // add this back
            gs->itemsInv[i] = NULL;
    };
}


Location *gameCurrentLocation(GameState *gs) {
    if (gs == NULL) return NULL;
    if (gs->currentLocation >= gs->numLocations) return NULL;
    return &gs->locations[gs->currentLocation];
}







void env_handleLook(GameState *gs) {
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
        env_handleInteract(gs, (void*)p, TYPE_PERSON);

    } else if (choice <= i + j) {

        const Door *d = current->doors[choice - i - 1];
        printf("\n%s\n", d->description);
        env_handleInteract(gs, (void*)d, TYPE_DOOR);

    } else if (choice <= i + j + k) {

        const Objekt *o = current->objects[choice - i - j - 1];
        printf("\n%s\n", o->description);
        env_handleInteract(gs, (void*)o, TYPE_OBJECT);

    } else {

        printf("Ungültige Wahl.\n");
    }

    return;

}




void env_handleInteract(GameState *gs, void *prxy , ObjektType typ) {
    switch (typ) {
        int choice;
        int i;
        case TYPE_DOOR: {

            Door *d = (Door*)prxy;
            printf("\nWhat do you want to do?\n");


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

    return;

}



void act_openDoor(GameState *gs, void* prxy, void* token) {

    Door *d = (Door*)prxy;
    if (d -> locked) {
        printf("\n\n\nThis door is locked.\nTry using a key...\n\n\n");
        return;
    }
    else {
        gs -> currentLocation == d -> targetIndex[0] ? gs -> currentLocation = d -> targetIndex[1] :
        gs -> currentLocation == d -> targetIndex[1] ? gs -> currentLocation = d -> targetIndex[0] :
        printf("You are not supposed to be here! (or get there?)");
        return;
    }
}



void act_take(GameState *gs, void* prxy, void* token) {

    Item *i = (Item*)token;
    

    gs -> itemsInv[gs -> itemsInvCount] = i;
    gs->itemsInvCount++; 
    
    printf("\n\n\n%s added to inventory.\n\n\n", i -> name);

}


void act_openComp(GameState *gs, void* prxy, void* token) {

    Objekt *o = (Objekt*)prxy;
    Compartment *c = (Compartment*)token;
    
    if (c -> locked) {
        printf("\n\n\nThis compartment is locked.\nTry using a key...\n\n\n");
        return;
    }
    else {
        if (c -> itemCount == 0) {printf("%s is empty", c -> name); return;};

        for (int i = 0; i < c -> itemCount; i++) {

            if (c->items[i] == NULL) continue; // Springe zum nächsten Slot
        
            switch (c -> items[i] -> itemType) {

                case ITM_TYPE_TEXT : {
                    
                    printf("\n\n\n%s:\n%s",c -> items[i] -> name, c -> items[i] -> text);

                    printf("\n\nTake, y/n?\nYour choice: ");

                    int ch;
                    // 1. Puffer-Reinigung: Ignoriere alle alten Newlines/Leerzeichen
                    while ((ch = getchar()) == '\n' || ch == '\r' || ch == ' ');

                    if (ch == 'y') {
                        
                        act_take(gs, prxy, c -> items[i]);
                        c -> items[i] = NULL;
                        c -> itemCount--;
                    }
                }
                break;
                
                default:{
                    break;
                }
            }
        }

        return;
                
    }

    


}

itm_satisfy(GameState *gs, void* prxy, void* token) {
    
    Satisfier *s = (Satisfier*)token;

    s -> level -= 1;

    if (s -> level == 0) {
        for (int i = 0; i < s -> actionCount; i++) s -> satActions[i].execute(gs, prxy, s -> satActions -> token);
    }

}

void act_giveItem(GameState *gs, void* prxy, void* token) {

    Item **wantedItems = (Item **)token;
    Item *toggleItem = env_openInventory(gs);
    if (toggleItem == NULL) return; 

    int i = 0;
    while (wantedItems[i] != NULL) {

        if (wantedItems[i] == toggleItem) {

            for (int j = 0; j < wantedItems[i] -> actionCount; j++) wantedItems[i] -> itmActions[j].execute(gs, prxy, wantedItems[i] -> itmActions[j].token);

            wantedItems[i] = &itemPool[ITM_LEER];
            return;
        }
        i++;
    }

    printf("\n\n\n%s cannot be used with here.\n\n\n", toggleItem -> name);
    return;



    
}

Item* env_openInventory(GameState *gs) {

    printf("\n\n\nYour items:\n");

    for (int i = 0; i < gs -> itemsInvCount; i++) {
        printf("\n%d. %s", i+1, gs -> itemsInv[i] -> name);
    }


    printf("\nYour Choice: ");

    int choice;
    scanf("%d", &choice);

    if (choice == 0) return NULL;
    
    
    if (choice <= gs -> itemsInvCount) {
        printf("\n\n\nJANK!!!: %s", gs -> itemsInv[choice-1] -> text);
        return gs -> itemsInv[choice-1];
    }

    printf("Pockets to deep?");
    return NULL;

    

}
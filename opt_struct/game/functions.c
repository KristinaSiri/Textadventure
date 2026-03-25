

#include <stdio.h>
#include <string.h>
#include <gamestate.h>


void act_openDoor(GameState *gs, void* prxy, Token* token);
void act_openComp(GameState *gs, void* prxy, Token* token);

itm_satisfy(GameState *gs, void* prxy, Token* token);

void sat_UnOrLock(GameState *gs, void* prxy, Token* token);

void act_giveItem(GameState *gs, void* prxy, Token* token);

void env_handleInteract(GameState *gs, void *prxy , ObjektType typ);

Item* env_openInventory(GameState *gs);

enum {

    LOC_START = 0,
    LOC_MAIN_DRIVEWAY,
    LOC_GATEHOUSE,
    LOC_MAIN_ENTRANCE,
    LOC_GATEHOUSE_OFFICE,
    LOC_GATEHOUSE_CLOSET,
    LOC_GATEHOUSE_GUARDSHACK,
    LOC_COUNT
};

enum { 

    ENT_PATH_,
    ENT_DRIVEWAY_GATEHOUSE,
    ENT_GATEHOUSE_MAIN,
    ENT_GATEHOUSE_OFFICE,
    ENT_GATEHOUSE_CLOSET,
    ENT_GATEHOUSE_GUARDSHK,
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
    NPC_GRANNY,
    NPC_COUNT 
};

enum {
    ITM_LEER = 0,
    ITM_TEST_ZETTEL,
    ITM_GRANNYS_PURSE,
    ITM_GRANNYS_GLASSES,


    KEY_TEST,
    KEY_GATEH_MAIN,


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
static  Door doorPool[DOOR_COUNT];
static  Item itemPool[ITM_COUNT];
static  Compartment compPool[COMP_COUNT];
static  Objekt objectPool[OBJ_COUNT];
static  Person personPool[NPC_COUNT];
static  Location worldData[LOC_COUNT];




static Door doorPool[DOOR_COUNT] = {

    
    [ENT_PATH_] = {
        .name = "stone arch",
        .description = "A rough, stoneren archway marks the entrance to an astate.",
        .targetIndex = { LOC_START, LOC_MAIN_DRIVEWAY },
        .actions    = {
            {.name = "open door", .execute = act_openDoor},
            {.name = "use item", .execute = act_giveItem, .token = { .Tkns = { &doorPool[ENT_PATH_].wants } } }
        },

        .wants = {&itemPool[KEY_TEST]},
        .wantsCount = 1,

        .actionCount = 2,
        .locked = false,

        .sat = {
            .level = 1, 
            .satActions = {
                {.execute = sat_UnOrLock, .token = { .Tkns = { &doorPool[ENT_PATH_].locked } } }
            }, 
            .satActionCount = 1
        }
    },

    [ENT_DRIVEWAY_GATEHOUSE] = {
        .name = "ENT_DRIVEWAY_GATEHOUSE",
        .description = "Test desc door 2",

        .targetIndex = { LOC_MAIN_DRIVEWAY, LOC_GATEHOUSE },
        .actions    = {
            {.name = "open door", .execute = act_openDoor}
        },
        .actionCount = 1,
        
        .wants = { },
        .wantsCount = 0,

        
        .locked = false,


    },

    [ENT_GATEHOUSE_MAIN] = {
        .name = "ENT_GATEHOUSE_MAIN",
        .description = "Test desc door 3",

        .targetIndex = { LOC_GATEHOUSE, LOC_MAIN_ENTRANCE },

        .actions    = {
            {.name = "open door", .execute = act_openDoor},
            {.name = "use item", .execute = act_giveItem, .token = { .Tkns = { &doorPool[ENT_GATEHOUSE_MAIN].wants } } }
        },

        .wants = {&itemPool[KEY_GATEH_MAIN]},
        .wantsCount = 1,

        .actionCount = 2,
        .locked = true,

        .sat = {
            .level = 1, 
            .satActions = {
                {.execute = sat_UnOrLock, .token = { .Tkns = { &doorPool[ENT_GATEHOUSE_MAIN].locked } } }
            }, 
            .satActionCount = 1
        }
    },

    [ENT_GATEHOUSE_OFFICE] = {
        .name = "ENT_GATEHOUSE_OFFICE",
        .description = "Test desc door 3",

        .targetIndex = { LOC_GATEHOUSE, LOC_GATEHOUSE_OFFICE },

        .actions    = {
            {.name = "open door", .execute = act_openDoor}
            
        },



        .actionCount = 1,
        .locked = false

        
    },

    [ENT_GATEHOUSE_CLOSET] = {
        .name = "",
        .description = "Test desc door 3",

        .targetIndex = { LOC_GATEHOUSE, LOC_GATEHOUSE_CLOSET },

        .actions    = {
            {.name = "open door", .execute = act_openDoor}
        },


        .actionCount = 1,
        .locked = false

        
    },

    [ENT_GATEHOUSE_GUARDSHK] = {
        .name = "",
        .description = "Test desc door 3",

        .targetIndex = { LOC_GATEHOUSE, LOC_GATEHOUSE_GUARDSHACK},

        .actions    = {
            {.name = "open door", .execute = act_openDoor}
        },


        .actionCount = 1,
        .locked = false

        
    },


    
};

static  Item itemPool[ITM_COUNT] = {
    [ITM_LEER] = {
        .name = "leer"
    },

    [ITM_TEST_ZETTEL] = {
        .name = "old letter",
        .text = "This is a test letter. Hope you are seeing this!",

        .itemType = ITM_TYPE_TEXT,

        .itmActions = {
            {.name = "satisfy", .execute = itm_satisfy, .token = { .Tkns = {&personPool[NPC_MINER].sat}}}
        },
        .itmActionCount = 1
    },


    [ITM_GRANNYS_PURSE] = {
        .name = "The reseptionists handbag.",
        .text = "A Handbag deeper than any other place known to mankind",

        .itemType = ITM_TYPE_TEXT,

        .itmActions = {
            {.name = "satisfy", .execute = itm_satisfy, .token = { .Tkns = {&personPool[NPC_GRANNY].sat}}}
        },
        .itmActionCount = 1

    },

    [ITM_GRANNYS_GLASSES] = {
        .name = "A pair of glasses",
        .text = "",

        .itemType = ITM_TYPE_TEXT,

        .itmActions = {
            {.name = "satisfy", .execute = itm_satisfy, .token = { .Tkns = {&personPool[NPC_GRANNY].sat}}}
        },
        .itmActionCount = 1

    },







    [KEY_TEST] = {
        .name = "Key door 1",
        .text = "This key might be usefull to open the first door.",

        .itemType = ITM_TYPE_TEXT,

        .itmActions = {
            { .name = "Key 1", .execute = itm_satisfy, .token = { .Tkns = {&doorPool[ENT_PATH_].sat} } }
        },
        .itmActionCount = 1
    },

    [KEY_GATEH_MAIN] = {
        .name = "Key ENT_GATEHOUSE_MAIN",
        .text = "",

        .itemType = ITM_TYPE_TEXT,

        .itmActions = {
            { .name = "Key main gate", .execute = itm_satisfy, .token = { .Tkns = {&doorPool[ENT_GATEHOUSE_MAIN].sat} } }
        },
        .itmActionCount = 1
    }
};

static  Compartment compPool[COMP_COUNT] = {


    [COMP_TEST] = {
        .name = "Broken drawer",

        .items = { &itemPool[KEY_TEST] },
        .itemCount = 1,

        .locked = false
    }
};

static Objekt objectPool[OBJ_COUNT] = {


    [OBJ_TABLE] = { 
        .name = "Table", 
        .description = "A abondoned, very much messy desk.",
        
        .actions = {
            {.name = "open drawer", .execute = act_openComp, .token= { .Tkns = { &compPool[COMP_TEST] } } }
        },
        .actionCount = 1,

        .compartmens = {&compPool[COMP_TEST]},
        .compaCount = 1
        
        
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

        .actions = {
            {.name = "give item", .execute = act_giveItem, .token = { .Tkns = { &personPool[NPC_MINER].wants } } }
        },
        .actionCount = 1,


        .wants = {&itemPool[ITM_TEST_ZETTEL], NULL, NULL},
        .wantsCount = 1,
        
        
        .sat = {
            .level = 1, 
            .satActions = {
                {.execute = act_openComp, .token = { .Tkns = { &compPool[COMP_TEST] } }}
            }, 
            .satActionCount = 1
        }
    },
    [NPC_GUARD] = { "guard", "A guard keeps his distance." },

    [NPC_GRANNY] = {
        .name = "old receptionist",
        .description = "A hunchbacked, little and very old resptionist looks at you in distress.",

        .dialog = {
            {""},
            {""}
        },

        .actions = {
            {.name = "give item", .execute = act_giveItem, .token = { .Tkns = { &personPool[NPC_MINER].wants } } }
        },
        .actionCount = 1,


        .wants = {&itemPool[ITM_GRANNYS_PURSE], &itemPool[ITM_GRANNYS_GLASSES], NULL},
        .wantsCount = 2,
        
        
        .sat = {
            .level = 1, 
            .satActions = {
                {.execute = act_take, .token = { .Tkns = { &itemPool[KEY_GATEH_MAIN], } } }
            }, 
            .satActionCount = 1
        }

    }
};



static Location worldData[LOC_COUNT] = {
    [LOC_START] = {
        .tag = "start",
        .description = "a path in the",
        .doors = { &doorPool[ENT_PATH_] },
        .doorCount = 1,
        .objects = { &objectPool[OBJ_STONE], &objectPool[OBJ_TABLE] },
        .objectCount = 2,
        .persons = { &personPool[NPC_MINER] },
        .personCount = 1
    },

    [LOC_MAIN_DRIVEWAY] = {
        .tag = "driveway",
        .description = "a large driveway ",
        .doors = { &doorPool[ENT_PATH_], &doorPool[ENT_DRIVEWAY_GATEHOUSE] },
        .doorCount = 2,
        .objects = {  },
        .objectCount = 0,
        .persons = { &personPool[NPC_GUARD] },
        .personCount = 1
    },

    [LOC_GATEHOUSE] = {
        .tag = "gatehouse",
        .description = "an dimly lit gatehouse",
        .doors = { &doorPool[ENT_DRIVEWAY_GATEHOUSE], &doorPool[ENT_GATEHOUSE_MAIN], &doorPool[ENT_GATEHOUSE_OFFICE] },
        .doorCount = 3,
        .objects = {  },
        .objectCount = 0,
        .persons = { &personPool[NPC_GRANNY] },
        .personCount = 0

    },

    [LOC_MAIN_ENTRANCE] = {
        .tag = "LOC_MAIN_ENTRANCE",
        .description = "Test room desc 4",
        .doors = { &doorPool[ENT_GATEHOUSE_MAIN] },
        .doorCount = 1,
        .objects = {  },
        .objectCount = 0,
        .persons = {  },
        .personCount = 0

    },

    [LOC_GATEHOUSE_OFFICE] = {
        .tag = "LOC_GATEHOUSE_OFFICE",
        .description = "Test room desc 5",
        .doors = { &doorPool[ENT_GATEHOUSE_OFFICE] },
        .doorCount = 1,
        .objects = {  },
        .objectCount = 0,
        .persons = {  },
        .personCount = 0

    },

    [LOC_GATEHOUSE_CLOSET] = {
        .tag = "LOC_GATEHOUSE_CLOSET",
        .description = "Test room desc 5",
        .doors = { &doorPool[ENT_GATEHOUSE_CLOSET] },
        .doorCount = 1,
        .objects = {  },
        .objectCount = 0,
        .persons = {  },
        .personCount = 0

    },


    [LOC_GATEHOUSE_GUARDSHACK] = {
        .tag = "",
        .description = "Test room desc 5",
        .doors = { &doorPool[ENT_GATEHOUSE_GUARDSHK] },
        .doorCount = 1,
        .objects = {  },
        .objectCount = 0,
        .persons = {  },
        .personCount = 0

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







void env_handleLook(GameState *gs) 
{
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

            choice <= i+1 ? d -> actions[choice - 1].execute(gs, d, &d -> actions[choice - 1].token) :

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

            choice <= i+1 ? p -> actions[choice - 1].execute(gs, p, &p -> actions[choice - 1].token) :
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

            choice <= i+1 ? o -> actions[choice - 1].execute(gs, o, &o -> actions[choice - 1].token) :
            
            printf("invalid");
            }
            break;

        
        
        default:
            printf("jibbet nich");
            break;
    }

    return;

}


void act_talk(GameState *gs, void* prxy, Token* token) {

    Person *p = (Person*)prxy;


    for (int i = 0; i < p -> diaCount; i++) {

        printf("\n\n\n%s:\n\n%s",p -> name, p -> dialog[i]);

        printf("\n\nYou: ...\n");
        printf("\n1. %s\n", p -> dialogActionsA -> name);
        printf("\n2. %s\n", p -> dialogActionsB -> name);

        printf("\nYour Choice; ");

        int decsion = 0;
        
        while(decsion == 0) {

            int ch = 0;
            scanf("%d", ch);

            if (ch == 1) { p -> dialogActionsA -> execute(gs, prxy, &p -> dialogActionsA -> token); decsion = 1; }
            else if (ch == 2) { p -> dialogActionsA -> execute(gs, prxy, &p -> dialogActionsB -> token); decsion = 1; }
            else printf("\n\nYou need to decide...");
    
        }
    }

} 


void act_openDoor(GameState *gs, void* prxy, Token* token) {

    Door *d = (Door*)prxy;
    if (d -> locked) {
        printf("\n\n\nThis door is locked.\nTry using a key...\n\n\n");
        return;
    }   
    else {
        gs -> currentLocation == d -> targetIndex[0] ? (gs -> currentLocation = d -> targetIndex[1], printf("\n\n\nYou enter %s\n\n\n", gs -> locations[gs -> currentLocation].description )) :
        gs -> currentLocation == d -> targetIndex[1] ? (gs -> currentLocation = d -> targetIndex[0], printf("\n\n\nYou enter %s\n\n\n", gs -> locations[gs -> currentLocation].description )) :
        printf("You are not supposed to be here! (or get there?)");
        return;
    }
}

// für take() muss token -> Tkns[0] auf das Item pointer array .itemsInv innerhalb der struktur zeigen


void act_take(GameState *gs, void* prxy, Token* token) {

    Token *t = (Token*)token;

    Item *i = t -> Tkns[0];
    

    gs -> itemsInv[gs -> itemsInvCount] = i;
    gs->itemsInvCount++; 
    
    printf("\n\n\n%s added to inventory.\n\n\n", i -> name);

}


// für openComp() muss token -> Tkns[0] auf das Compartments pointer array .compartments innerhalb der struktur zeigen


void act_openComp(GameState *gs, void* prxy, Token* token) {
    
    Token *t = (Token*)token;

    Compartment *c = t -> Tkns[0];
    
    if (c -> locked) {
        printf("\n\n\nThis compartment is locked.\nTry using a key...\n\n\n");
        return;
    }
    else {
        if (c -> itemCount == 0) {printf("\n\n%s is empty\n\n\n", c -> name); return;}

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

                        // "statischer" aufruf ist relativ klobig

                        Token t = {
                            .Tkns={c -> items[i]}
                        };
                        
                        act_take(gs, prxy, &t);

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


// für itm_satisfy() muss token -> Tkns[0] auf den Satisfier .sat innerhalb der struktur zeigen

itm_satisfy(GameState *gs, void* prxy, Token* token) {
    
    Token *t = (Token*)token;

    Satisfier *s = t -> Tkns[0];

    s -> level -= 1;

    if (s -> level == 0) {
        for (int i = 0; i < s -> satActionCount; i++) s -> satActions[i].execute(gs, prxy, &s -> satActions[i].token);
    }

}

// Spagetthi? Weil will keine separate struktur für locked welche ich immer als token übergeben kann. DOch nicht weil ich der goat bin...


// für unOrLocj() muss token -> Tkns[0] auf den .locked bool innerhalb der struktur zeigen

void sat_UnOrLock(GameState *gs, void* prxy, Token* token) {

    Token *t = (Token*)token;

    bool *locked = t -> Tkns[0];

    if (*locked == true){
        printf("\n\n\n*click*\n\n\n");
        *locked = false;
    }
    else if (*locked == false){
        printf("*click*");
        *locked = true;
    }
    return;
}



// für act_giveItem() muss token -> Tkns[0] auf das Item pointer array .wants innerhalb der struktur zeigen

void act_giveItem(GameState *gs, void* prxy, Token* token) {

    Token *t = (Token*)token;

    Item **wantedItems = t -> Tkns[0];
    Item *toggleItem = env_openInventory(gs);
    if (toggleItem == NULL) return; 

    int i = 0;
    while (wantedItems[i] != NULL) {

        if (wantedItems[i] == toggleItem) {

            for (int j = 0; j < wantedItems[i] -> itmActionCount; j++) wantedItems[i] -> itmActions[j].execute(gs, prxy, &wantedItems[i] -> itmActions[j].token);

            return;
        }
        i++;
    }

    printf("\n\n\n%s cannot be used here.\n\n\n", toggleItem -> name);
    return;



    
}

Item* env_openInventory(GameState *gs) {

    printf("\n\n\nYour items:\n");

    for (int i = 0; i < gs -> itemsInvCount; i++) {
        printf("\n%d. %s:", i+1, gs -> itemsInv[i] -> name);
        printf("\n%s", gs -> itemsInv[i] -> text);
    }


    printf("\n\nYour Choice: ");

    int choice;
    scanf("%d", &choice);

    if (choice == 0) return NULL;
    
    
    if (choice <= gs -> itemsInvCount) {
       
        return gs -> itemsInv[choice-1];
    }

    printf("Pockets to deep?");
    return NULL;

    

}
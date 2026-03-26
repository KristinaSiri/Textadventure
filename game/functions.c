#include <stdio.h>
#include <string.h>
#include "gamestate.h"
#include "farben.h"
#include "menu.h"
#include "lebensanzeige.h"

void act_openDoor(GameState *gs, void* prxy, Token* token);
void act_openComp(GameState *gs, void* prxy, Token* token);
void act_talk(GameState *gs, void* prxy, Token* token);
void env_unhideActions(GameState *gs, void* prxy, Token* token);
void itm_satisfy(GameState *gs, void* prxy, Token* token);
void sat_UnOrLock(GameState *gs, void* prxy, Token* token);
void act_giveItem(GameState *gs, void* prxy, Token* token);
void act_take(GameState *gs, void* prxy, Token* token);
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

    ENT_PATH_ESTATE,

    ENT_DRIVEWAY_GATEHOUSE,

    ENT_GATEHOUSE_OFFICE,
    ENT_GATEHOUSE_CLOSET,
    ENT_GATEHOUSE_GUARDSHK,
    
    ENT_GATEHOUSE_MAIN,


    DOOR_COUNT
    
};

enum { 

    OBJ_WORKDESK, 
    OBJ_STONE,
    
    OBJ_SIDEBOARD,
    OBJ_LAMP,

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
    KEY_WORKDESK,


    ITM_COUNT
};

enum {

    COMP_EMPTY,

    COMP_DESK_LOCKED,
    COMP_GATEH_WD_LS,


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
static  Location worldData[LOC_COUNT];




static Door doorPool[DOOR_COUNT] = {

    
    [ENT_PATH_ESTATE] = {
        .name = "stone arch",
        .description = "A rough, stoneren archway marks the entrance to a large astate.",
        .targetIndex = { LOC_START, LOC_MAIN_DRIVEWAY },
        .actions    = {
            {.name = "go through", .execute = act_openDoor},
            {.name = "use item", .execute = act_giveItem, .token = { .Tkns = { &doorPool[ENT_PATH_ESTATE].wants } } }
        },

        .wants = {&itemPool[KEY_TEST]},
        .wantsCount = 1,

        .actionCount = 2,
        .locked = false,

        .sat = {
            .level = 1, 
            .satActions = {
                {.execute = sat_UnOrLock, .token = { .Tkns = { &doorPool[ENT_PATH_ESTATE].locked } } }
            }, 
            .satActionCount = 1
        }
    },

    [ENT_DRIVEWAY_GATEHOUSE] = {
        .name = "the enternce to a small gatehouse.",
        .description = "A simple but still decadent door.",

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
        .name = "the enternce to the main courtyard",
        .description = "just a door",

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
        .name = "a door to a small office.",
        .description = "just a door.",

        .targetIndex = { LOC_GATEHOUSE, LOC_GATEHOUSE_OFFICE },

        .actions    = {
            {.name = "enter", .execute = act_openDoor}
            
        },



        .actionCount = 1,
        .locked = false

        
    },

    [ENT_GATEHOUSE_CLOSET] = {
        .name = "a door to a closet.",
        .description = "Just a regual door aswell",

        .targetIndex = { LOC_GATEHOUSE, LOC_GATEHOUSE_CLOSET },

        .actions    = {
            {.name = "enter", .execute = act_openDoor}
        },


        .actionCount = 1,
        .locked = false

        
    },

    [ENT_GATEHOUSE_GUARDSHK] = {
        .name = "a door to a guard shed.",
        .description = "Still a door",

        .targetIndex = { LOC_GATEHOUSE, LOC_GATEHOUSE_GUARDSHACK},

        .actions    = {
            {.name = "enter", .execute = act_openDoor}
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
        .name = "The reseptionists handbag",
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
            { .name = "Key 1", .execute = itm_satisfy, .token = { .Tkns = {&doorPool[ENT_PATH_ESTATE].sat} } }
        },
        .itmActionCount = 1
    },

    [KEY_GATEH_MAIN] = {
        .name = "Key to courtyard",
        .text = "",

        .itemType = ITM_TYPE_TEXT,

        .itmActions = {
            { .name = "Key main gate", .execute = itm_satisfy, .token = { .Tkns = {&doorPool[ENT_GATEHOUSE_MAIN].sat} } }
        },
        .itmActionCount = 1
    },

    [KEY_WORKDESK] = {

        .name = "U2nknown key",
        .text = "An absurdly large, unwieldy Key. It may belong to a drawer of some sort...",

        .itemType = ITM_TYPE_TEXT,

        .itmActions = {
            { .name = "Key to workdesk", .execute = sat_UnOrLock, .token = { .Tkns = {&compPool[COMP_DESK_LOCKED].locked} } }
        },
        .itmActionCount = 1
        
    },

    
};

static  Compartment compPool[COMP_COUNT] = {


    [COMP_DESK_LOCKED] = {
        .name = "Desk cabinet",

        .items = { &itemPool[ITM_GRANNYS_PURSE] },
        .itemCount = 1,

        .locked = true
    },

    [COMP_GATEH_WD_LS] = {
        .name = "hidden lampshade comp",

        .items = { &itemPool[KEY_WORKDESK] },
        .itemCount = 1,

        .locked = false
    },

    [COMP_EMPTY] = {
        .name = "this compartment",

    },

   
};

static Objekt objectPool[OBJ_COUNT] = {


    [OBJ_WORKDESK] = { 
        .name = "a desk", 
        .description = "A well worked, very much messy desk.",
        
        .actions = {
            {.name = "open drawer", .execute = act_openComp, .token= { .Tkns = { &compPool[COMP_EMPTY] } } },
            {.name = "open large cabinet", .execute = act_openComp, .token= { .Tkns = { &compPool[COMP_DESK_LOCKED] } } },
            {.name = "use key", .execute = act_giveItem, .token = { .Tkns = { &objectPool[OBJ_WORKDESK].wants } } }

        },
        .actionCount = 3,

        .compartmens = {&compPool[COMP_EMPTY], &compPool[COMP_DESK_LOCKED]},
        .compaCount = 2,

        .wants = {&itemPool[KEY_WORKDESK],NULL,NULL},
        .wantsCount = 1,

        
        
        
    },

    [OBJ_STONE] = { 
        .name = "stone", 
        .description = "A damp cave stone.",
// braucht noch eine .actions damit man etwas auswählen kann
    },

    [OBJ_SIDEBOARD] = { 
        .name = "sideboard", 
        .description = "A small empty sideboard, with a lamp atop.",
        
        .actions = {
            {.name = "open first drawer", .execute = act_openComp, .token= { .Tkns = { &compPool[COMP_EMPTY] } } },
            {.name = "open second drawer", .execute = act_openComp, .token= { .Tkns = { &compPool[COMP_EMPTY] } } }
        },
        .actionCount = 2,

        .compartmens = {&compPool[COMP_EMPTY], &compPool[COMP_EMPTY]},
        .compaCount = 2
        
        
    },


    [OBJ_LAMP] = { 
        .name = "a lamp", 
        .description = "A discontinued desk lamp",
        
        .actions = {
            {.name = "look under the shade", .execute = act_openComp, .token= { .Tkns = { &compPool[COMP_GATEH_WD_LS] } } }
        },
        .actionCount = 0,

        .compartmens = {&compPool[COMP_GATEH_WD_LS]},
        .compaCount = 1
        
        
    },

   
};

static  Person personPool[NPC_COUNT] = {
    [NPC_MINER] = { 
        .name = "miner", 
        .description = "An old miner watches you silently.",
        
    },

    [NPC_GUARD] = { 
	.name = "guard", "A guard keeps his distance." },


    [NPC_GRANNY] = {
        .name = "an old receptionist",
        .description = "A hunchbacked, little and very old resptionist looks at you in distress.",

        .dialog = {

            "Oh deary, oh deary me! I can't see a thing without my glasses, and my handbag is still locked inside my desk!",
            "Please, if you find my things, bring them to me I'll let you into the estate. It's freezing out here anyway.",
        },

        .actions = {
            {.name = "give item", .execute = act_giveItem, .token = { .Tkns = { &personPool[NPC_GRANNY].wants } } },
            {.name = "talk", .execute = act_talk}
        },
        .actionCount = 2,
        .diaCount = 2,

        .dialogActionsA = {
            {.name = "I'll find your things", .execute = act_openComp, .token = { .Tkns = { &compPool[COMP_EMPTY] } } },
            {.name = "You can count on me", .execute = act_openComp, .token = { .Tkns = { &compPool[COMP_EMPTY] } } }
        },
        .dialogActionsB = {
            {.name = "Not my problem, granny", .execute = act_openComp, .token = { .Tkns = { &compPool[COMP_EMPTY] } } },
            {.name = "Just hand over the key", .execute = act_openComp, .token = { .Tkns = { &compPool[COMP_EMPTY] } } }
        },


        .wants = {&itemPool[ITM_GRANNYS_PURSE], &itemPool[ITM_GRANNYS_GLASSES], NULL},
        .wantsCount = 2,
        
        
        .sat = {
            .level = 2, 
            .satActions = {
                {.execute = act_take, .token = { .Tkns = { &itemPool[KEY_GATEH_MAIN], } } }
            }, 
            .satActionCount = 1
        }

    },


    [NPC_GUARD] = { 
        .name = "guard", 
        .description = "A guard sits there unbotherd." ,

        .dialog = {
            "Here are the old hags glasses",
            "What? Ahh anyway, fetch the keys while youre at it. They're under the lampshade in the closet"
        },

        .actions = {
            {.name = "talk", .execute = act_talk}
        },
        .actionCount = 1,
        .diaCount = 2,

        .dialogActionsA = {
            {.name ="Thanks, I'll bring these to her", .execute = act_take, .token = { .Tkns = { &itemPool[ITM_GRANNYS_GLASSES] } }},
            {.name ="Under the lampshade? Got it", .execute = env_unhideActions, .token = { .Tkns = {&objectPool[OBJ_LAMP].actionCount}} }
        },

        .dialogActionsB = {
            {.name ="Why do you have her glasses?", .execute = act_take, .token = { .Tkns = { &itemPool[ITM_GRANNYS_GLASSES] } }},
            {.name ="I'm not your errand boy", .execute = env_unhideActions, .token = { .Tkns = {&objectPool[OBJ_LAMP].actionCount}}}
        }
    },

   
};



static Location worldData[LOC_COUNT] = {
    [LOC_START] = {
        .tag = "path",
        .description = "a path in the woods",
        .doors = { &doorPool[ENT_PATH_ESTATE] },
        .doorCount = 1,
        .objects = { &objectPool[OBJ_STONE] },
        .objectCount = 1,
        .persons = { &personPool[NPC_MINER] },
        .personCount = 1
    },

    [LOC_MAIN_DRIVEWAY] = {
        .tag = "driveway",
        .description = "a large driveway ",
        .doors = {&doorPool[ENT_PATH_ESTATE],   &doorPool[ENT_DRIVEWAY_GATEHOUSE],  },
        .doorCount = 2,
        .objects = {  },
        .objectCount = 0,
        .persons = {  },
        .personCount = 0
    },

    [LOC_GATEHOUSE] = {
        .tag = "gatehouse",
        .description = "an dimly lit gatehouse with an old lady standing helplessly",
        .doors = { &doorPool[ENT_DRIVEWAY_GATEHOUSE], &doorPool[ENT_GATEHOUSE_MAIN], &doorPool[ENT_GATEHOUSE_OFFICE], &doorPool[ENT_GATEHOUSE_CLOSET], &doorPool[ENT_GATEHOUSE_GUARDSHK] },
        .doorCount = 5,
        .objects = {  },
        .objectCount = 0,
        .persons = { &personPool[NPC_GRANNY] },
        .personCount = 1

    },

    [LOC_MAIN_ENTRANCE] = {
        .tag = "entrance courtyard",
        .description = "a wide cobblestone courtyard before the estate's grand facade",
        .doors = { &doorPool[ENT_GATEHOUSE_MAIN], },
        .doorCount = 2,
        .objects = {  },
        .objectCount = 0,
        .persons = {  },
        .personCount = 0

    },

    [LOC_GATEHOUSE_OFFICE] = {
        .tag = "office",
        .description = "a little, bunched together, well worked office",
        .doors = { &doorPool[ENT_GATEHOUSE_OFFICE] },
        .doorCount = 1,
        .objects = { &objectPool[OBJ_WORKDESK] },
        .objectCount = 1,
        .persons = {  },
        .personCount = 0

    },

    [LOC_GATEHOUSE_CLOSET] = {
        .tag = "closet",
        .description = "a tiny but, heavily stacked closet",
        .doors = { &doorPool[ENT_GATEHOUSE_CLOSET] },
        .doorCount = 1,
        .objects = {&objectPool[OBJ_SIDEBOARD], &objectPool[OBJ_LAMP] },
        .objectCount = 2,
        .persons = {  },
        .personCount = 0

    },


    [LOC_GATEHOUSE_GUARDSHACK] = {
        .tag = "guard shack",
        .description = "a gurad shack full of smoke, with a fat guard sitting there, his legs stretched to a sterile desk.",
        .doors = { &doorPool[ENT_GATEHOUSE_GUARDSHK] },
        .doorCount = 1,
        .objects = {  },
        .objectCount = 0,
        .persons = {  &personPool[NPC_GUARD]},
        .personCount = 1

    },

    
    
};




void gameInit(GameState *gs) {
    gs->locations = worldData;
    gs->numLocations = sizeof(worldData) / sizeof(worldData[0]);
    gs->currentLocation = 0;
    gs -> wasLocation = 0;
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

// Tkns[0] object wo compartment spawnt
// Tkns[1] compartment was spwanen soll

// Warte, bessere lösung für versteckte actionene -> einfach counter hoch machen

void env_unhideActions (GameState *gs, void* prxy, Token *token) {

    Token *t = (Token*)token;
    unsigned *count = t -> Tkns[0];

    (*count)++;

}

void env_spawnCompartment(GameState *gs, void* prxy, Token *token) {
    
    Token *t = (Token*)token;

    Objekt *o = (Objekt*)t -> Tkns[0];

    if (o->compaCount >= 3) { printf(weißH schwarz "%-30s" "No more compartment slots." "%-30s" normal "\n", "", ""); return; }

    o -> compartmens[o -> compaCount] = t -> Tkns[1];
    o -> compaCount ++;
}



void env_handleLook(GameState *gs) 
{
    Location *current = &gs -> locations[gs->currentLocation];

    printf(löschen);
    printf(leerezeile5);
    printf(weißH schwarz "%-31s" "You look around and see:" "%-31s" normal "\n", "", "");
    printf(leerezeile);



    unsigned i;
    for (i = 0 ; i < current -> personCount; i++) {
	printf(leerezeile5);
        printf(weißH schwarz "%-36s" "%d. %-47s" normal "\n", "", i + 1, current -> persons[i] -> name);
    }

    unsigned j;
    for ( j = 0 ; j < current -> doorCount; j++) {
        printf(weißH schwarz "%-36s" "%d. %-47s" normal "\n", "", i + j +1, current -> doors[j] -> name);
    }

    unsigned k;
    for ( k = 0 ; k < current -> objectCount; k++) {
        printf(weißH schwarz "%-36s" "%d. %-47s" normal "\n", "", i + j + k + 1, current -> objects[k] -> name);
    }

    printf(leerezeile5);
    printf(weißH schwarz "%-25s" "What do you want to look at?" "%-33s" normal "\n", "", "");
    printf(leerezeile5);
    printf(leerezeile);
    printf(hp100);
    printf("\nYour Choice: " normal);

    int choice;
    scanf("%d", &choice);
    
    
    if (choice <= i) {

        const Person *p = current->persons[choice-1];
	printf(löschen);
	printf(leerezeile2);
        printf(weißH schwarz "%-20s" "%-66s" normal "\n", "", p->description);
	printf(leerezeile3);
        env_handleInteract(gs, (void*)p, TYPE_PERSON);

    } else if (choice <= i + j) {

        const Door *d = current->doors[choice - i - 1];
	printf(löschen);
	printf(leerezeile2);
        printf(weißH schwarz "%-20s" "%-66s" normal "\n", "", d->description);
	printf(leerezeile3);
        env_handleInteract(gs, (void*)d, TYPE_DOOR);

    } else if (choice <= i + j + k) {

        const Objekt *o = current->objects[choice - i - j - 1];
	printf(löschen);
	printf(leerezeile2);
        printf(weißH schwarz "%-20s" "%-66s" normal "\n", "", o->description);
	printf(leerezeile3);
        env_handleInteract(gs, (void*)o, TYPE_OBJECT);


    } else {

	printf(leerezeile10);
	printf(leerezeile4);
        printf(weißH schwarz "%-36s" "Ungültige Wahl." "%-37s" normal "\n", "", "");
	printf(leerezeile10);
	printf(leerezeile4);
    }

    return;

}


void env_handleInteract(GameState *gs, void *prxy , ObjektType typ) {
          int choice;
          int i;

    switch (typ) {


        case TYPE_DOOR: {

            Door *d = (Door*)prxy;


	        printf(leerezeile5);
	        printf(weißH schwarz "%-30s" "What do you want to do?" "%-33s" normal "\n", "", "");
	        printf(leerezeile5);

	        
            for (i = 0 ; i < d -> actionCount; i++) {
                printf(weißH schwarz "%-36s" "%d. %-47s" normal "\n", "", i+1, d -> actions[i].name);
	        }
            printf(leerezeile3);
            printf(weißH schwarz "%-36s" "0. go back to looking... %-25s" normal "\n", "", "");
	        printf(leerezeile4);
            printf(hp100);

            printf("\nYour Choice:  ");

            scanf("%d", &choice);

            if (choice == 0) {
                printf(weißH schwarz "%-34s" "back to looking..." "%-34s" normal "\n", "", "");
                return;
            }


            choice <= i+1 ? d -> actions[choice - 1].execute(gs, d, &d -> actions[choice - 1].token) :

            printf(weißH rot "%-40s" "invalid" "%-39s" normal, "", "");
        }
        break;



        case TYPE_PERSON: {

            Person *p = (Person*)prxy;




	        printf(leerezeile5);
	        printf(weißH schwarz "%-30s" "What do you want to do?" "%-33s" normal "\n", "", "");
	        printf(leerezeile2);



            for (i = 0 ; i < p -> actionCount; i++) {
                printf(weißH schwarz "%-36s" "%d. %-47s" normal "\n", "", i+1, p -> actions[i].name);
		    }
            printf(leerezeile3);
            printf(weißH schwarz "%-36s" "0. go back to looking... %-25s" normal "\n", "", "");
	        printf(leerezeile4);
            printf(hp100);




            printf("\nYour Choice:  ");

            
            scanf("%d", &choice);

            if (choice == 0) {
                printf(weißH schwarz "%-34s" "back to looking..." "%-34s" normal "\n", "", "");
                return;
            }

            choice <= i+1 ? p -> actions[choice - 1].execute(gs, p, &p -> actions[choice - 1].token) :

            printf(weißH rot "%-40s" "invalid" "%-39s" normal, "", "");
        }
        break;




        case TYPE_OBJECT: {

            Objekt *o = (Objekt*)prxy;

	        printf(leerezeile5);
            printf(weißH schwarz "%-30s" "What do you want to do?" "%-33s" normal "\n", "", "");
	        printf(leerezeile2);

            for (i = 0 ; i < o -> actionCount; i++) {
               printf(weißH schwarz "%-36s" "%d. %-47s" normal "\n", "", i+1, o -> actions[i].name);
               }
            printf(leerezeile3);
            printf(weißH schwarz "%-36s" "0. go back to looking... %-25s" normal "\n", "", "");
	        printf(leerezeile4);
            printf(hp100);

            printf("\nYour Choice:  ");

            scanf("%d", &choice);

            if (choice == 0) {
                printf(weißH schwarz "%-34s" "back to looking..." "%-34s" normal "\n", "", "");
                return;
            }

            choice <= i+1 ? o -> actions[choice - 1].execute(gs, o, &o -> actions[choice - 1].token) :
            
            printf(weißH rot "%-40s" "invalid" "%-39s" normal, "", "");
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

	        printf(leerezeile5);
            printf(weißH schwarz "%-20s" "%s: %-66s" normal "\n", "", p -> name, p -> dialog[i]);
	        printf(leerezeile5);
            
            if (p -> decision == false) {

                printf(weißH schwarz "%-20s" "You: ..." "%-58s" normal "\n", "", "");
                printf(weißH schwarz "%-20s" "1. %-67s" normal "\n", "", p -> dialogActionsA[i].name);
                printf(weißH schwarz "%-20s" "2. %-67s" normal "\n", "", p -> dialogActionsB[i].name);
                printf(leerezeile10);
		        printf(hp100);
                

                printf("\nYour Answer: ");



                int d = 0;
                
                while(d == 0) {

                    int ch = 0;
                    scanf("%d", &ch);

                    if (ch == 1) { p -> dialogActionsA[i].execute(gs, prxy, &p -> dialogActionsA[i].token); d = 1; }
                    else if (ch == 2) { p -> dialogActionsB[i].execute(gs, prxy, &p -> dialogActionsB[i].token); d = 1; }
                    else printf(weißH schwarz "%-32s" "You need to answer..." "%-33s" normal "\n", "", "");
            
                }



            }


    }
    p -> decision = true;

} 


void act_openDoor(GameState *gs, void* prxy, Token* token) {

    Door *d = (Door*)prxy;
    if (d -> locked) {
        printf(weißH schwarz "%-33s" "This door is locked." "%-33s" normal "\n", "", "");
	    printf(weißH schwarz "%-34s" "Try using a key..." "%-34s"  normal "\n", "", "" );
        return;
    }   
    else {
	    printf(leerezeile5);
        gs -> currentLocation == d -> targetIndex[0] ? (gs -> currentLocation = d -> targetIndex[1], printf(löschen), printf(leerezeile5), printf(weißH schwarz "%-22s" "You enter %-54s" normal "\n" , "", gs -> locations[gs -> currentLocation].description )) :
        gs -> currentLocation == d -> targetIndex[1] ? (gs -> currentLocation = d -> targetIndex[0], printf(löschen), printf(leerezeile5), printf(weißH schwarz "%-22s" "You enter %-54s" normal "\n", "", gs -> locations[gs -> currentLocation].description )) :
        printf(weißH rot "%-19s" "You are not supposed to be here! (or get there?)" "%-19s" normal "\n", "", "");
        return;
    }
}

// für take() muss token -> Tkns[0] auf das Item pointer array .itemsXXX innerhalb der struktur zeigen


void act_take(GameState *gs, void* prxy, Token* token) {

    Token *t = (Token*)token;

    Item *i = t -> Tkns[0];
    
    if (gs->itemsInvCount >= 10) { printf(weißH schwarz "%-36s" "Inventory full." "%-37s" normal "\n", "", ""); return; }

    gs -> itemsInv[gs -> itemsInvCount] = i;
    gs->itemsInvCount++; 
    
    printf(weißH schwarz "%-31s" "%s added to inventory." "%-33s" normal "\n", "", i -> name, "");

}


// für openComp() muss token -> Tkns[0] auf das Compartments pointer array .compartments innerhalb der struktur zeigen


void act_openComp(GameState *gs, void* prxy, Token* token) {
    
    Token *t = (Token*)token;

    Compartment *c = t -> Tkns[0];
    
    if (c -> locked) {
        printf(weißH schwarz "%-29s" "This compartment is locked." "%-30s" normal "\n", "", "");
         printf(weißH schwarz "%-34s" "Try using a key..." "%-34s" normal "\n", "", "");
        return;
    }
    else {
        if (c -> itemCount == 0) {printf(weißH schwarz "%-35s" "%s is empty" "%-35s"  normal "\n","", c -> name, ""); return;}

        int total = c->itemCount;
        for (int i = 0; i < total; i++) {

            if (c->items[i] == NULL) continue; // Springe zum nächsten Slot
        
            switch (c -> items[i] -> itemType) {

                case ITM_TYPE_TEXT : {
                    
		            printf(löschen);
		            printf(leerezeile5);
                    printf(weißH schwarz "%-12s" "%s: %-62s" normal "\n", "", c -> items[i] -> name, c -> items[i] -> text);
		            printf(leerezeile15);
		            printf(weißH schwarz "%-37s" "Take, y/n?" "%39s" normal "\n", "", "");
		            printf(leerezeile5);
		            printf(hp100);
                    printf("\nYour Choice:  ");

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

void itm_satisfy(GameState *gs, void* prxy, Token* token) {
    
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
        printf(weißH schwarz "%-40s" "*click*" "%-39s" normal "\n", "", "");
        *locked = false;
    }
    else if (*locked == false){
        printf(weißH schwarz "%-40s" "*click*" "%-39s" normal "\n", "", "");
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

            for (int k = 0; k < (int)gs->itemsInvCount; k++) {
                if (gs->itemsInv[k] == toggleItem) {
                    for (int m = k; m < (int)gs->itemsInvCount - 1; m++) gs->itemsInv[m] = gs->itemsInv[m+1];
                    gs->itemsInv[--gs->itemsInvCount] = NULL;
                    break;
                }
            }
            return;
        }
        i++;
    }

    printf("\n\n\n%s cannot be used here.\n\n\n", toggleItem -> name);
    return;



    
}

Item* env_openInventory(GameState *gs) {

    printf(löschen);
    printf(leerezeile5);
    printf(weißH schwarz "%-38s" "Your items:" "%-37s" normal "\n", "", "");
    printf(leerezeile5);

    for (int i = 0; i < gs -> itemsInvCount; i++) {
        printf(weißH schwarz "%-35s" "%d. %s:" "%-37s" normal "\n", "", i+1, gs -> itemsInv[i] -> name, "");
        printf(weißH schwarz "%-23s" "%-49s" normal "\n", "", gs -> itemsInv[i] -> text);
    }
    printf(leerezeile10);
    printf(leerezeile4);
    printf(hp100);



    printf("\nYour Choice:  ");

    int choice;
    scanf("%d", &choice);

    if (choice == 0) return NULL;
    
    
    if (choice <= gs -> itemsInvCount) {
       
        return gs -> itemsInv[choice-1];
    }

    printf(weißH schwarz "%-35s" "Pockets to deep?" "%-35s" normal "\n", "", "");
    return NULL;

    

}

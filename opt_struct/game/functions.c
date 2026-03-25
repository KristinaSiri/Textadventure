#include <stdio.h>
#include <string.h>
#include "gamestate.h"
#include "farben.h"
#include "menu.h"
#include "lebensanzeige.h"

void act_openDoor(GameState *gs, void* prxy, Token* token);
void act_openComp(GameState *gs, void* prxy, Token* token);

void itm_satisfy(GameState *gs, void* prxy, Token* token);

void sat_UnOrLock(GameState *gs, void* prxy, Token* token);

void act_giveItem(GameState *gs, void* prxy, Token* token);

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
    KEY_TEST,
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
            {.name = "open door", .execute = act_openDoor},
            {.name = "use item", .execute = act_giveItem}
        },
        .actionCount = 1,
        .locked = true,

        .sat = {
            .level = 1, 
            .satActions = {
                {.execute = sat_UnOrLock, .token = { .Tkns = { &doorPool[DOOR_START_CHAMBER].locked } }}
            }, 
            .satActionCount = 1
        }
    }
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

    [KEY_TEST] = {
        .name = "Key door 1",
        .description = "This key might be usefull to open the first door.",

        .itemType = ITM_TYPE_TEXT,

        .itmActions = {
            { .name = "Key 1", .execute = itm_satisfy, .token = { .Tkns = {&doorPool[DOOR_START_CHAMBER].sat} } }
        },
        .itmActionCount = 1
    }
};

static  Compartment compPool[COMP_COUNT] = {


    [COMP_TEST] = {
        .name = "Broken drawer",

        .items = { &itemPool[ITM_TEST_ZETTEL], &itemPool[KEY_TEST] },
        .itemCount = 2,

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
        .description = "A damp cave stone.",

// braucht noch eine .actions damit man etwas auswählen kann

   },
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
    [NPC_GUARD] = { "guard", "A guard keeps his distance." }
};



static Location worldData[LOC_COUNT] = {
    [LOC_START] = {
        .tag = "start",
        .description = "a dark cave",
        .doors = { &doorPool[DOOR_START_CHAMBER] },
        .doorCount = 1,
        .objects = { &objectPool[OBJ_STONE], &objectPool[OBJ_TABLE] },
        .objectCount = 2,
        .persons = { &personPool[NPC_MINER] },
        .personCount = 1
    },

    [LOC_CHAMBER] = {
        .tag = "chamber",
        .description = "a torch lit chamber",
        .doors = { &doorPool[DOOR_START_CHAMBER] },
        .doorCount = 1,
        .objects = {  },
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

    printf(löschen);
    printf(leerezeile5);
    printf(weißH schwarz "%-31s" "You look around and see:" "%-31s" normal "\n", "", "");



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
	printf(leerezeile5);
        printf(weißH schwarz "%-28s" "%-58s" normal "\n", "", p->description);
	printf(leerezeile5);
        env_handleInteract(gs, (void*)p, TYPE_PERSON);

    } else if (choice <= i + j) {

        const Door *d = current->doors[choice - i - 1];
	printf(löschen);
	printf(leerezeile5);
        printf(weißH schwarz "%-28s" "%-58s" normal "\n", "", d->description);
	printf(leerezeile5);
        env_handleInteract(gs, (void*)d, TYPE_DOOR);

    } else if (choice <= i + j + k) {

        const Objekt *o = current->objects[choice - i - j - 1];
	printf(löschen);
	printf(leerezeile5);
        printf(weißH schwarz "%-28s" "%-58s" normal "\n", "", o->description);
	printf(leerezeile5);
        env_handleInteract(gs, (void*)o, TYPE_OBJECT);


    } else {

	printf(leerezeile10);
	printf(leerezeile4);
        printf("Ungültige Wahl.\n");
	printf(leerezeile10);
	printf(leerezeile4);
    }

    return;

}




void env_handleInteract(GameState *gs, void *prxy , ObjektType typ) {
    switch (typ) {
        
        
        int choice;
        int i;

        case TYPE_DOOR: {

            Door *d = (Door*)prxy;
            printf(weißH schwarz "%-30s" "What do you want to do?" "%-33s" normal "\n", "", "");
	    printf(leerezeile2);

            for (i = 0 ; i < d -> actionCount; i++) {
                printf(weißH schwarz "%-36s" "%d. %-47s" normal "\n", "", i+1, d -> actions[i].name);
		printf(leerezeile10);
		printf(leerezeile2);
		printf(hp100);
            }


            printf("\nYour Choice:  ");

            
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
	    printf(weißH schwarz "%-30s" "What do you want to do?" "%-33s" normal "\n", "", "");
	    printf(leerezeile2);

            for (i = 0 ; i < p -> actionCount; i++) {
                printf(weißH schwarz "%-36s" "%d. %-47s" normal "\n", "", i+1, p -> actions[i].name);
                printf(leerezeile10);
                printf(leerezeile2);
                printf(hp100);
            }



            printf("\nYour Choice:  ");

            
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
            printf(weißH schwarz "%-30s" "What do you want to do?" "%-33s" normal "\n", "", "");
	    printf(leerezeile2);

            for (i = 0 ; i < o -> actionCount; i++) {
               printf(weißH schwarz "%-36s" "%d. %-47s" normal "\n", "", i+1, o -> actions[i].name);
               printf(leerezeile10);
               printf(leerezeile2);
               printf(hp100);
	       }



            printf("\nYour Choice:  ");

            
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
            scanf("%d", &ch);

            if (ch == 1) { p -> dialogActionsA -> execute(gs, prxy, &p -> dialogActionsA -> token); decsion = 1; }
            else if (ch == 2) { p -> dialogActionsB -> execute(gs, prxy, &p -> dialogActionsB -> token); decsion = 1; }
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
        gs -> currentLocation == d -> targetIndex[0] ? gs -> currentLocation = d -> targetIndex[1] :
        gs -> currentLocation == d -> targetIndex[1] ? gs -> currentLocation = d -> targetIndex[0] :
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
        if (c -> itemCount == 0) {printf("%s is empty", c -> name); return;}

        for (int i = 0; i < c -> itemCount; i++) {

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
        printf("*click*");
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


    printf("\nYour Choice:  ");

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

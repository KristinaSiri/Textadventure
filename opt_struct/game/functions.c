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

    /* ============== NEW ESTATE CONTENT START ============== */
    LOC_MAIN_HALL,
    LOC_LIBRARY,
    LOC_STUDY,
    LOC_TROPHY_ROOM,
    LOC_STATUE_ALCOVE,
    LOC_CELLAR,
    LOC_WINE_VAULT,
    LOC_UPPER_LANDING,
    LOC_BEDROOM,
    LOC_OBSERVATORY,
    LOC_VAULT_ANTECHAMBER,
    LOC_VAULT,
    /* ============== NEW ESTATE CONTENT END ================ */
    LOC_COUNT
};

enum { 

    ENT_PATH_ESTATE,

    ENT_DRIVEWAY_GATEHOUSE,

    ENT_GATEHOUSE_OFFICE,
    ENT_GATEHOUSE_CLOSET,
    ENT_GATEHOUSE_GUARDSHK,
    
    ENT_GATEHOUSE_MAIN,

    /* ============== NEW ESTATE CONTENT START ============== */
    ENT_ENTRANCE_HALL,
    ENT_HALL_LIBRARY,
    ENT_LIBRARY_STUDY,
    ENT_HALL_TROPHY,
    ENT_TROPHY_ALCOVE,
    ENT_HALL_CELLAR,
    ENT_CELLAR_WINE,
    ENT_HALL_UPPER,
    ENT_UPPER_BEDROOM,
    ENT_UPPER_OBSERVATORY,
    ENT_OBSERVATORY_ANTE,
    ENT_ANTE_VAULT,
    /* ============== NEW ESTATE CONTENT END ================ */
    DOOR_COUNT
    
};

enum { 

    OBJ_WORKDESK, 
    OBJ_STONE,
    
    OBJ_SIDEBOARD,
    OBJ_LAMP,

    /* ============== NEW ESTATE CONTENT START ============== */
    OBJ_BOOKSHELF,
    OBJ_TROPHY_CASE,
    OBJ_STATUE,
    OBJ_WINE_RACK,
    OBJ_BEDSIDE_TABLE,
    OBJ_TELESCOPE,
    OBJ_VAULT_MECHANISM,
    OBJ_VAULT_CHEST,
    /* ============== NEW ESTATE CONTENT END ================ */
    OBJ_COUNT
};

enum { 

    NPC_MINER, 
    NPC_GUARD,
    NPC_GRANNY,

    /* ============== NEW ESTATE CONTENT START ============== */
    NPC_LIBRARIAN,
    NPC_PHILOSOPHER,
    NPC_WINEMASTER,
    NPC_CARETAKER,
    /* ============== NEW ESTATE CONTENT END ================ */
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

    /* ============== NEW ESTATE CONTENT START ============== */
    ITM_ECHO_BOOK,
    ITM_TORCH,
    ITM_ESTATE_KEY,
    ITM_STAR_MAP,
    ITM_VAULT_SIGIL,
    ITM_GOLDEN_CROWN,
    /* ============== NEW ESTATE CONTENT END ================ */

    ITM_COUNT
};

enum {

    COMP_EMPTY,

    COMP_DESK_LOCKED,
    COMP_GATEH_WD_LS,

    /* ============== NEW ESTATE CONTENT START ============== */
    COMP_BOOKSHELF_SECRET,
    COMP_STATUE_BASE,
    COMP_BEDSIDE,
    COMP_TELESCOPE_BASE,
    COMP_VAULT_CHEST_C,
    /* ============== NEW ESTATE CONTENT END ================ */

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


    /* ============== NEW ESTATE CONTENT START ============== */

    [ENT_ENTRANCE_HALL] = {
        .name = "grand entrance",
        .description = "A pair of tall oak doors, slightly ajar, lead into the estate's main hall.",
        .targetIndex = { LOC_MAIN_ENTRANCE, LOC_MAIN_HALL },
        .actions = {
            {.name = "enter", .execute = act_openDoor}
        },
        .actionCount = 1,
        .locked = false
    },

    [ENT_HALL_LIBRARY] = {
        .name = "library door",
        .description = "An arched doorway with carved lettering: 'SILENTIUM'.",
        .targetIndex = { LOC_MAIN_HALL, LOC_LIBRARY },
        .actions = {
            {.name = "go through", .execute = act_openDoor}
        },
        .actionCount = 1,
        .locked = false
    },

    [ENT_LIBRARY_STUDY] = {
        .name = "study passage",
        .description = "A narrow passage connects the library to a private study.",
        .targetIndex = { LOC_LIBRARY, LOC_STUDY },
        .actions = {
            {.name = "go through", .execute = act_openDoor}
        },
        .actionCount = 1,
        .locked = false
    },

    [ENT_HALL_TROPHY] = {
        .name = "trophy room door",
        .description = "A heavy iron door with hunting motifs etched into the metal.",
        .targetIndex = { LOC_MAIN_HALL, LOC_TROPHY_ROOM },
        .actions = {
            {.name = "enter", .execute = act_openDoor}
        },
        .actionCount = 1,
        .locked = false
    },

    [ENT_TROPHY_ALCOVE] = {
        .name = "alcove archway",
        .description = "A stone archway leads deeper into a shadowed alcove with a large statue.",
        .targetIndex = { LOC_TROPHY_ROOM, LOC_STATUE_ALCOVE },
        .actions = {
            {.name = "go through", .execute = act_openDoor}
        },
        .actionCount = 1,
        .locked = false
    },

    [ENT_HALL_CELLAR] = {
        .name = "cellar hatch",
        .description = "A reinforced trapdoor in the floor, sealed with an old mechanism.",
        .targetIndex = { LOC_MAIN_HALL, LOC_CELLAR },
        .actions = {
            {.name = "open hatch", .execute = act_openDoor},
            {.name = "use item", .execute = act_giveItem, .token = { .Tkns = { &doorPool[ENT_HALL_CELLAR].wants } } }
        },
        .wants = {&itemPool[ITM_ECHO_BOOK]},
        .wantsCount = 1,
        .actionCount = 2,
        .locked = true,
        .sat = {
            .level = 1,
            .satActions = {
                {.execute = sat_UnOrLock, .token = { .Tkns = { &doorPool[ENT_HALL_CELLAR].locked } } }
            },
            .satActionCount = 1
        }
    },

    [ENT_CELLAR_WINE] = {
        .name = "wine vault entrance",
        .description = "A low archway smelling of old oak and fermented grapes.",
        .targetIndex = { LOC_CELLAR, LOC_WINE_VAULT },
        .actions = {
            {.name = "go through", .execute = act_openDoor}
        },
        .actionCount = 1,
        .locked = false
    },

    [ENT_HALL_UPPER] = {
        .name = "grand staircase gate",
        .description = "An ornate wrought-iron gate blocks the grand staircase leading up.",
        .targetIndex = { LOC_MAIN_HALL, LOC_UPPER_LANDING },
        .actions = {
            {.name = "open gate", .execute = act_openDoor},
            {.name = "use item", .execute = act_giveItem, .token = { .Tkns = { &doorPool[ENT_HALL_UPPER].wants } } }
        },
        .wants = {&itemPool[ITM_ESTATE_KEY]},
        .wantsCount = 1,
        .actionCount = 2,
        .locked = true,
        .sat = {
            .level = 1,
            .satActions = {
                {.execute = sat_UnOrLock, .token = { .Tkns = { &doorPool[ENT_HALL_UPPER].locked } } }
            },
            .satActionCount = 1
        }
    },

    [ENT_UPPER_BEDROOM] = {
        .name = "bedroom door",
        .description = "A faded wooden door with a brass handle, slightly warm to the touch.",
        .targetIndex = { LOC_UPPER_LANDING, LOC_BEDROOM },
        .actions = {
            {.name = "open door", .execute = act_openDoor}
        },
        .actionCount = 1,
        .locked = false
    },

    [ENT_UPPER_OBSERVATORY] = {
        .name = "observatory stairwell",
        .description = "A spiral staircase winds upward into a domed tower.",
        .targetIndex = { LOC_UPPER_LANDING, LOC_OBSERVATORY },
        .actions = {
            {.name = "open door", .execute = act_openDoor}
        },
        .actionCount = 1,
        .locked = false
    },

    [ENT_OBSERVATORY_ANTE] = {
        .name = "sealed passage",
        .description = "A heavy stone slab with a circular recess in its center blocks the way.",
        .targetIndex = { LOC_OBSERVATORY, LOC_VAULT_ANTECHAMBER },
        .actions = {
            {.name = "open passage", .execute = act_openDoor},
            {.name = "use item", .execute = act_giveItem, .token = { .Tkns = { &doorPool[ENT_OBSERVATORY_ANTE].wants } } }
        },
        .wants = {&itemPool[ITM_VAULT_SIGIL]},
        .wantsCount = 1,
        .actionCount = 2,
        .locked = true,
        .sat = {
            .level = 1,
            .satActions = {
                {.execute = sat_UnOrLock, .token = { .Tkns = { &doorPool[ENT_OBSERVATORY_ANTE].locked } } }
            },
            .satActionCount = 1
        }
    },

    [ENT_ANTE_VAULT] = {
        .name = "vault door",
        .description = "A massive iron vault door, cold to the touch. There is no keyhole — only a lever slot.",
        .targetIndex = { LOC_VAULT_ANTECHAMBER, LOC_VAULT },
        .actions = {
            {.name = "open door", .execute = act_openDoor}
        },
        .actionCount = 1,
        .locked = true
    },

    /* ============== NEW ESTATE CONTENT END ================ */
    
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

    /* ============== NEW ESTATE CONTENT START ============== */

    [ITM_ECHO_BOOK] = {
        .name = "Echo Book",
        .text = "A leather-bound tome. The pages whisper when turned, repeating your words back in fading voices.",
        .itemType = ITM_TYPE_TEXT,
        .itmActions = {
            { .name = "Echo book", .execute = itm_satisfy, .token = { .Tkns = {&doorPool[ENT_HALL_CELLAR].sat} } }
        },
        .itmActionCount = 1
    },

    [ITM_TORCH] = {
        .name = "Ancient Torch",
        .text = "A tar-soaked torch wrapped in linen. It burns with an unnaturally steady blue flame.",
        .itemType = ITM_TYPE_TEXT,
        .itmActions = {
            { .name = "Torch", .execute = itm_satisfy, .token = { .Tkns = {&personPool[NPC_WINEMASTER].sat} } }
        },
        .itmActionCount = 1
    },

    [ITM_ESTATE_KEY] = {
        .name = "Estate Key",
        .text = "An ornate iron key with the estate crest embossed on the bow. It hums faintly.",
        .itemType = ITM_TYPE_TEXT,
        .itmActions = {
            { .name = "Estate key", .execute = itm_satisfy, .token = { .Tkns = {&doorPool[ENT_HALL_UPPER].sat} } }
        },
        .itmActionCount = 1
    },

    [ITM_STAR_MAP] = {
        .name = "Star Map",
        .text = "A brittle parchment charting constellations. One star is circled in red ink: 'Polaris Oculi'.",
        .itemType = ITM_TYPE_TEXT,
        .itmActions = {
            { .name = "Star map", .execute = env_unhideActions, .token = { .Tkns = {&objectPool[OBJ_TELESCOPE].actionCount} } }
        },
        .itmActionCount = 1
    },

    [ITM_VAULT_SIGIL] = {
        .name = "Vault Sigil",
        .text = "A heavy stone disc etched with concentric rings and celestial symbols. It fits a door recess.",
        .itemType = ITM_TYPE_TEXT,
        .itmActions = {
            { .name = "Vault sigil", .execute = itm_satisfy, .token = { .Tkns = {&doorPool[ENT_OBSERVATORY_ANTE].sat} } }
        },
        .itmActionCount = 1
    },

    [ITM_GOLDEN_CROWN] = {
        .name = "The Golden Crown",
        .text = "A crown of pure gold, studded with sapphires. The estate's lost treasure, finally reclaimed.",
        .itemType = ITM_TYPE_TEXT,
        .itmActionCount = 0
    },

    /* ============== NEW ESTATE CONTENT END ================ */
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
        .name = "empty comp",

    },

    /* ============== NEW ESTATE CONTENT START ============== */

    [COMP_BOOKSHELF_SECRET] = {
        .name = "hidden shelf",
        .items = { &itemPool[ITM_ECHO_BOOK] },
        .itemCount = 1,
        .locked = false
    },

    [COMP_STATUE_BASE] = {
        .name = "hollow statue base",
        .items = { &itemPool[ITM_TORCH] },
        .itemCount = 1,
        .locked = false
    },

    [COMP_BEDSIDE] = {
        .name = "bedside drawer",
        .items = { &itemPool[ITM_STAR_MAP] },
        .itemCount = 1,
        .locked = false
    },

    [COMP_TELESCOPE_BASE] = {
        .name = "telescope compartment",
        .items = { &itemPool[ITM_VAULT_SIGIL] },
        .itemCount = 1,
        .locked = false
    },

    [COMP_VAULT_CHEST_C] = {
        .name = "ancient vault chest",
        .items = { &itemPool[ITM_GOLDEN_CROWN] },
        .itemCount = 1,
        .locked = false
    },

    /* ============== NEW ESTATE CONTENT END ================ */
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

    /* ============== NEW ESTATE CONTENT START ============== */

    [OBJ_BOOKSHELF] = {
        .name = "towering bookshelf",
        .description = "Floor-to-ceiling shelves crammed with dusty volumes. Some spines are unreadable with age.",
        .actions = {
            {.name = "look behind the books", .execute = act_openComp, .token = { .Tkns = { &compPool[COMP_BOOKSHELF_SECRET] } } }
        },
        .actionCount = 0,
        .compartmens = {&compPool[COMP_BOOKSHELF_SECRET]},
        .compaCount = 1
    },

    [OBJ_TROPHY_CASE] = {
        .name = "trophy case",
        .description = "A glass display case filled with faded medals, antlers, and a stuffed raven.",
        .actions = {
            {.name = "open the glass panel", .execute = act_openComp, .token = { .Tkns = { &compPool[COMP_EMPTY] } } }
        },
        .actionCount = 1,
        .compartmens = {&compPool[COMP_EMPTY]},
        .compaCount = 1
    },

    [OBJ_STATUE] = {
        .name = "marble statue",
        .description = "A weathered marble figure of a man carrying the world on his shoulders. The base looks hollow.",
        .actions = {
            {.name = "lift the base", .execute = act_openComp, .token = { .Tkns = { &compPool[COMP_STATUE_BASE] } } }
        },
        .actionCount = 0,
        .compartmens = {&compPool[COMP_STATUE_BASE]},
        .compaCount = 1
    },

    [OBJ_WINE_RACK] = {
        .name = "wine rack",
        .description = "Rows upon rows of dusty bottles, most long since turned to vinegar.",
        .actions = {
            {.name = "browse the bottles", .execute = act_openComp, .token = { .Tkns = { &compPool[COMP_EMPTY] } } }
        },
        .actionCount = 1,
        .compartmens = {&compPool[COMP_EMPTY]},
        .compaCount = 1
    },

    [OBJ_BEDSIDE_TABLE] = {
        .name = "bedside table",
        .description = "A small nightstand with a single drawer. A candle stub sits on top, long extinguished.",
        .actions = {
            {.name = "open the drawer", .execute = act_openComp, .token = { .Tkns = { &compPool[COMP_BEDSIDE] } } }
        },
        .actionCount = 1,
        .compartmens = {&compPool[COMP_BEDSIDE]},
        .compaCount = 1
    },

    [OBJ_TELESCOPE] = {
        .name = "brass telescope",
        .description = "A grand telescope aimed at the night sky through a crack in the dome. Its lens is fogged.",
        .actions = {
            {.name = "use star map", .execute = act_giveItem, .token = { .Tkns = { &objectPool[OBJ_TELESCOPE].wants } } },
            {.name = "peer through the lens", .execute = act_openComp, .token = { .Tkns = { &compPool[COMP_TELESCOPE_BASE] } } }
        },
        .actionCount = 1,
        .wants = {&itemPool[ITM_STAR_MAP]},
        .wantsCount = 1,
        .compartmens = {&compPool[COMP_TELESCOPE_BASE]},
        .compaCount = 1
    },

    [OBJ_VAULT_MECHANISM] = {
        .name = "ancient mechanism",
        .description = "A complex arrangement of gears and levers embedded in the wall, covered in dust.",
        .actions = {
            {.name = "pull the lever", .execute = sat_UnOrLock, .token = { .Tkns = { &doorPool[ENT_ANTE_VAULT].locked } } }
        },
        .actionCount = 0
    },

    [OBJ_VAULT_CHEST] = {
        .name = "ornate chest",
        .description = "A magnificent gilded chest sits alone on a stone pedestal, untouched for centuries.",
        .actions = {
            {.name = "open the chest", .execute = act_openComp, .token = { .Tkns = { &compPool[COMP_VAULT_CHEST_C] } } }
        },
        .actionCount = 1,
        .compartmens = {&compPool[COMP_VAULT_CHEST_C]},
        .compaCount = 1
    },

    /* ============== NEW ESTATE CONTENT END ================ */
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

    /* ============== NEW ESTATE CONTENT START ============== */

    [NPC_LIBRARIAN] = {
        .name = "spectral librarian",
        .description = "A translucent figure hovers between the shelves, adjusting books with ghostly fingers.",

        .dialog = {
            "I speak without a mouth and hear without ears. I have no body, but I come alive with the wind. What am I?",
            "Test",
        },

        .actions = {
            {.name = "talk", .execute = act_talk}
        },
        .actionCount = 1,
        .diaCount = 1,

        .dialogActionsA = {
            {.name = "An echo", .execute = env_unhideActions, .token = { .Tkns = {&objectPool[OBJ_BOOKSHELF].actionCount} } }
        },
        .dialogActionsB = {
            {.name = "A ghost", .execute = act_openComp, .token = { .Tkns = { &compPool[COMP_EMPTY] } } }
        }
    },

    [NPC_PHILOSOPHER] = {
        .name = "stone philosopher",
        .description = "A figure carved from living stone sits cross-legged. Its eyes follow you. It speaks in grinding tones.",

        .dialog = {
            "What walks on four legs at dawn, two legs at noon, and three legs at dusk?",
            "Test",
        },

        .actions = {
            {.name = "talk", .execute = act_talk}
        },
        .actionCount = 1,
        .diaCount = 1,

        .dialogActionsA = {
            {.name = "The sun", .execute = act_openComp, .token = { .Tkns = { &compPool[COMP_EMPTY] } } }
        },
        .dialogActionsB = {
            {.name = "Man", .execute = env_unhideActions, .token = { .Tkns = {&objectPool[OBJ_STATUE].actionCount} } }
        }
    },

    [NPC_WINEMASTER] = {
        .name = "old winemaster",
        .description = "An ancient man sits among the barrels, shivering. His lantern has long since gone out.",

        .dialog = {
            "Cold and dark, this vault has been for years. My lantern died. Bring me a flame and I shall part with the master's key to the upper chambers.",
            "Test",
        },

        .actions = {
            {.name = "give item", .execute = act_giveItem, .token = { .Tkns = { &personPool[NPC_WINEMASTER].wants } } },
            {.name = "talk", .execute = act_talk}
        },
        .actionCount = 2,
        .diaCount = 1,

        .wants = {&itemPool[ITM_TORCH]},
        .wantsCount = 1,

        .dialogActionsA = {
            {.name = "I will find you a flame", .execute = act_openComp, .token = { .Tkns = { &compPool[COMP_EMPTY] } } }
        },
        .dialogActionsB = {
            {.name = "Sit in the dark then", .execute = act_openComp, .token = { .Tkns = { &compPool[COMP_EMPTY] } } }
        },

        .sat = {
            .level = 1,
            .satActions = {
                {.execute = act_take, .token = { .Tkns = { &itemPool[ITM_ESTATE_KEY] } } }
            },
            .satActionCount = 1
        }
    },

    [NPC_CARETAKER] = {
        .name = "blind caretaker",
        .description = "A hunched figure in tattered robes guards the passage. Blind eyes stare through you.",

        .dialog = {
            "The more of them you take, the more of them you leave behind. What are they?",
            "Test",
        },

        .actions = {
            {.name = "talk", .execute = act_talk}
        },
        .actionCount = 1,
        .diaCount = 1,

        .dialogActionsA = {
            {.name = "Memories", .execute = act_openComp, .token = { .Tkns = { &compPool[COMP_EMPTY] } } }
        },
        .dialogActionsB = {
            {.name = "Footsteps", .execute = env_unhideActions, .token = { .Tkns = {&objectPool[OBJ_VAULT_MECHANISM].actionCount} } }
        }
    },

    /* ============== NEW ESTATE CONTENT END ================ */
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
        .doors = { &doorPool[ENT_PATH_ESTATE], &doorPool[ENT_DRIVEWAY_GATEHOUSE] },
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
        .doors = { &doorPool[ENT_GATEHOUSE_MAIN], &doorPool[ENT_ENTRANCE_HALL] },
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

    /* ============== NEW ESTATE CONTENT START ============== */

    [LOC_MAIN_HALL] = {
        .tag = "main hall",
        .description = "a grand hall with vaulted ceilings and faded tapestries depicting hunts and feasts",
        .doors = { &doorPool[ENT_ENTRANCE_HALL], &doorPool[ENT_HALL_LIBRARY], &doorPool[ENT_HALL_TROPHY], &doorPool[ENT_HALL_CELLAR], &doorPool[ENT_HALL_UPPER] },
        .doorCount = 5,
        .objects = {  },
        .objectCount = 0,
        .persons = {  },
        .personCount = 0
    },

    [LOC_LIBRARY] = {
        .tag = "library",
        .description = "a vast library choked with dust — shelves reach into shadow above",
        .doors = { &doorPool[ENT_HALL_LIBRARY], &doorPool[ENT_LIBRARY_STUDY] },
        .doorCount = 2,
        .objects = { &objectPool[OBJ_BOOKSHELF] },
        .objectCount = 1,
        .persons = { &personPool[NPC_LIBRARIAN] },
        .personCount = 1
    },

    [LOC_STUDY] = {
        .tag = "study",
        .description = "a small private study — ink-stained papers litter the writing desk",
        .doors = { &doorPool[ENT_LIBRARY_STUDY] },
        .doorCount = 1,
        .objects = {  },
        .objectCount = 0,
        .persons = {  },
        .personCount = 0
    },

    [LOC_TROPHY_ROOM] = {
        .tag = "trophy room",
        .description = "mounted heads and dusty weapons line the walls of this grim gallery",
        .doors = { &doorPool[ENT_HALL_TROPHY], &doorPool[ENT_TROPHY_ALCOVE] },
        .doorCount = 2,
        .objects = { &objectPool[OBJ_TROPHY_CASE] },
        .objectCount = 1,
        .persons = {  },
        .personCount = 0
    },

    [LOC_STATUE_ALCOVE] = {
        .tag = "statue alcove",
        .description = "a dim alcove dominated by a towering marble statue carrying the weight of the world",
        .doors = { &doorPool[ENT_TROPHY_ALCOVE] },
        .doorCount = 1,
        .objects = { &objectPool[OBJ_STATUE] },
        .objectCount = 1,
        .persons = { &personPool[NPC_PHILOSOPHER] },
        .personCount = 1
    },

    [LOC_CELLAR] = {
        .tag = "cellar",
        .description = "a cold stone cellar — water drips from the ceiling and the air smells of earth",
        .doors = { &doorPool[ENT_HALL_CELLAR], &doorPool[ENT_CELLAR_WINE] },
        .doorCount = 2,
        .objects = {  },
        .objectCount = 0,
        .persons = {  },
        .personCount = 0
    },

    [LOC_WINE_VAULT] = {
        .tag = "wine vault",
        .description = "a vaulted chamber lined with oak barrels and dusty bottle racks",
        .doors = { &doorPool[ENT_CELLAR_WINE] },
        .doorCount = 1,
        .objects = { &objectPool[OBJ_WINE_RACK] },
        .objectCount = 1,
        .persons = { &personPool[NPC_WINEMASTER] },
        .personCount = 1
    },

    [LOC_UPPER_LANDING] = {
        .tag = "upper landing",
        .description = "a moonlit landing at the top of the grand staircase — portraits watch from the walls",
        .doors = { &doorPool[ENT_HALL_UPPER], &doorPool[ENT_UPPER_BEDROOM], &doorPool[ENT_UPPER_OBSERVATORY] },
        .doorCount = 3,
        .objects = {  },
        .objectCount = 0,
        .persons = {  },
        .personCount = 0
    },

    [LOC_BEDROOM] = {
        .tag = "bedroom",
        .description = "a dusty master bedroom — the four-poster bed still has its moth-eaten curtains drawn",
        .doors = { &doorPool[ENT_UPPER_BEDROOM] },
        .doorCount = 1,
        .objects = { &objectPool[OBJ_BEDSIDE_TABLE] },
        .objectCount = 1,
        .persons = {  },
        .personCount = 0
    },

    [LOC_OBSERVATORY] = {
        .tag = "observatory",
        .description = "a domed tower room — starlight streams through cracks in the ceiling onto a grand telescope",
        .doors = { &doorPool[ENT_UPPER_OBSERVATORY], &doorPool[ENT_OBSERVATORY_ANTE] },
        .doorCount = 2,
        .objects = { &objectPool[OBJ_TELESCOPE] },
        .objectCount = 1,
        .persons = {  },
        .personCount = 0
    },

    [LOC_VAULT_ANTECHAMBER] = {
        .tag = "vault antechamber",
        .description = "a narrow stone chamber before the vault — gears and levers protrude from the walls",
        .doors = { &doorPool[ENT_OBSERVATORY_ANTE], &doorPool[ENT_ANTE_VAULT] },
        .doorCount = 2,
        .objects = { &objectPool[OBJ_VAULT_MECHANISM] },
        .objectCount = 1,
        .persons = { &personPool[NPC_CARETAKER] },
        .personCount = 1
    },

    [LOC_VAULT] = {
        .tag = "the vault",
        .description = "a glittering vault — gold and jewels are piled high, and at the center sits a magnificent chest",
        .doors = { &doorPool[ENT_ANTE_VAULT] },
        .doorCount = 1,
        .objects = { &objectPool[OBJ_VAULT_CHEST] },
        .objectCount = 1,
        .persons = {  },
        .personCount = 0
    }

    /* ============== NEW ESTATE CONTENT END ================ */
    
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

    if (o->compaCount >= 3) { printf("\n\n\nNo more compartment slots.\n\n\n"); return; }

    o -> compartmens[o -> compaCount] = t -> Tkns[1];
    o -> compaCount ++;
}



void env_handleLook(GameState *gs) 
{
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

            while (1) {
            printf(weißH schwarz "%-30s" "What do you want to do?" "%-33s" normal "\n", "", "");
	    printf(leerezeile2);

            for (i = 0 ; i < d -> actionCount; i++) {
                printf(weißH schwarz "%-36s" "%d. %-47s" normal "\n", "", i+1, d -> actions[i].name);
	    }
            printf(leerezeile10);
            printf(leerezeile2);
            printf(hp100);

            printf("\nYour Choice:  ");

            scanf("%d", &choice);

            if (choice == 0) {
                printf("\n\n\nback to looking...\n\n\n");
                return;
            }

            choice <= i+1 ? d -> actions[choice - 1].execute(gs, d, &d -> actions[choice - 1].token) :

            printf("invalid");
            }
            }
            break;




        

        case TYPE_PERSON: {

            Person *p = (Person*)prxy;

            while (1) {
	    printf(weißH schwarz "%-30s" "What do you want to do?" "%-33s" normal "\n", "", "");
	    printf(leerezeile2);

            for (i = 0 ; i < p -> actionCount; i++) {
                printf(weißH schwarz "%-36s" "%d. %-47s" normal "\n", "", i+1, p -> actions[i].name);
		}
            printf(leerezeile10);
            printf(leerezeile2);
            printf(hp100);




            printf("\nYour Choice:  ");

            
            scanf("%d", &choice);

            if (choice == 0) {
                printf("\n\n\nback to looking...\n\n\n");
                return;
            }

            choice <= i+1 ? p -> actions[choice - 1].execute(gs, p, &p -> actions[choice - 1].token) :
            printf("invalid");
            }
            }
            break;
        
        




        case TYPE_OBJECT: {

            Objekt *o = (Objekt*)prxy;

            while (1) {
            printf(weißH schwarz "%-30s" "What do you want to do?" "%-33s" normal "\n", "", "");
	    printf(leerezeile2);

            for (i = 0 ; i < o -> actionCount; i++) {
               printf(weißH schwarz "%-36s" "%d. %-47s" normal "\n", "", i+1, o -> actions[i].name);
               }
            printf(leerezeile10);
            printf(leerezeile2);
            printf(hp100);

            printf("\nYour Choice:  ");
            
            scanf("%d", &choice);

            if (choice == 0) {
                printf("\n\n\nback to looking...\n\n\n");
                return;
            }

            choice <= i+1 ? o -> actions[choice - 1].execute(gs, o, &o -> actions[choice - 1].token) :
            
            printf("invalid");
            }
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
            
            if (p -> decision == false) {

                printf("\n\nYou: ...\n");
                printf("\n1. %s\n", p -> dialogActionsA[i].name);
                printf("\n2. %s\n", p -> dialogActionsB[i].name);

                

                printf("\nYour Answer; ");



                int d = 0;
                
                while(d == 0) {

                    int ch = 0;
                    scanf("%d", &ch);

                    if (ch == 1) { p -> dialogActionsA[i].execute(gs, prxy, &p -> dialogActionsA[i].token); d = 1; }
                    else if (ch == 2) { p -> dialogActionsB[i].execute(gs, prxy, &p -> dialogActionsB[i].token); d = 1; }
                    else printf("\n\nYou need to answer...");
            
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
        gs -> currentLocation == d -> targetIndex[0] ? (gs -> currentLocation = d -> targetIndex[1], printf(weißH schwarz "%-35s" "You enter %-41s" normal "\n" , "", gs -> locations[gs -> currentLocation].description )) :
        gs -> currentLocation == d -> targetIndex[1] ? (gs -> currentLocation = d -> targetIndex[0], printf(weißH schwarz "%-35s" "You enter %-41s" normal "\n", "", gs -> locations[gs -> currentLocation].description )) :
        printf(weißH rot "%-19s" "You are not supposed to be here! (or get there?)" "%-19s" normal "\n", "", "");
        return;
    }
}

// für take() muss token -> Tkns[0] auf das Item pointer array .itemsXXX innerhalb der struktur zeigen


void act_take(GameState *gs, void* prxy, Token* token) {

    Token *t = (Token*)token;

    Item *i = t -> Tkns[0];
    
    if (gs->itemsInvCount >= 10) { printf("\n\n\nInventory full.\n\n\n"); return; }

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

#pragma once

typedef enum EntAction
{ //When adding an action, create the LUA constant in entluafuncs.c
    EA_NONE,
    EA_ATTACK,
    EA_BLOCK,
    EA_THROW,
    EA_PUSH,
    EA_PICKUP,
    EA_TURN,
    EA_USEITEM,
    EA_COUNT,

}EntAction;

typedef struct Entity Entity; 

extern void(*entActionFuncs[])(Entity*);
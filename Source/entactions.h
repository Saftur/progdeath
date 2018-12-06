/**
 * @file entactions.h
 * @author Connor Meyers (connor.meyers)
 * @course GAM100F18
 * Copyright © 2018 DigiPen (USA) Corporation
 */
#pragma once

typedef enum EntAction
{ //When adding an action, create the LUA constant in entluafuncs.c
    EA_NONE,
    EA_ATTACK,
    EA_BLOCK,
    EA_THROW,

    EA_WPN_ATTACK,

    EA_COUNT,
}EntAction;

typedef struct Entity Entity; 

extern void(*entActionFuncs[])(Entity*);
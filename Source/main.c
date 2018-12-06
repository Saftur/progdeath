/**
 * @file main.c
 * @author Arthur Bouvier (a.bouvier)
 * @course GAM100-F18
 * @brief Game entry point
 * @course GAM100F18
 * Copyright © 2018 DigiPen (USA) Corporation
 * @addtogroup Game
 * @{
 */

#include <windows.h>
#include <C_Processing.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <Engine/engine.h>

#include "gamecomps.h"
#include "gamecolls.h"
#include "screens.h"

// when used with Run() this function will be called once at the beginning of the program
void init()
{
    size(1920, 1080);
    setExitOnEscape(FALSE);
    Engine_init(1);
    Engine_initLayer(0, true, true);
    Engine_initCollFuncList(NUM_TOTAL_COMPS);
    listColls();
    listScreens();
    ScreenMngr_setNextScreen(Engine_getLayer(0)->scrMngr, "Splash Screen");
}

// when used with Run() this function will be called repeatedly every frame
void update()
{
    background(255, 255, 255);
    Engine_update();
}

// main() the starting point for the program
// Run() is used to tell the program which init and update functions to use.
int main(void)
{    
    Run(init, update);
    Engine_delete();
    return 0;
}

/// @}
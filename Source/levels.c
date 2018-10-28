/**
 * @file levels.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/27/18
 * @brief List of game Level s
 * @addtogroup Game-Levels
 * @{
 */
#include "levels.h"

#include <Engine/engine.h>
#include <Engine/levelmngr.h>

#include "splashscreen.h"
#include "menuscreen.h"
#include "gamescreen.h"
#include "deathscreen.h"

/**
 * @brief Create Level s
 */
void listLevels() {
    Level *splashLevel = Level_new("Splash Screen", splashScreenInit, NULL);
    Level *menuLevel = Level_new("Menu", menuScreenInit, NULL);
    Level *gameLevel = Level_new("Game", gameScreenInit, NULL);
    Level *deathLevel = Level_new("Death Screen", deathScreenInit, NULL);

    LevelMngr_addLevel(Engine_getLayer(0)->lvlMngr, splashLevel);
    LevelMngr_addLevel(Engine_getLayer(0)->lvlMngr, menuLevel);
    LevelMngr_addLevel(Engine_getLayer(0)->lvlMngr, gameLevel);
    LevelMngr_addLevel(Engine_getLayer(0)->lvlMngr, deathLevel);
}

/// @}

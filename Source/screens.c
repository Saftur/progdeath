/**
 * @file screens.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/27/18
 * @brief List of game Screen s
 * @addtogroup Game-Screens
 * @{
 */
#include "screens.h"

#include <Engine/engine.h>
#include <Engine/screenmngr.h>

#include "splashscreen.h"
#include "menuscreen.h"
#include "editorscreen.h"
#include "gamescreen.h"
#include "deathscreen.h"

/**
 * @brief Create Screen s
 */
void listScreens() {
    Screen *splashScreen = Screen_new("Splash Screen", splashScreenInit, NULL);
    Screen *menuScreen = Screen_new("Menu", menuScreenInit, NULL);
    Screen *editorScreen = Screen_new("Editor", editorScreenInit, NULL);
    Screen *gameScreen = Screen_new("Game", gameScreenInit, NULL);
    Screen *deathScreen = Screen_new("Death Screen", deathScreenInit, NULL);

    ScreenMngr_addScreen(Engine_getLayer(0)->scrMngr, splashScreen);
    ScreenMngr_addScreen(Engine_getLayer(0)->scrMngr, menuScreen);
    ScreenMngr_addScreen(Engine_getLayer(0)->scrMngr, editorScreen);
    ScreenMngr_addScreen(Engine_getLayer(0)->scrMngr, gameScreen);
    ScreenMngr_addScreen(Engine_getLayer(0)->scrMngr, deathScreen);
}

/// @}

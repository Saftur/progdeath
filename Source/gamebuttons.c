/**
 * @file gamebuttons.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/18/18
 * @brief Game Button effects implementation
 * @addtogroup Game-Components
 * @{
 */
#include "gamebuttons.h"

#include <Engine/gamelayer.h>
#include <Engine/button.h>

/**
 * @brief Go to Game level
 * @param button Effect's Button
 */
void playButtonEffect(Button *button) {
    LevelMngr_setNextLevel(button->comp.owner->objMngr->gLayer->lvlMngr, "Game");
}

/**
 * @brief Go to Menu level
 * @param button Effect's Button
 */
void menuButtonEffect(Button *button) {
    LevelMngr_setNextLevel(button->comp.owner->objMngr->gLayer->lvlMngr, "Menu Screen");
}

/**
 * @brief Exit game
 * @param button Effect's Button
 */
void quitButtonEffect(Button *button) {
    Terminate();
}

/// @}

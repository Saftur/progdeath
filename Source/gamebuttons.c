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
 * @brief Go to Game screen
 * @param button Effect's Button
 */
void playButtonEffect(Button *button) {
    ScreenMngr_setNextScreen(button->comp.owner->objMngr->gLayer->scrMngr, "Game");
}

/**
 * @brief Go to Menu screen
 * @param button Effect's Button
 */
void menuButtonEffect(Button *button) {
    ScreenMngr_setNextScreen(button->comp.owner->objMngr->gLayer->scrMngr, "Menu");
}

/**
 * @brief Exit game
 * @param button Effect's Button
 */
void quitButtonEffect(Button *button) {
    Terminate();
}

/**
 * @brief Go to Editor screen
 * @param button Effect's Button
 */
void editorButtonEffect(Button *button) {
    ScreenMngr_setNextScreen(button->comp.owner->objMngr->gLayer->scrMngr, "Editor");
}

/// @}

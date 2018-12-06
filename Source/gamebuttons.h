/**
 * @file gamebuttons.h
 * @author Arthur Bouvier (a.bouvier)
 * @brief Game Button effects
 * @addtogroup Game-Components
 * @{
 */
#pragma once

typedef struct Button Button; ///< @brief Button forward declaration

/**
 * @brief Go to Game screen
 * @param button Effect's Button
 */
void playButtonEffect(Button *button);

/**
 * @brief Go to Menu screen
 * @param button Effect's Button
 */
void menuButtonEffect(Button *button);

/**
 * @brief Exit game
 * @param button Effect's Button
 */
void quitButtonEffect(Button *button);

/**
 * @brief Go to Editor screen
 * @param button Effect's Button
 */
void editorButtonEffect(Button *button);

/// @}

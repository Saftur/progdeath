/**
 * @file gamebuttons.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/28/18
 * @brief Game Button effects
 * @addtogroup Game-Components
 * @{
 */
#pragma once

typedef struct Button Button; ///< @brief Button forward declaration

/**
 * @brief Go to Game level
 * @param button Effect's Button
 */
void playButtonEffect(Button *button);

/**
 * @brief Go to Menu level
 * @param button Effect's Button
 */
void menuButtonEffect(Button *button);

/**
 * @brief Exit game
 * @param button Effect's Button
 */
void quitButtonEffect(Button *button);

/**
 * @brief Go to Editor level
 * @param button Effect's Button
 */
void editorButtonEffect(Button *button);

/// @}

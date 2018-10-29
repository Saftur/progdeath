/**
 * @file gamescreen.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/27/18
 * @brief Game screen screen
 * @addtogroup Game-Screens
 * @{
 */
#pragma once

typedef struct ObjectMngr ObjectMngr; ///< @brief ObjectMngr forward declaration

/**
 * @brief Initialize Game Screen
 * @param objMngr ObjectMngr to load screen on
 */
void gameScreenInit(ObjectMngr *objMngr);

/// @}

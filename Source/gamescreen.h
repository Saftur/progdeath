/**
 * @file gamescreen.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/26/18
 * @brief Game screen level
 * @addtogroup Game-Levels
 * @{
 */
#pragma once

typedef struct ObjectMngr ObjectMngr; ///< @brief ObjectMngr forward declaration

/**
 * @brief Initialize Game Screen
 * @param objMngr ObjectMngr to load level on
 */
void gameScreenInit(ObjectMngr *objMngr);

/// @}

/**
 * @file gamescreen.h
 * @author Arthur Bouvier (a.bouvier)
 * @brief Game screen screen
 * @course GAM100F18
 * Copyright © 2018 DigiPen (USA) Corporation
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

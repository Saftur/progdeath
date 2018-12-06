/**
 * @file deathscreen.h
 * @author Arthur Bouvier (a.bouvier)
 * @brief Death screen screen
 * @course GAM100F18
 * Copyright © 2018 DigiPen (USA) Corporation
 * @addtogroup Game-Screens
 * @{
 */
#pragma once

typedef struct ObjectMngr ObjectMngr; ///< @brief ObjectMngr forward declaration

extern int has_won;  

/**
 * @brief Initialize Death Screen
 * @param objMngr ObjectMngr to load screen on
 */
void deathScreenInit(ObjectMngr *objMngr);

/// @}

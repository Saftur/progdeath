/**
 * @file menuscreen.h
 * @author Arthur Bouvier (a.bouvier)
 * @brief Menu Screen screen
 * @addtogroup Game-Screens
 * @{
 */
#pragma once

#include <stdbool.h>

typedef struct ObjectMngr ObjectMngr; ///< @brief ObjectMngr forward declaration

/**
 * @brief Initialize Menu Screen
 * @param objMngr ObjectMngr to load screen on
 */
void menuScreenInit(ObjectMngr *objMngr);

/// @}

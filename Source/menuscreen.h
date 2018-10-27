/**
 * @file menuscreen.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/18/18
 * @brief Menu Screen level
 * @addtogroup Game-Levels
 * @{
 */
#pragma once

#include <stdbool.h>

typedef struct ObjectMngr ObjectMngr; ///< @brief ObjectMngr forward declaration

/**
 * @brief Initialize Menu Screen
 * @param objMngr ObjectMngr to load level on
 */
void menuScreenInit(ObjectMngr *objMngr);

/// @}

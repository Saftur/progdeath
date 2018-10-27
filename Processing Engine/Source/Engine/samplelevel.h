/**
 * @file samplelevel.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 
 * @brief Sample Level
 * @addtogroup Game-Levels
 * @{
 */
#pragma once

#include <stdbool.h>

typedef struct ObjectMngr ObjectMngr; ///< @brief ObjectMngr forward declaration

/**
 * @brief Initialize Sample Level
 * @param objMngr ObjectMngr to load level on
 */
void sampleLevelInit(ObjectMngr *objMngr);

/// @}

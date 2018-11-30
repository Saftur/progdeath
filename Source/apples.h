/**
 * @file apples.h
 * @author Connor Meyers(connor.meyers)
 * @date 10/30/18
 * @brief Deals with apple dropping, healing, and tree destruction
 * @addtogroup Components
 * @{
 */
#pragma once

#include <CP_Math.h>

#include "map.h"

/**
  * @brief Spawns an apple on the map
  * @param worldPos world position of tree
  * @param objMngr  object manager
  */
void spawnApple(vec2_t worldPos, ObjectMngr *objMngr);

/// @}

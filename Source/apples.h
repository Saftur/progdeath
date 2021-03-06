/**
 * @file apples.h
 * @author Connor Meyers(connor.meyers)
 * @brief Deals with apple dropping, healing, and tree destruction
 * @course GAM100F18
 * Copyright � 2018 DigiPen (USA) Corporation
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

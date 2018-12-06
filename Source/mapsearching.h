/**
 * @file mapsearching.h
 * @author Connor Meyers(connor.meyers)
 * @brief Functions to search a map for something specific
 * @addtogroup Components
 * @{
 */

#pragma once
#include "environment.h"
#include <C_Processing.h>

/**
* @brief converts a map location to a world location
* @param mapPos position on the map
* @return world position 
*/
vec2_t MapPosToWorldPos(vec2_t mapPos);

/**
 * @brief converts a world location to a map location
 * @param mapPos position on the map
 * @return map position
 */
vec2_t WorldPosToMapPos(vec2_t worldPos);

/**
 * @brief Finds the closest environment of a certain type
 * @param envType the type of environment to search for
 * @param mapPos  position in the world of the 'player' (location to compare distance with)
 * @return world position of closest
 */
vec2_t ClosestEnvironment(EnvironmentType envType, vec2_t worldPos);

/// @}


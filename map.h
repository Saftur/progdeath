/**
 * @file map.h
 * @author Connor Meyuers(connor.meyers)
 * @date 10/30/18
 * @brief Map display and creation
 * @addtogroup Components
 * @{
 */
#pragma once
#include "environment.h"
#include "mapitems.h"
#include "enemyspawns.h"

#define MAP_WIDTH 50
#define MAP_HEIGHT 50

 /**
  * @brief Entity Component
  */
typedef struct Map
{
    EnvironmentType environment[MAP_HEIGHT][MAP_WIDTH]; ///< @brief grid location for environmental placement
    MapItemType items[MAP_HEIGHT][MAP_WIDTH];           ///< @brief grid location for initial item placement
    EnemyType enemies[MAP_HEIGHT][MAP_WIDTH];           ///< @brief grid location representing where enemies will spawn
}Map;

/**
 * @brief Create new map
 * @param seed       Seeds random numbers - set to 0 for no seed.
 */
Map createMap(int seed);

/**
 * @brief Draw a map on the screen
 * @param map        Map to render
 */
void renderMap(Map map);

/// @}
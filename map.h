/**
 * @file map.h
 * @author Connor Meyers(connor.meyers)
 * @date 10/30/18
 * @brief Map display and creation
 * @addtogroup Components
 * @{
 */
#pragma once
#include "environment.h"
#include "mapitems.h"
#include "enemyspawns.h"

#define MAX_MAP_WIDTH 50
#define MAX_MAP_HEIGHT 50


 /**
  * @brief Entity Component
  */
typedef struct Map
{
    EnvironmentType environment[MAX_MAP_HEIGHT][MAX_MAP_WIDTH]; ///< @brief grid location for environmental placement
    MapItemType items[MAX_MAP_HEIGHT][MAX_MAP_WIDTH];           ///< @brief grid location for initial item placement
    EnemyType enemies[MAX_MAP_HEIGHT][MAX_MAP_WIDTH];           ///< @brief grid location representing where enemies will spawn
}Map;

/**
 * @brief Create new map
 * @param map_width  Declares the width of the map
 * @param map_height Declares the height of the map
 * @param seed       Seeds random numbers - set to 0 for no seed.
 * @return Created map
 */
Map* createMap(int map_width, int map_height, int seed);

/**
 * @brief Draw a map on the screen
 * @param map Map to render
 */
void renderMap(Map* map);

/// @}
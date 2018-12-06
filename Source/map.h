/**
 * @file map.h
 * @author Connor Meyers(connor.meyers)
 * @brief Map display and creation
 * @addtogroup Components
 * @{
 */
#pragma once
#include "environment.h"
#include "mapitems.h"
#include "enemytype.h"
#include "spawnpadding.h"
#include "Engine/object.h"
#include <C_Processing.h>

#define MAP_TILE_SIZE 50

#define MAP_MAX_WIDTH 100
#define MAP_MAX_HEIGHT 100
#define MAP_MIN_WIDTH (int)(MAP_MAX_WIDTH / 10)
#define MAP_MIN_HEIGHT (int)(MAP_MAX_HEIGHT / 10)
#define MAP_DEFAULT_HEIGHT (int)(MAP_MAX_HEIGHT / 2)
#define MAP_DEFAULT_WIDTH (int)(MAP_MAX_WIDTH / 2)

#define MAP_MIN_PLAYERS 2  // Includes the main player
#define MAP_DEFAULT_PLAYER_COUNT 16 // ^ 

#define MAP_MIN_PADDING 1
#define MAP_DEFAULT_PADDING MAP_PADDING_FORMULA(MAP_DEFAULT_HEIGHT, MAP_DEFAULT_WIDTH, MAP_DEFAULT_PLAYER_COUNT)

struct EnvObj
{
    unsigned int chance;
    unsigned int spawnPadding;
    unsigned int size;
    unsigned int health;
    PColor color;
};

extern struct EnvObj envObjs[];

 /**
  * @brief Entity Component
  */
typedef struct Map
{
    EnvironmentType environment[MAP_MAX_HEIGHT][MAP_MAX_WIDTH]; ///< @brief grid location for environmental placement
    MapItemType items[MAP_MAX_HEIGHT][MAP_MAX_WIDTH];           ///< @brief grid location for initial item placement
    EnemyType enemies[MAP_MAX_HEIGHT][MAP_MAX_WIDTH];           ///< @brief grid location representing where enemies will spawn
}Map;

/**
 * @brief Create new map
 * @param[out] map  address to store the map at
 * @param seed Seeds random numbers - set to 0 for no seed.
 */
void createMap(Map* map, int seed, ObjectMngr *objMngr);

/**
 * @brief Draw a map on the screen
 * @param map  Map to render
 * @param size size of the map
 */
void renderMap(Map* map, int size);
/// @}
/**
 * @file spawnpadding.c
 * @author Connor Meyers(connor.meyers)
 * @brief Handles the spawn padding for map generation and enemy spawning
 * @addtogroup Components
 * @{
 */
#include "map.h"
#include "spawnpadding.h"
#include <CP_Math.h>
#include "mapsize.h"

static int _padding = MAP_DEFAULT_PADDING;
static int _max_padding = 0;

static void _assertWithinBounds(int*);

 /**
  * @brief Increments the Spawn Padding - wraps around min and max based on minimum players and map size
  * @return Spawn Padding
  */
int incrementSpawnPadding()
{
    _padding++;
    _assertWithinBounds(&_padding);
    return getSpawnPadding();
}

/**
 * @brief Decrements the Spawn Padding - wraps around min and max based on minimum players and map size
 * @return Spawn Padding
 */
int decrementSpawnPadding()
{
    _padding--;
    _assertWithinBounds(&_padding);
    return getSpawnPadding();
}

/**
 * @brief Sets the Spawn Padding - clamps to min and max based on minimum players and map size
 * @return Spawn Padding - used for when clamping occurs
 */
int setSpawnPadding(int amount)
{
    return _padding = constrain_i(amount, MAP_MIN_PADDING, getMaxSpawnPadding());
}

/**
 * @brief gets the current Spawn Padding
 * @return Spawn Padding
 */
int getSpawnPadding()
{
    return _padding;
}

/**
 * @brief Calculates the maximum amount of padding based on minimum players and map size
 * @return Spawn Padding
 */
void calculateMaxPadding()
{
    _max_padding = MAP_PADDING_FORMULA(getMapHeight(), getMapWidth(), MAP_MIN_PLAYERS);
}

/**
 * @brief Gets the maximum amount of padding that can be set
 * @return maximum spawn padding
 */
int getMaxSpawnPadding()
{
    if (!_max_padding) calculateMaxPadding();
    return _max_padding;
}

/**
 * @brief Ensures that the amount of padding never exceeds the bounds - will wrap around min and max padding limit
 */
static void _assertWithinBounds(int * padding)
{
    if (*padding > getMaxSpawnPadding()) *padding = MAP_MIN_PADDING;
    if (*padding < MAP_MIN_PADDING) *padding = getMaxSpawnPadding();
}
/// @}
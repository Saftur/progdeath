/**
 * @file mapsize.c
 * @author Connor Meyers(connor.meyers)
 * @date 11/01/18
 * @brief Handles the size of the map
 * @addtogroup Components
 * @{
 */

#include "map.h"
#include "mapsize.h"
#include <CP_Math.h>
static int _map_height = MAP_DEFAULT_HEIGHT;
static int _map_width = MAP_DEFAULT_WIDTH;

static enum sizeType {HEIGHT, WIDTH};
static void _assertWithinBounds(int * value, enum sizeType type);

 /**
  * @brief Increments the width of the map - wraps around MIN and MAX for map size
  * @return map width
  */
int incrementMapWidth()
{
    _map_width++;
    _assertWithinBounds(&_map_width, WIDTH);
    return getMapWidth();
}
/**
 * @brief Increments the height of the map - wraps around MIN and MAX for map size
 * @return map height
 */
int incrementMapHeight()
{
    _map_height++;
    _assertWithinBounds(&_map_width, HEIGHT);
    return getMapHeight();
}

/**
 * @brief Decrements the width of the map - wraps around MIN and MAX for map size
 * @return map width
 */
int decrementMapWidth()
{
    _map_width--;
    _assertWithinBounds(&_map_width, WIDTH);
    return getMapWidth();
}

/**
 * @brief Decrements the width of the map - wraps around MIN and MAX for map size
 * @return map height
 */
int decrementMapHeight()
{
    _map_height--;
    _assertWithinBounds(&_map_width, HEIGHT);
    return getMapHeight();
}

/**
 * @brief Sets the width of the map - clamps to MIN and MAX for map size
 * @return map width - used for when clamping occurs
 */
int setMapWidth(int width)
{
    return _map_width = constrain_i(width, MAP_MIN_WIDTH, MAP_MAX_WIDTH);
}

/**
 * @brief Sets the height of the map - clamps to MIN and MAX for map size
 * @return map height - used for when clamping occurs
 */
int setMapHeight(int height)
{
    return _map_height = constrain_i(height, MAP_MIN_HEIGHT, MAP_MAX_HEIGHT);
}

/**
 * @brief gets the current width of the map
 * @return map width
 */
int getMapWidth()
{
    return _map_width;
}
/**
 * @brief gets the current height of the map
 * @return map height
 */
int getMapHeight()
{
    return _map_height;
}

/**
 * @brief Ensures that the map size never exceeds the bounds - will wrap around min and max map limit
 */
static void _assertWithinBounds(int * value, enum sizeType type)
{
    if(type == HEIGHT)
    {
        if (*value > MAP_MAX_HEIGHT) *value = MAP_MIN_HEIGHT;
        if (*value < MAP_MIN_HEIGHT) *value = MAP_MAX_HEIGHT;
    }
    if (*value > MAP_MAX_WIDTH) *value = MAP_MIN_WIDTH;
    if (*value < MAP_MIN_WIDTH) *value = MAP_MAX_WIDTH;
}
/// @}
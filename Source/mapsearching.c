/**
 * @file mapsearching.c
 * @author Connor Meyers(connor.meyers)
 * @date 11/02/18
 * @brief Functions to search a map for something specific
 * @addtogroup Components
 * @{
 */
#include "mapsearching.h""
#include "map.h"
#include "mapsize.h"


/**
* @brief converts a map location to a world location
* @param mapPos position on the map
* @return world position 
*/
vec2_t MapPosToWorldPos(vec2_t mapPos)
{
    return (vec2_t) { 0 + MAP_TILE_SIZE / 2 * mapPos.x, 0 + MAP_TILE_SIZE / 2 * mapPos.y };
}

/**
 * @brief converts a world location to a map location
 * @param mapPos position on the map 
 * @return map position 
 */
vec2_t WorldPosToMapPos(vec2_t worldPos)
{
    return (vec2_t) { ceil((double)(worldPos.x) / MAP_TILE_SIZE), ceil((double)(worldPos.y) / MAP_TILE_SIZE)};
}

/**
 * @brief Finds the closest environment of a certain type
 * @param map     address of the map to search
 * @param envType the type of environment to search for
 * @param mapPos  position in the world of the 'player' (location to compare distance with)
 * @return world position of closest
 */
vec2_t ClosestEnvironment(Map* map, EnvironmentType envType, vec2_t worldPos)
{
    vec2_t closest = {0, 0};
    vec2_t const mapPos = WorldPosToMapPos(worldPos);

    for(int y = 0; y < getMapHeight(); y++)
    {
        for(int x = 0; x < getMapWidth(); x++)
        {
            if (map->environment[y][x] != envType) continue;

            if (vec2_distance(closest, mapPos) <= vec2_distance(closest, (vec2_t) { x, y })) continue;

            closest.x = x;
            closest.y = y;
        }
    }

    return MapPosToWorldPos(closest);
}

/// @}


/**
 * @file map.c
 * @author Connor Meyers(connor.meyers)
 * @date 11/02/18
 * @brief Map display and creation
 * @addtogroup Components
 * @{
 */


#include <C_Processing.h>
#include "map.h"
#include <time.h>
#include "mapsize.h"

static struct EnvObj
{
    unsigned int chance;
    unsigned int spawnPadding;
    unsigned int size;
    PColor color;
};

static struct EnvObj envObjs[] = {
    {.size = 1,.color = {128,255,128, 128}},
    {.chance = 4,.spawnPadding = 2, .size = 4, .color = {139,141,122, 255}}, // Mountain
    {.chance = 2,.spawnPadding = 3, .size = 3,.color = {226,88,34, 255}}, // Fire
    {.chance = 3,.spawnPadding = 1,.size = 2,.color = {79,66,181, 255}}, // Water
    {.chance = 8,.spawnPadding = 0, .size = 1,.color = {83, 49, 24, 200}}, // Tree
};

/*static void _placeEnemies(Map* map);
static void _placeItems(Map* map);*/

static void _placeEnvironment(Map* map);
static enum SEARCHTYPE
{
    ENVIRONMENT,
    ENEMY,
    ITEM,
};

static int _search(int subMap[MAP_MAX_HEIGHT][MAP_MAX_WIDTH], vec2_t node, int distance, int exception);
static void _replaceNodes(int subMap[MAP_MAX_HEIGHT][MAP_MAX_WIDTH], vec2_t refLoc, int distance, int replacement, int node);
static int _nodeDist(vec2_t node, vec2_t target);

static void _renderEnvironment(Map* map, int size);
/*static void _renderItems(Map* map);
static void _renderEnemies(Map* map);*/


/**
 * @brief Create new map
 * @param[out] map  address to store the map at
 * @param seed Seeds random numbers - set to 0 for no seed.
 */
void createMap(Map* map, int seed)
{
    if (seed)
        randomSeed(seed);
    else
        randomSeed(time(0));

    /*_placeEnemies(map);*/
    _placeEnvironment(map);
    /*_placeItems(map);*/
}

/**
 * @brief Draw a map on the screen
 * @param map  Map to render
 * @param size size of the map
 */
void renderMap(Map* map, int size)
{
    _renderEnvironment(map, size);
    /*_renderItems(map);
    _renderEnemies(map);*/
}

/**
 * @brief Declares where certain environment objects shall be placed on render
 * @param[out] map address to store the map at
 */
static void _placeEnvironment(Map * map)
{
#define PROTECTED_GRASS -1
    for (int y = 0; y < getMapHeight(); y++)
    {
        for (int x = 0; x < getMapWidth(); x++)
        {
            map->environment[y][x] = GRASS;
        }
    }

    for(int y = 0; y < getMapHeight(); y++)
    {
        for(int x = 0; x < getMapWidth(); x++)
        {
            for(int i = ENVIRONMENT_FIRST; i < ENVIRONMENT_COUNT; i++)
            {
                if (randomRangeInt(0, 100) >= envObjs[i].chance) continue;

                int BREAK = 0;
                for (int offsetX = 0; offsetX < envObjs[i].size; offsetX++)
                    for (int offsetY = 0; offsetY < envObjs[i].size; offsetY++)
                        if (map->environment[y + offsetY][x + offsetX] != GRASS) BREAK = 1;
                
                if (BREAK)break;

                for (int offsetX = 0; offsetX < envObjs[i].size; offsetX++)
                {
                    for (int offsetY = 0; offsetY < envObjs[i].size; offsetY++)
                    {
                        map->environment[y + offsetY][x + offsetX] = i;
                        _replaceNodes(map->environment, (vec2_t) { x + offsetX, y + offsetY }, envObjs[i].spawnPadding, PROTECTED_GRASS, GRASS);
                    }
                }
                
            }
        }
    }

    for (int y = 0; y < getMapHeight(); y++)
    {
        for (int x = 0; x < getMapWidth(); x++)
        {
            if (map->environment[y][x] == PROTECTED_GRASS)
                map->environment[y][x] = GRASS;
        }
    }

#undef PROTECTED_GRASS
}

/**
 * @brief Searches for any integers besides the exception within the distance to the node
 * @param[out]submap address to store the sub-map at
 * @param node       the location of the node to compare with 
 * @param distance   the distance to search within
 * @param exception  what integer type will be excluded from the search
 */
static int _search(int subMap[MAP_MAX_HEIGHT][MAP_MAX_WIDTH], vec2_t node, int distance, int exception)
{
    for(int y = 0; y < getMapHeight(); y++)
    {
        for(int x = 0; x < getMapWidth(); x++)
        {
            
            if (subMap[y][x] == exception)
                continue;
         
            if (_nodeDist(node, (vec2_t) { x, y }) <= distance)
                return 1;
        }
    }

    return 0;
}

/**
 * @brief Sets all nodes of a certain type within the distance to the replacement, except for the self type
 * @param[out]subMap  address to store the sub map at
 * @param refLoc      the location to reference from for distance
 * @param replacement the type of replacement to set
 * @param node        the type of node to be replaced
 * @param distance    the distance to set within
 */
static void _replaceNodes(int subMap[MAP_MAX_HEIGHT][MAP_MAX_WIDTH], vec2_t refLoc, int distance, int replacement, int node)
{
    for (int y = 0; y < getMapHeight(); y++)
    {
        for (int x = 0; x < getMapWidth(); x++)
        {

            if (subMap[y][x] != node)
                continue;

            if (_nodeDist(refLoc, (vec2_t) { x, y }) <= distance)
                subMap[y][x] = replacement;
        }
    }
}

/**
 * @brief Calculates the distance between two nodes
 * @param node   first node
 * @param target second node
 */
static int _nodeDist(vec2_t node, vec2_t target)
{
    return abs((int)node.x - (int)target.x) + abs((int)node.y - (int)target.y);
}

/**
 * @brief Draws the sub-environment map
 * @param map  address of the map to render
 * @param size the size of the nodes
 */
static void _renderEnvironment(const Map* map, const int size)
{
    for(int y = 0; y < getMapHeight(); y++)
    {
        for(int x = 0; x < getMapWidth(); x++)
        {
            const struct EnvObj curObj = envObjs[map->environment[y][x]];

            fill(curObj.color.r, curObj.color.g, curObj.color.b, curObj.color.a);
            rect(x*size, y*size, size, size);
        }
    }
}

/// @}

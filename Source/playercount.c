/**
 * @file playercount.c
 * @author Connor Meyers(connor.meyers)
 * @brief Keeps track of how many players will be spawned
 * @course GAM100F18
 * Copyright © 2018 DigiPen (USA) Corporation
 * @addtogroup Components
 * @{
 */
#include "playercount.h"
#include <C_Processing.h>
#include "map.h"
#include "mapsize.h"
#include "spawnpadding.h"

#define SIGN(a) (a / abs(a))

static int _players = MAP_MIN_PLAYERS;
static int _max_players = 0;

static void _assertWithinBounds(int*);

/**
 * @brief Adds another player - wraps around MAP_MIN_PLAYERS and MAX_PLAYERS
 * @return New player count
 */
int addPlayer()
{
    _players++;
    _assertWithinBounds(&_players);
    return getPlayerCount();
}

/**
 * @brief Removes a player - wraps around MAP_MIN_PLAYERS and MAX_PLAYERS
 * @return New player count
 */
int removePlayer()
{
    _players--;
    _assertWithinBounds(&_players);
    return getPlayerCount();
}

/**
 * @brief Directly sets the amount of players
 * @param amount Amount of players to set - clamps between MAP_MIN_PLAYERS and MAX_PLAYERS
 * @return New player count - used in case of clamp
 */
int setPlayers(int amount)
{
    return _players = constrain_i(amount, MAP_MIN_PLAYERS, getMaxPlayers());
}

/**
 * @brief Retrieves the amount of players
 * @return Player count
 */
int getPlayerCount()
{
    return _players;
}

/**
 * @brief Sets the maximum amount of players that can be spawned - caps based on map size and padding
 * @param players The amount of players to be spawned
 * @return maximum player count
 */
int setMaxPlayers(int players)
{
    return _max_players = constrain_i(players, MAP_MIN_PLAYERS, getMaxPlayers());
}

/**
 * @brief Gets the maximum amount of players that can be spawned
 * @return maximum player count
 */
int getMaxPlayers()
{
    if (!_max_players) calcMaxPlayers();
    return _max_players;
}

/**
 * @brief Ensures that the amount of players never exceeds the bounds - will wrap around min and max player limit
 */
static void _assertWithinBounds(int * players)
{
    if (*players > getMaxPlayers()) *players = MAP_MIN_PLAYERS;
    if (*players < MAP_MIN_PLAYERS) *players = getMaxPlayers();
}

/**
 * @brief Sets the maximum amount of players that can be spawned
 * @param map_height    Height of the map
 * @param map_width     Width of the map
 * @param spawn_padding The minimum distance between spawning players
 */
void calcMaxPlayers()
{
#define CLOSED -1
#define OPEN    0

    int virtual_map[MAP_MAX_HEIGHT][MAP_MAX_WIDTH] = { OPEN };
    int player_count = 0;

    for (int y = 0; y < getMapHeight(); y++)
    {
        for (int x = 0; x < getMapWidth(); x++)
        {
            if (virtual_map[y][x] == OPEN)
            {
                player_count++;

                for (int i = -getSpawnPadding(); i <= getSpawnPadding(); i++)
                {
                    for (int j = -getSpawnPadding(); j <= getSpawnPadding(); j++)
                    {
                        if (j == 0)continue;
                        const int subX = x + j - abs(i) * SIGN(j);
                        const int subY = y + i;
                        if (subY >= 0 && subX >= 0 && subY < getMapHeight() && subX < getMapWidth())
                            virtual_map[subY][subX] = CLOSED;
                    }
                }
            }
        }
    }

    _max_players = player_count;
#undef CLOSED
#undef OPEN
}
/// @}
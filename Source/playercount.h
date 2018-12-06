/**
 * @file playercount.h
 * @author Connor Meyers(connor.meyers)
 * @brief Keeps track of how many players will be spawned
 * @addtogroup Components
 * @{
 */
#pragma once

 /**
  * @brief Adds another player - wraps around MAP_MIN_PLAYERS and MAX_PLAYERS
  * @return New player count
  */
int addPlayer();

/**
 * @brief Removes a player - wraps around MAP_MIN_PLAYERS and MAX_PLAYERS
 * @return New player count
 */
int removePlayer();

/**
 * @brief Directly sets the amount of players
 * @param amount Amount of players to set - clamps between MAP_MIN_PLAYERS and MAX_PLAYERS
 * @return New player count - used in case of clamp
 */
int setPlayers(int amount);

/**
 * @brief Retrieves the amount of players
 * @return Player count
 */
int getPlayerCount();

/**
 * @brief Sets the maximum amount of players that can be spawned - caps based on map size and padding
 * @param players The amount of players to be spawned
 * @return maximum player count
 */
int setMaxPlayers(int players);

/**
 * @brief Gets the maximum amount of players that can be spawned - automatically calculates max players on FIRST call
 * @return maximum player count
 */
int getMaxPlayers();

/**
 * @brief Calculates/Updates the maximum amount of players that can be spawned
 */
void calcMaxPlayers();
/// @}
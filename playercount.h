/**
 * @file playercount.h
 * @author Connor Meyers(connor.meyers)
 * @date 10/31/18
 * @brief Keeps track of how many players will be spawned
 * @addtogroup Components
 * @TODO: dynamically determine max players based on spawn padding
 * @{
 */
#pragma once

#define MIN_PLAYERS 2  // Includes the main player
#define MAX_PLAYERS 16 // ^

 /**
  * @brief Adds another player - wraps around MIN_PLAYERS and MAX_PLAYERS
  * @return New player count
  */
int addPlayer();

/**
 * @brief Removes a player - wraps around MIN_PLAYERS and MAX_PLAYERS
 * @return New player count
 */
int removePlayer();

/**
 * @brief Directly sets the amount of players
 * @param amount Amount of players to set - clamps between MIN_PLAYERS and MAX_PLAYERS
 * @return New player count - used in case of clamp
 */
int setPlayers(int amount);

/**
 * @brief Retrieves the amount of players
 * @return Player count
 */
int getPlayerCount();

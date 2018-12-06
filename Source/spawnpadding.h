/**
 * @file spawnpadding.h
 * @author Connor Meyers(connor.meyers)
 * @brief Handles the spawn padding for map generation and enemy spawning
 * @addtogroup Components
 * @{
 */
#pragma once

#define MAP_PADDING_FORMULA(mapH, mapW, players) ((mapW + mapH - players) / (players - 1))

 /**
  * @brief Increments the Spawn Padding - wraps around min and max based on minimum players and map size
  * @return Spawn Padding
  */
int incrementSpawnPadding();

/**
 * @brief Decrements the Spawn Padding - wraps around min and max based on minimum players and map size
 * @return Spawn Padding
 */
int decrementSpawnPadding();

/**
 * @brief Sets the Spawn Padding - clamps to min and max based on minimum players and map size
 * @return Spawn Padding - used for when clamping occurs
 */
int setSpawnPadding(int amount);

/**
 * @brief gets the current Spawn Padding
 * @return Spawn Padding
 */
int getSpawnPadding();

/**
 * @brief Gets the maximum amount of padding that can be set
 * @return maximum spawn padding
 */
int getMaxSpawnPadding();


/**
 * @brief Calculates the maximum amount of padding based on minimum players and map size
 * @return Spawn Padding
 */
void calculateMaxSpawnPadding();
/// @}
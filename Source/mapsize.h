/**
 * @file mapsize.h
 * @author Connor Meyers(connor.meyers)
 * @brief Handles the size of the map
 * @addtogroup Components
 * @{
 */
#pragma once

 /**
  * @brief Increments the width of the map - wraps around MIN and MAX for map size
  * @return map width
  */
int incrementMapWidth();
/**
 * @brief Increments the height of the map - wraps around MIN and MAX for map size
 * @return map height
 */
int incrementMapHeight();

/**
 * @brief Decrements the width of the map - wraps around MIN and MAX for map size
 * @return map width
 */
int decrementMapWidth();

/**
 * @brief Decrements the width of the map - wraps around MIN and MAX for map size
 * @return map height
 */
int decrementMapHeight();

/**
 * @brief Sets the width of the map - clamps to MIN and MAX for map size
 * @return map width - used for when clamping occurs
 */
int setMapWidth(int width);

/**
 * @brief Sets the height of the map - clamps to MIN and MAX for map size
 * @return map height - used for when clamping occurs
 */
int setMapHeight(int height);

/**
 * @brief gets the current width of the map
 * @return map width 
 */
int getMapWidth(); 
/**
 * @brief gets the current height of the map
 * @return map height
 */
int getMapHeight();
/// @}
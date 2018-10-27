/**
 * @file util.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/18/18
 * @brief Utilities
 * @addtogroup Util
 * @{
 */
#pragma once

#include <stdbool.h>
#include <C_Processing.h>

/**
 * @brief Get distance between two points
 * @param x1 X of point 1
 * @param y1 Y of point 1
 * @param x2 X of point 2
 * @param y2 Y of point 2
 * @return Distance between (x1, y1) and (x2, y2)
 */
float dist(float x1, float y1, float x2, float y2);
/**
 * @brief Get length of vector
 * @param x X of vector
 * @param y Y of vector
 * @return Length of (x, y)
 */
float length(float x, float y);

/**
 * @brief Check if two floats are within a given range of each other
 * @param a     First value
 * @param b     Second value
 * @param range Range to check
 * @return If values are within given range of each other
 */
bool aboutEqual(float a, float b, float range);
/**
 * @brief Check if a float is withing a given range
 * @param num    Value
 * @param range1 First range endpoint
 * @param range2 Second range endpoint
 * @return If value is in given range
 */
bool inRange(float num, float range1, float range2);

/**
 * @brief Check if two PColor s are equal
 * @param c1 First PColor
 * @param c2 Second PColor
 * @return If PColor s are equal
 */
bool colorEquals(PColor c1, PColor c2);

/**
 * @brief Swap two int values
 * @param a Pointer to first value
 * @param b Pointer to second value
 */
void swapInt(int *a, int *b);
/**
 * @brief Swap two unsigned values
 * @param a Pointer to first value
 * @param b Pointer to second value
 */
void swapUnsigned(unsigned *a, unsigned *b);

/// @}

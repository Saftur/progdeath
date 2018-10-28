/**
 * @file util.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/27/18
 * @brief Utilities
 * @addtogroup Util
 * @{
 */
#pragma once

#include <stdbool.h>
#include <C_Processing.h>

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

/**
 * @brief Check if a Vector2D is in a given range (rectangle)
 * @param v The Vector2D
 * @param range1 First range endpoint
 * @param range2 Second range endpoint
 * @return If the Vector2D is in given range
 */
bool vec2_in_range(vec2_t v, vec2_t range1, vec2_t range2);

/// @}

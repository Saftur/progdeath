/**
 * @file util.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/28/18
 * @brief Utilities
 * @addtogroup Util
 * @{
 */
#pragma once

#include <C_Processing.h>

/**
 * @brief Copy data with given size
 * @param data Data to copy
 * @param size Size of data
 * @return Copied data
 */
void *copy(void *data, size_t size);

/**
 * @brief Copy vec2_t from pointer
 * @param vec vec2_t to copy
 * @return Copied vec2_t
 */
vec2_t *vec2_copy(vec2_t *vec);

/**
 * @brief Check if two floats are within a given range of each other
 * @param a     First value
 * @param b     Second value
 * @param range Range to check
 * @return If values are within given range of each other
 */
int aboutEqual(float a, float b, float range);
/**
 * @brief Check if a float is withing a given range
 * @param num    Value
 * @param range1 First range endpoint
 * @param range2 Second range endpoint
 * @return If value is in given range
 */
int inRange(float num, float range1, float range2);

/**
 * @brief Check if two PColor s are equal
 * @param c1 First PColor
 * @param c2 Second PColor
 * @return If PColor s are equal
 */
int colorEquals(PColor c1, PColor c2);

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
 * @brief Check if a vec2_t is in a given range (rectangle)
 * @param v The vec2_t
 * @param range1 First range endpoint
 * @param range2 Second range endpoint
 * @return If the vec2_t is in given range
 */
int vec2_in_range(vec2_t v, vec2_t range1, vec2_t range2);
/**
 * @brief Check if two vec2_t are equal
 * @param a First vec2_t
 * @param b Second vec2_t
 * @return If the two vec2_t are equal
 */
int vec2_equal(vec2_t a, vec2_t b);

/// @}

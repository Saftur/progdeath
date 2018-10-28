/**
 * @file util.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/27/18
 * @brief Utilities implementation
 * @addtogroup Util
 * @{
 */
#include "util.h"

#include <math.h> // sqrtf, fabs

/**
 * @brief Check if two floats are within a given range of each other
 * @param a     First value
 * @param b     Second value
 * @param range Range to check
 * @return If values are within given range of each other
 */
bool aboutEqual(float a, float b, float range) {
    return fabs(a-b) < range;
}

/**
 * @brief Check if a float is withing a given range
 * @param num    Value
 * @param range1 First range endpoint
 * @param range2 Second range endpoint
 * @return If value is in given range
 */
bool inRange(float num, float range1, float range2) {
    return (range1 < range2) ? (num > range1 && num < range2) : (num > range2 && num < range1);
}

/**
 * @brief Check if two PColor s are equal
 * @param c1 First PColor
 * @param c2 Second PColor
 * @return If PColor s are equal
 */
bool colorEquals(PColor c1, PColor c2) {
    return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a;
}

/**
 * @brief Swap two int values
 * @param a Pointer to first value
 * @param b Pointer to second value
 */
void swapInt(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

/**
 * @brief Swap two unsigned values
 * @param a Pointer to first value
 * @param b Pointer to second value
 */
void swapUnsigned(unsigned *a, unsigned *b) {
    unsigned t = *a;
    *a = *b;
    *b = t;
}

/**
 * @brief Check if a vec2_t is in a given range (rectangle)
 * @param v The vec2_t
 * @param range1 First range endpoint
 * @param range2 Second range endpoint
 * @return If the vec2_t is in given range
 */
bool vec2_in_range(vec2_t v, vec2_t range1, vec2_t range2) {
    return inRange(v.x, range1.x, range2.x) && inRange(v.y, range1.y, range2.y);
}

/// @}

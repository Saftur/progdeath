/**
 * @file util.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/18/18
 * @brief Utilities implementation
 * @addtogroup Util
 * @{
 */
#include "util.h"

#include <math.h> // sqrtf, fabs

/**
 * @brief Get distance between two points
 * @param x1 X of point 1
 * @param y1 Y of point 1
 * @param x2 X of point 2
 * @param y2 Y of point 2
 * @return Distance between (x1, y1) and (x2, y2)
 */
float dist(float x1, float y1, float x2, float y2) {
    return length(x1 - x2, y1 - y2);
}

/**
 * @brief Get length of vector
 * @param x X of vector
 * @param y Y of vector
 * @return Length of (x, y)
 */
float length(float x, float y) {
    return sqrtf(x * x + y * y);
}

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

/// @}

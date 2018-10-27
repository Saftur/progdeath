/**
 * @file vector2d.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/17/18
 * @brief Vector2D implementation
 * @addtogroup Util
 * @{
 */
#include "vector2d.h"

#include "util.h" // length, inRange

/**
 * @brief Add two Vector2D s
 * @param a First Vector2D
 * @param b Second Vector2D
 * @return The Vector2D s added
 */
Vector2D Vector2D_add(Vector2D a, Vector2D b) {
    return (Vector2D) { a.x + b.x, a.y + b.y };
}

/**
 * @brief Subtract two Vector2D s
 * @param a First Vector2D
 * @param b Second Vector2D
 * @return The Vector2D s subtracted
 */
Vector2D Vector2D_sub(Vector2D a, Vector2D b) {
    return (Vector2D) { a.x - b.x, a.y - b.y };
}

/**
 * @brief Scale a Vector2D by a scalar
 * @param scalar Scalar to scale by
 * @param v      Vector2D to scale
 * @return The Vector2D scaled by scalar
 */
Vector2D Vector2D_scale(float scalar, Vector2D v) {
    return (Vector2D) { scalar * v.x, scalar * v.y };
}

/**
 * @brief Get the dot product of two Vector2D s
 * @param a First Vector2D
 * @param b Second Vector2D
 * @return The Dot product of the two Vector2D s
 */
float Vector2D_dotProd(Vector2D a, Vector2D b) {
    return a.x*b.x+a.y*b.y;
}

/**
 * @brief Get the projection of a Vector2D onto another
 * @param a Vector2D to project on
 * @param b Vector2D to project
 * @return b projected onto a
 */
Vector2D Vector2D_proj(Vector2D a, Vector2D b) {
    return Vector2D_scale(Vector2D_dotProd(a, b) / Vector2D_dotProd(a, a), a);
}

/**
 * @brief Get the distance between two Vector2D s
 * @param a First Vector2D
 * @param b Second Vector2D
 * @return The distance between the two Vector2D s
 */
float Vector2D_dist(Vector2D a, Vector2D b) {
    return length(a.x - b.x, a.y - b.y);
}

/**
 * @brief Get the length of a Vector2D
 * @param v The Vector2D
 * @return The length of v
 */
float Vector2D_length(Vector2D v) {
    return length(v.x, v.y);
}

/**
 * @brief Check if two Vector2D s are equal
 * @param a First Vector2D
 * @param b Second Vector2D
 * @return If the Vector2D s are equal
 */
bool Vector2D_equals(Vector2D a, Vector2D b) {
    return a.x == b.x && a.y == b.y;
}

/**
 * @brief Check if a Vector2D is in a given range (rectangle)
 * @param v The Vector2D
 * @param range1 First range endpoint
 * @param range2 Second range endpoint
 * @return If the Vector2D is in given range
 */
bool Vector2D_inRange(Vector2D v, Vector2D range1, Vector2D range2) {
    return inRange(v.x, range1.x, range2.x) && inRange(v.y, range1.y, range2.y);
}

/// @}

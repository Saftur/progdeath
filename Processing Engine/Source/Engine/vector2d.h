/**
 * @file vector2d.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/17/18
 * @brief 2D vector
 * @addtogroup Util
 * @{
 */
#pragma once

#include <stdbool.h>

/**
 * @brief 2D vector
 */
typedef struct Vector2D {
    float x, ///< @brief X position of Vector2D
          y; ///< @brief Y position of Vector2D
} Vector2D;

typedef Vector2D Point; ///< @brief Different name for Vector2D

/**
 * @brief Add two Vector2D s
 * @param a First Vector2D
 * @param b Second Vector2D
 * @return The Vector2D s added
 */
Vector2D Vector2D_add(Vector2D a, Vector2D b);
/**
 * @brief Subtract two Vector2D s
 * @param a First Vector2D
 * @param b Second Vector2D
 * @return The Vector2D s subtracted
 */
Vector2D Vector2D_sub(Vector2D a, Vector2D b);
/**
 * @brief Scale a Vector2D by a scalar
 * @param scalar Scalar to scale by
 * @param v      Vector2D to scale
 * @return The Vector2D scaled by scalar
 */
Vector2D Vector2D_scale(float scalar, Vector2D v);

/**
 * @brief Get the dot product of two Vector2D s
 * @param a First Vector2D
 * @param b Second Vector2D
 * @return The Dot product of the two Vector2D s
 */
float Vector2D_dotProd(Vector2D a, Vector2D b);
/**
 * @brief Get the projection of a Vector2D onto another
 * @param a Vector2D to project on
 * @param b Vector2D to project
 * @return b projected onto a
 */
Vector2D Vector2D_proj(Vector2D a, Vector2D b);

/**
 * @brief Get the distance between two Vector2D s
 * @param a First Vector2D
 * @param b Second Vector2D
 * @return The distance between the two Vector2D s
 */
float Vector2D_dist(Vector2D a, Vector2D b);
/**
 * @brief Get the length of a Vector2D
 * @param v The Vector2D
 * @return The length of v
 */
float Vector2D_length(Vector2D v);

/**
 * @brief Check if two Vector2D s are equal
 * @param a First Vector2D
 * @param b Second Vector2D
 * @return If the Vector2D s are equal
 */
bool Vector2D_equals(Vector2D a, Vector2D b);
/**
 * @brief Check if a Vector2D is in a given range (rectangle)
 * @param v The Vector2D
 * @param range1 First range endpoint
 * @param range2 Second range endpoint
 * @return If the Vector2D is in given range
 */
bool Vector2D_inRange(Vector2D v, Vector2D range1, Vector2D range2);

/// @}

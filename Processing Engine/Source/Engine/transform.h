/**
 * @file transform.h
 * @author Arthur Bouvier (a.bouvier)
 * @brief Transform Component
 * @addtogroup Components
 * @{
 */
#pragma once

#include "component.h"

typedef struct Transform Transform; ///< @brief Transform forward declaration

/**
 * @brief Transform Component
 */
typedef struct Transform {
    Component comp; ///< @brief Component data

    vec2_t pos; ///< @brief Position

    void (*posUpdate)(Transform*); ///< @brief Position updating function
} Transform;

/**
 * @brief Create new Transform
 * @return New Transform
 */
Transform *Transform_new();

/**
 * @brief Clone Transform
 * @param this Transform to clone
 * @return Cloned Transform
 */
Transform *_Transform_clone(Transform *this);

/**
 * @brief Delete Transform
 * @param this Transform to delete
 */
void _Transform_delete(Transform *this);

/**
 * @brief Update Transform
 * @param this Transform to update
 */
void _Transform_update(Transform *this);

/// @}

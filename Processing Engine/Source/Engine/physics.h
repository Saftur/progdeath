/**
 * @file physics.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/17/18
 * @brief Physics Component
 * @addtogroup Components
 * @{
 */
#pragma once

#include "component.h"

/**
 * @brief Physics Component
 */
typedef struct Physics {
    Component comp; ///< @brief Component data

    float maxVel;   ///< @brief Maximum velocity
    float deccel;   ///< @brief Decceleration speed

    vec2_t vel;   ///< @brief Current velocity
    vec2_t pVel;  ///< @brief Previous velocity
    vec2_t accel; ///< @brief Current acceleration
} Physics;

/**
 * @brief Create new Physics
 * @param maxVel Maximum velocity
 * @param deccel Decceleration speed
 * @return New Physics
 */
Physics *Physics_new(float maxVel, float deccel);

/**
 * @brief Delete Physics
 * @param this Physics to delete
 */
void _Physics_delete(Physics *this);

/**
 * @brief Update Physics
 * @param this Physics to update
 */
void _Physics_update(Physics *this);

/// @}

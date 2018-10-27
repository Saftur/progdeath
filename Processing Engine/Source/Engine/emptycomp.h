/**
 * @file emptycomp.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/18/18
 * @brief Empty Component
 * @addtogroup Components
 * @{
 */
#pragma once

#include "component.h"

/**
 * @brief EmptyComp Component
 * Used to create a Component with only functions
 */
typedef struct EmptyComp {
    Component comp; ///< @brief Component data
} EmptyComp;

/**
 * @brief Create new EmptyComp
 * @return New EmptyComp
 */
EmptyComp *EmptyComp_new();

/// @}

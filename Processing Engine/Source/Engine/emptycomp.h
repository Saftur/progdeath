/**
 * @file emptycomp.h
 * @author Arthur Bouvier (a.bouvier)
 * @brief Empty Component
 * @course GAM100F18
 * Copyright © 2018 DigiPen (USA) Corporation
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

/**
 * @brief Clone EmptyComp
 * @param this EmptyComp to clone
 * @return Cloned EmptyComp
 */
EmptyComp *_EmptyComp_clone(EmptyComp *this);

/// @}

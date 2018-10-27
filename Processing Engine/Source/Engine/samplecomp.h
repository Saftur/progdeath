/**
 * @file samplethis.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 
 * @brief SampleComp Component
 * @addtogroup Components
 * @{
 */
#pragma once

#include "component.h"

/**
 * @brief SampleComp Component
 */
typedef struct SampleComp {
    Component comp; ///< @brief Component data


} SampleComp;

/**
 * @brief Create new SampleComp
 * @return New SampleComp
 */
SampleComp *SampleComp_new();

/**
 * @brief Delete SampleComp
 * @param this SampleComp to delete
 */
void _SampleComp_delete(SampleComp *this);

/**
 * @brief Update SampleComp
 * @param this SampleComp to update
 */
void _SampleComp_update(SampleComp *this);
/**
 * @brief Draw SampleComp
 * @param this SampleComp to draw
 */
void _SampleComp_draw(SampleComp *this);

/**
 * @brief Resolve collisions with other Component s
 * @param this  This Component
 * @param other Other Component
 */
void _SampleComp_coll_resolve(SampleComp *this, Component *other);

/// @}

/**
 * @file cbgrabcomp.h
 * @author Arthur Bouvier (a.bouvier)
 * @brief CBGrabComp Component
 * @addtogroup Components
 * @{
 */
#pragma once

#include <Engine/component.h>
#include "gamecomps.h"

typedef struct CodeBlock CodeBlock; ///< @brief CodeBlock forward declaration

/**
 * @brief CBGrabComp Component
 */
typedef struct CBGrabComp {
    Component comp; ///< @brief Component data

    CodeBlock *grabbed;
    vec2_t offset;
} CBGrabComp;

/**
 * @brief Create new CBGrabComp
 * @return New CBGrabComp
 */
CBGrabComp *CBGrabComp_new();

/**
 * @brief Clone CBGrabComp
 * @param this CBGrabComp to clone
 * @return Cloned CBGrabComp
 */
CBGrabComp *_CBGrabComp_clone(CBGrabComp *this);

/**
 * @brief Delete CBGrabComp
 * @param this CBGrabComp to delete
 */
void _CBGrabComp_delete(CBGrabComp *this);

/**
 * @brief Update CBGrabComp
 * @param this CBGrabComp to update
 */
void _CBGrabComp_update(CBGrabComp *this);
/**
 * @brief Draw CBGrabComp
 * @param this CBGrabComp to draw
 */
void _CBGrabComp_draw(CBGrabComp *this);

/**
 * @brief Set grabbed CodeBlock
 * @param this  CBGrabComp to set on
 * @param block CodeBlock to set
 * @param offset Offset from mouse
 */
void CBGrabComp_setGrabbed(CBGrabComp *this, CodeBlock *block, vec2_t offset);

/// @}

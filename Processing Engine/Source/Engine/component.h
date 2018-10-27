/**
 * @file component.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/18/18
 * @brief Object Component
 * @addtogroup Components
 * @{
 */
#pragma once

#include <stdlib.h> // malloc
#include <stdbool.h>

#include <C_Processing.h>

#include "object.h"

typedef struct Component Component; ///< @brief Component forward declaration

/**
 * @brief List of Component type ids
 */
enum _Component_TypeId {
    ILLEGAL_COMP,
    WILDSTAR_COMP,
    TRANSFORM,
    PHYSICS,
    TEXT,
    LISTENER,
    BUTTON,

    NUM_ENGINE_COMPS
};

typedef void (*Comp_DeleteFunc)(Component*); ///< @brief Component delete function signature
typedef void (*Comp_UpdateFunc)(Component*); ///< @brief Component update function signature
typedef void (*Comp_DrawFunc)(Component*);   ///< @brief Component draw function signature

typedef void (*Comp_CollResolveFunc)(Component*, Component*); ///< @brief Component collision resolve function signature

/**
 * @brief Object Component
 */
typedef struct Component {
    const char *typeName; ///< @brief Name of Component type
    unsigned typeId;      ///< @brief Id of Component type

    Comp_DeleteFunc delete; ///< @brief Component delete function
    Comp_UpdateFunc update; ///< @brief Component update function
    Comp_DrawFunc draw;     ///< @brief Component draw function

    bool collides;                     ///< @brief Does this Component check collisions
    Comp_CollResolveFunc coll_resolve; ///< @brief Component collision resolve function

    Object *owner; ///< @brief Object this Component is on
} Component;

/**
 * @brief Delete Component
 * @param comp Component to delete
 */
void Component_delete(Component *comp);

/**
 * @brief Update Component
 * @param comp Component to update
 */
void Component_update(Component *comp);
/**
 * @brief Draw Component
 * @param comp Component to draw
 */
void Component_draw(Component *comp);

/**
 * @brief Resolve collision between Component s
 * @param comp1 First Component
 * @param comp2 Second Component
 */
void Component_coll_resolve(Component *comp1, Component *comp2);

/// @}

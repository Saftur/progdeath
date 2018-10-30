/**
 * @file component.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/18/18
 * @brief Component implementation
 * @addtogroup Components
 * @{
 */
#include "component.h"

#include <string.h>

/**
 * @brief Clone Component
 * @param comp Component to clone
 * @return Clone of Component
 */
Component *Component_clone(Component *comp) {
    if (!comp->clone)
        return NULL;
    Component *clone = comp->clone(comp);

    *clone = *comp;
    clone->owner = NULL;

    return clone;
}

/**
 * @brief Delete Component
 * @param comp Component to delete
 */
void Component_delete(Component *comp) {
    if (comp->delete)
        comp->delete(comp);
}

/**
 * @brief Update Component
 * @param comp Component to update
 */
void Component_update(Component *comp) {
    if (comp->update)
        comp->update(comp);
}

/**
 * @brief Draw Component
 * @param comp Component to draw
 */
void Component_draw(Component *comp) {
    if (comp->draw)
        comp->draw(comp);
}

/**
 * @brief Resolve collision
 * @param comp1 First Component
 * @param comp2 Second Component
 */
void Component_coll_resolve(Component *comp1, Component *comp2) {
    if (comp1->coll_resolve)
        comp1->coll_resolve(comp1, comp2);
    if (comp2->coll_resolve)
        comp2->coll_resolve(comp2, comp1);
}

/// @}

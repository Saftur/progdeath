/**
 * @file transform.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/18/18
 * @brief Transform implementation
 * @addtogroup Components
 * @{
 */
#include "transform.h"

/**
 * @brief Create new Transform
 * @return New Transform
 */
Transform *Transform_new() {
    Transform *this = malloc(sizeof(Transform));
    this->comp.typeName = "Transform";
    this->comp.typeId = TRANSFORM;
    this->comp.delete = _Transform_delete;
    this->comp.update = _Transform_update;
    this->comp.draw = NULL;
    this->comp.collides = false;
    this->comp.coll_resolve = NULL;

    this->pos = (vec2_t){0.f, 0.f};
    this->posUpdate = NULL;

    return this;
}

/**
 * @brief Delete Transform
 * @param this Transform to delete
 */
void _Transform_delete(Transform *this) {
    free(this);
}

/**
 * @brief Update Transform
 * @param this Transform to update
 */
void _Transform_update(Transform *this) {
    if (this->posUpdate)
        this->posUpdate(this);
}

/// @}

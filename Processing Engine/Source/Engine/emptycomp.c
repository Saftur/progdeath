/**
 * @file emptycomp.c
 * @author Arthur Bouvier (a.bouvier)
 * @brief EmptyComp implementation
 * @course GAM100F18
 * Copyright © 2018 DigiPen (USA) Corporation
 * @addtogroup Components
 * @{
 */
#include "emptycomp.h"

/**
 * @brief Create new EmptyComp
 * @return New EmptyComp
 */
EmptyComp *EmptyComp_new() {
    EmptyComp *this = malloc(sizeof(EmptyComp));
    this->comp.typeName = "EmptyComp";
    this->comp.typeId = WILDSTAR_COMP;
    this->comp.clone = _EmptyComp_clone;
    this->comp.delete = NULL;
    this->comp.update = NULL;
    this->comp.draw = NULL;
    this->comp.collides = false;
    this->comp.coll_resolve = NULL;
    this->comp.owner = NULL;

    return this;
}

/**
 * @brief Clone EmptyComp
 * @param this EmptyComp to clone
 * @return Cloned EmptyComp
 */
EmptyComp *_EmptyComp_clone(EmptyComp *this) {
    return malloc(sizeof(EmptyComp));
}

/// @}

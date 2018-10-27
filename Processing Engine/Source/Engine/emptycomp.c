/**
 * @file emptycomp.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/18/18
 * @brief EmptyComp implementation
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
    this->comp.delete = NULL;
    this->comp.update = NULL;
    this->comp.draw = NULL;
    this->comp.collides = false;
    this->comp.coll_resolve = NULL;

    return this;
}

/// @}

/**
 * @file samplecomp.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 
 * @brief SampleComp implementation
 * @addtogroup Components
 * @{
 */
#include "samplecomp.h"

/**
 * @brief Create new SampleComp
 * @return New SampleComp
 */
SampleComp *SampleComp_new() {
    SampleComp *this = malloc(sizeof(SampleComp));
    this->comp.typeName = "SampleComp";
    this->comp.typeId = ILLEGAL_COMP;
    this->comp.delete = _SampleComp_delete;
    this->comp.update = _SampleComp_update;
    this->comp.draw = _SampleComp_draw;
    this->comp.collides = false;
    this->comp.coll_resolve = _SampleComp_coll_resolve;



    return this;
}

/**
 * @brief Delete SampleComp
 * @param this SampleComp to delete
 */
void _SampleComp_delete(SampleComp *this) {
	
    free(this);
}

/**
 * @brief Update SampleComp
 * @param this SampleComp to update
 */
void _SampleComp_update(SampleComp *this) {

}

/**
 * @brief Draw SampleComp
 * @param this SampleComp to draw
 */
void _SampleComp_draw(SampleComp *this) {

}

/**
 * @brief Resolve collisions with other Component s
 * @param this  This Component
 * @param other Other Component
 */
void _SampleComp_coll_resolve(SampleComp *this, Component *other) {

}

/// @}

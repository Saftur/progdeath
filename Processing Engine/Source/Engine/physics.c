/**
 * @file physics.c
 * @author Arthur Bouvier (a.bouvier)
 * @brief Physics implementation
 * @course GAM100F18
 * Copyright © 2018 DigiPen (USA) Corporation
 * @addtogroup Components
 * @{
 */
#include "physics.h"

#include "engine.h"
#include "object.h"
#include "transform.h"
#include "util.h"

/**
 * @brief Create new Physics
 * @param maxVel Maximum velocity
 * @param deccel Decceleration speed
 * @return New Physics
 */
Physics *Physics_new(float maxVel, float deccel) {
    Physics *this = malloc(sizeof(Physics));
    this->comp.typeName = "Physics";
    this->comp.typeId = PHYSICS;
    this->comp.clone = _Physics_clone;
    this->comp.delete = _Physics_delete;
    this->comp.update = _Physics_update;
    this->comp.draw = NULL;
    this->comp.collides = false;
    this->comp.coll_resolve = NULL;
    this->comp.owner = NULL;

    this->maxVel = maxVel;
    this->deccel = deccel;

    this->vel = (vec2_t){0.f, 0.f};
    this->pVel = (vec2_t){0.f, 0.f};
    this->accel = (vec2_t){0.f, 0.f};

    return this;
}

/**
 * @brief Clone Physics
 * @param this Physics to clone
 * @return Cloned Physics
 */
Physics *_Physics_clone(Physics *this) {
    Physics *new = malloc(sizeof(Physics));

    new->maxVel = this->maxVel;
    new->deccel = this->deccel;
    new->vel = this->vel;
    new->pVel = this->pVel;
    new->accel = this->accel;

    return new;
}

/**
 * @brief Delete Physics
 * @param this Physics to delete
 */
void _Physics_delete(Physics *this) {
    free(this);
}

/**
 * @brief Update Physics
 * @param this Physics to update
 */
void _Physics_update(Physics *this) {
    this->vel = vec2_add(this->vel, this->accel);
    float len = vec2_length(this->vel);
    if (vec2_equal(this->vel, this->pVel)) {
        // Velocity hasn't changed
        vec2_t dec = (vec2_t) { 0.f, 0.f };
        if (len > 0.f) {
            dec = vec2_scale(this->vel, -this->deccel / len);
        }

        this->vel = vec2_add(this->vel, vec2_scale(dec, dt()));

        if ((dec.x > 0.f && this->vel.x > 0.f) || (dec.x < 0.f && this->vel.x < 0.f))
            this->vel.x = 0.f;
        if ((dec.y > 0.f && this->vel.y > 0.f) || (dec.y < 0.f && this->vel.y < 0.f))
            this->vel.y = 0.f;
    }
    /*if (len > this->maxVel) {
        // Limit max velocity
        this->vel = vec2_scale(this->vel, this->maxVel / len);
    }*/
    this->pVel = this->vel;
    Transform *trs = Object_getComp(this->comp.owner, TRANSFORM);
    if (!trs) return;
    trs->pos = vec2_add(trs->pos, vec2_scale(this->vel, dt()));
}

/// @}

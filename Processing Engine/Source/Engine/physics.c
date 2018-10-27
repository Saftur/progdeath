/**
 * @file physics.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/18/18
 * @brief Physics implementation
 * @addtogroup Components
 * @{
 */
#include "physics.h"

#include "engine.h"
#include "object.h"
#include "transform.h"

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
    this->comp.delete = _Physics_delete;
    this->comp.update = _Physics_update;
    this->comp.draw = NULL;
    this->comp.collides = false;
    this->comp.coll_resolve = NULL;

    this->maxVel = maxVel;
    this->deccel = deccel;

    this->vel = (Vector2D){0.f, 0.f};
    this->pVel = (Vector2D){0.f, 0.f};
    this->accel = (Vector2D){0.f, 0.f};

    return this;
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
    this->vel = Vector2D_add(this->vel, this->accel);
    float len = Vector2D_length(this->vel);
    if (Vector2D_equals(this->vel, this->pVel)) {
        // Velocity hasn't changed
        Vector2D dec = (Vector2D) { 0.f, 0.f };
        if (len > 0.f) {
            dec = Vector2D_scale(-this->deccel / len, this->vel);
        }

        this->vel = Vector2D_add(this->vel, Vector2D_scale(dt(), dec));

        if ((dec.x > 0.f && this->vel.x > 0.f) || (dec.x < 0.f && this->vel.x < 0.f))
            this->vel.x = 0.f;
        if ((dec.y > 0.f && this->vel.y > 0.f) || (dec.y < 0.f && this->vel.y < 0.f))
            this->vel.y = 0.f;
    }
    if (len > this->maxVel) {
        // Limit max velocity
        this->vel = Vector2D_scale(this->maxVel / len, this->vel);
    }
    this->pVel = this->vel;
    Transform *trs = Object_getComp(this->comp.owner, TRANSFORM);
    if (!trs) return;
    trs->pos = Vector2D_add(trs->pos, Vector2D_scale(dt(), this->vel));
}

/// @}

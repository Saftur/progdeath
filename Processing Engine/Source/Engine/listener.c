/**
 * @file listener.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/18/18
 * @brief Listener implementation
 * @addtogroup Components
 * @{
 */
#include "listener.h"

/**
 * @brief Create new Listener
 * @param event  Event function
 * @param effect Effect function
 * @return New Listener
 */
Listener *Listener_new(Listener_EventFunc event, Listener_EffectFunc effect) {
    Listener *this = malloc(sizeof(Listener));
    this->comp.typeName = "Listener";
    this->comp.typeId = LISTENER;
    this->comp.clone = _Listener_clone;
    this->comp.delete = _Listener_delete;
    this->comp.update = _Listener_update;
    this->comp.draw = NULL;
    this->comp.collides = false;
    this->comp.coll_resolve = NULL;
    this->comp.owner = NULL;

    this->event = event;
    this->effect = effect;

    return this;
}

/**
 * @brief Clone Listener
 * @param this Listener to clone
 * @return Cloned Listener
 */
Listener *_Listener_clone(Listener *this) {
    Listener *new = malloc(sizeof(Listener));

    new->event = this->event;
    new->effect = this->effect;

    return new;
}

/**
 * @brief Delete Listener
 * @param this Listener to delete
 */
void _Listener_delete(Listener *this) {
    free(this);
}

/**
 * @brief Update Listener
 * @param this Listener to update
 */
void _Listener_update(Listener *this) {
    if (this->event(this))
        this->effect(this);
}

/// @}

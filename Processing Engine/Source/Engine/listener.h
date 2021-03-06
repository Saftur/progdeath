/**
 * @file listener.h
 * @author Arthur Bouvier (a.bouvier)
 * @brief Simple event listener
 * @course GAM100F18
 * Copyright � 2018 DigiPen (USA) Corporation
 * @addtogroup Components
 * @{
 */
#pragma once

#include <stdbool.h>

#include "component.h"

typedef struct Listener Listener; ///< @brief Listener forward declaration

typedef bool(*Listener_EventFunc)(Listener*);   ///< @brief Listener event function signature
typedef void(*Listener_EffectFunc)(Listener*);  ///< @brief Listener effect function signature

/**
 * @brief Listener Component
 */
typedef struct Listener {
    Component comp; ///< @brief Component data

    Listener_EventFunc event;   ///< @brief Listener event function
    Listener_EffectFunc effect; ///< @brief Listener effect function
} Listener;

/**
 * @brief Create new Listener
 * @param event  Event function
 * @param effect Effect function
 * @return New Listener
 */
Listener *Listener_new(Listener_EventFunc event, Listener_EffectFunc effect);

/**
 * @brief Clone Listener
 * @param this Listener to clone
 * @return Cloned Listener
 */
Listener *_Listener_clone(Listener *this);

/**
 * @brief Delete Listener
 * @param this Listener to delete
 */
void _Listener_delete(Listener *this);

/**
 * @brief Update Listener
 * @param this Listener to update
 */
void _Listener_update(Listener *this);

/// @}

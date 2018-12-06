/**
 * @file sprite.h
 * @author Arthur Bouvier (a.bouvier)
 * @brief Sprite Component
 * @course GAM100F18
 * Copyright © 2018 DigiPen (USA) Corporation
 * @addtogroup Components
 * @{
 */
#pragma once

#include "component.h"

/**
 * @brief Sprite Component
 */
typedef struct Sprite {
    Component comp; ///< @brief Component data

    PImage image; ///< @brief Image
    vec2_t size;  ///< @brief Size of Sprite
    float alpha;  ///< @brief Sprite alpha value
} Sprite;

/**
 * @brief Create new Sprite
 * @param fileName File name of Sprite image
 * @param size     Size of Sprite
 * @return New Sprite
 */
Sprite *Sprite_new(const char *fileName, vec2_t size);

/**
 * @brief Clone Sprite
 * @param this Sprite to clone
 * @return Cloned Sprite
 */
Sprite *_Sprite_clone(Sprite *this);

/**
 * @brief Delete Sprite
 * @param this Sprite to delete
 */
void _Sprite_delete(Sprite *this);

/**
 * @brief Draw Sprite
 * @param this Sprite to draw
 */
void _Sprite_draw(Sprite *this);

/// @}

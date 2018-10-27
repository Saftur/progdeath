/**
 * @file sprite.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/22/18
 * @brief Sprite Component
 * @addtogroup Components
 * @{
 */
#pragma once

#include "component.h"

#include <C_Processing.h>

#include "vector2d.h"

/**
 * @brief Sprite Component
 */
typedef struct Sprite {
    Component comp; ///< @brief Component data

    PImage image;  ///< @brief Image
    Vector2D size; ///< @brief Size of Sprite
} Sprite;

/**
 * @brief Create new Sprite
 * @param fileName File name of Sprite image
 * @param size     Size of Sprite
 * @return New Sprite
 */
Sprite *Sprite_new(const char *fileName, Vector2D size);

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

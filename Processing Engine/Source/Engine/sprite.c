/**
 * @file sprite.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/27/18
 * @brief Sprite implementation
 * @addtogroup Components
 * @{
 */
#include "sprite.h"

#include "transform.h"

/**
 * @brief Create new Sprite
 * @param fileName File name of Sprite image
 * @param size     Size of Sprite
 * @return New Sprite
 */
Sprite *Sprite_new(const char *fileName, vec2_t size) {
    Sprite *this = malloc(sizeof(Sprite));
    this->comp.typeName = "Sprite";
    this->comp.typeId = ILLEGAL_COMP;
    this->comp.delete = _Sprite_delete;
    this->comp.update = NULL;
    this->comp.draw = _Sprite_draw;
    this->comp.collides = false;
    this->comp.coll_resolve = NULL;

    this->image = loadImage(fileName);
    this->size = size;
    this->alpha = 1.f;

    return this;
}

/**
 * @brief Delete Sprite
 * @param this Sprite to delete
 */
void _Sprite_delete(Sprite *this) {
    free(this);
}

/**
 * @brief Draw Sprite
 * @param this Sprite to draw
 */
void _Sprite_draw(Sprite *this) {
    if (!this->image) return;

    Transform *trs = Object_getComp(this->comp.owner, TRANSFORM);
    if (!trs) return;

    imageAlpha(this->image, trs->pos.x, trs->pos.y, this->size.x, this->size.y, this->alpha);
}

/// @}

/**
 * @file cbgrabcomp.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/31/18
 * @brief CBGrabComp implementation
 * @addtogroup Components
 * @{
 */
#include "cbgrabcomp.h"

#include "editorscreen.h"
#include "codeblock.h"

/**
 * @brief Create new CBGrabComp
 * @return New CBGrabComp
 */
CBGrabComp *CBGrabComp_new() {
    CBGrabComp *this = malloc(sizeof(CBGrabComp));
    this->comp.typeName = "CBGrabComp";
    this->comp.typeId = CBGRABCOMP;
    this->comp.clone = _CBGrabComp_clone;
    this->comp.delete = _CBGrabComp_delete;
    this->comp.update = _CBGrabComp_update;
    this->comp.draw = _CBGrabComp_draw;
    this->comp.collides = false;
    this->comp.coll_resolve = NULL;

    this->grabbed = NULL;

    return this;
}

/**
 * @brief Clone CBGrabComp
 * @param this CBGrabComp to clone
 * @return Cloned CBGrabComp
 */
CBGrabComp *_CBGrabComp_clone(CBGrabComp *this) {
    CBGrabComp *new = malloc(sizeof(CBGrabComp));

    new->grabbed = _CodeBlock_clone(this->grabbed);

    return new;
}

/**
 * @brief Delete CBGrabComp
 * @param this CBGrabComp to delete
 */
void _CBGrabComp_delete(CBGrabComp *this) {
    if (this->grabbed)
        _CodeBlock_delete(this->grabbed);
    free(this);
}

/**
 * @brief Update CBGrabComp
 * @param this CBGrabComp to update
 */
void _CBGrabComp_update(CBGrabComp *this) {
    if (mouseReleased(MOUSE_BUTTON_1) && this->grabbed) {
        vec2_t mPos = (vec2_t){ mouseX, mouseY };
        screenToWorld(&mPos.x, &mPos.y);
        addToBoard(this->grabbed, mPos);
        this->grabbed = NULL;
    }
}

/**
 * @brief Draw CBGrabComp
 * @param this CBGrabComp to draw
 */
void _CBGrabComp_draw(CBGrabComp *this) {
    if (this->grabbed) {
        vec2_t mPos = (vec2_t){ mouseX, mouseY };
        screenToWorld(&mPos.x, &mPos.y);
        CodeBlock_draw(this->grabbed, mPos);
    }

}

/**
 * @brief Set grabbed CodeBlock
 * @param this  CBGrabComp to set on
 * @param block CodeBlock to set
 */
void CBGrabComp_setGrabbed(CBGrabComp *this, CodeBlock *block) {
    if (this->grabbed)
        _CodeBlock_delete(this->grabbed);
    this->grabbed = block;
}

/// @}


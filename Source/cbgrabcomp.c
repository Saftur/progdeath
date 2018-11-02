/**
 * @file cbgrabcomp.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/31/18
 * @brief CBGrabComp implementation
 * @addtogroup Components
 * @{
 */
#include "cbgrabcomp.h"

#include <Engine/engine.h>

#include "editorscreen.h"
#include "codeblock.h"
#include "codeblockboard.h"

static void mouseCallback(MouseButton button, KeyState state, CBGrabComp *this);

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
    this->offset = vec2_zero();

    Engine_addMouseCallback(mouseCallback, this);

    return this;
}

/**
 * @brief Clone CBGrabComp
 * @param this CBGrabComp to clone
 * @return Cloned CBGrabComp
 */
CBGrabComp *_CBGrabComp_clone(CBGrabComp *this) {
    CBGrabComp *new = malloc(sizeof(CBGrabComp));

    new->grabbed = CodeBlock_clone(this->grabbed);

    return new;
}

/**
 * @brief Delete CBGrabComp
 * @param this CBGrabComp to delete
 */
void _CBGrabComp_delete(CBGrabComp *this) {
    Engine_removeMouseCallback(mouseCallback, this);
    if (this->grabbed)
        CodeBlock_delete(this->grabbed);
    free(this);
}

/**
 * @brief Update CBGrabComp
 * @param this CBGrabComp to update
 */
void _CBGrabComp_update(CBGrabComp *this) {

}

static void mouseCallback(MouseButton button, KeyState state, CBGrabComp *this) {
    if (!this->grabbed || button != MOUSE_BUTTON_1 || state != KeyReleased)
        return;
    vec2_t mPos = (vec2_t){ mouseX, mouseY };
    //screenToWorld(&mPos.x, &mPos.y);
    mPos = vec2_sub(mPos, this->offset);
    addToBoard(this->grabbed, mPos);
    this->grabbed = NULL;
}

/**
 * @brief Draw CBGrabComp
 * @param this CBGrabComp to draw
 */
void _CBGrabComp_draw(CBGrabComp *this) {
    translate(cb_offset.x, cb_offset.y);
    scale(cb_scale);
    if (this->grabbed) {
        /*vec2_t mPos = (vec2_t){ mouseX, mouseY };
        mPos = vec2_sub(mPos, cb_offset);
        mPos = vec2_scale(mPos, 1 / cb_scale);*/
        vec2_t mPos = CB_getMousePos();
        mPos = vec2_sub(mPos, this->offset);
        //screenToWorld(&mPos.x, &mPos.y);
        CodeBlock_draw(this->grabbed, mPos);
    }
    scale(1 / cb_scale);
    translate(-cb_offset.x, -cb_offset.y);
}

/**
 * @brief Set grabbed CodeBlock
 * @param this  CBGrabComp to set on
 * @param block CodeBlock to set
 */
void CBGrabComp_setGrabbed(CBGrabComp *this, CodeBlock *block, vec2_t offset) {
    if (this->grabbed)
        CodeBlock_delete(this->grabbed);
    this->grabbed = block;
    this->offset = offset;
}

/// @}


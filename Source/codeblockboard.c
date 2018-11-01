/**
 * @file codeblockboard.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/31/18
 * @brief CodeBlockBoard implementation
 * @addtogroup Components
 * @{
 */
#include "codeblockboard.h"

#include <Engine/util.h>

#include "editorscreen.h"
#include "codeblock.h"

float cb_scale;
vec2_t cb_offset;

/**
 * @brief Create new CodeBlockBoard
 * @return New CodeBlockBoard
 */
CodeBlockBoard *CodeBlockBoard_new() {
    CodeBlockBoard *this = malloc(sizeof(CodeBlockBoard));
    this->comp.typeName = "CodeBlockBoard";
    this->comp.typeId = CODEBLOCKBOARD;
    this->comp.clone = _CodeBlockBoard_clone;
    this->comp.delete = _CodeBlockBoard_delete;
    this->comp.update = _CodeBlockBoard_update;
    this->comp.draw = _CodeBlockBoard_draw;
    this->comp.collides = false;
    this->comp.coll_resolve = NULL;

    this->blocks = List_new(10, CodeBlock_clone, CodeBlock_delete);
    this->blockPos = List_new(10, vec2_copy, free);

    return this;
}

/**
 * @brief Clone CodeBlockBoard
 * @param this CodeBlockBoard to clone
 * @return Cloned CodeBlockBoard
 */
CodeBlockBoard *_CodeBlockBoard_clone(CodeBlockBoard *this) {
    CodeBlockBoard *new = malloc(sizeof(CodeBlockBoard));

    return new;
}


/**
 * @brief Delete CodeBlockBoard
 * @param this CodeBlockBoard to delete
 */
void _CodeBlockBoard_delete(CodeBlockBoard *this) {
    List_delete(this->blocks);
    List_delete(this->blockPos);
    free(this);
}

/**
 * @brief Update CodeBlockBoard
 * @param this CodeBlockBoard to update
 */
void _CodeBlockBoard_update(CodeBlockBoard *this) {
    if (mousePressed(MOUSE_BUTTON_1)) {
        vec2_t mPos = (vec2_t){ mouseX, mouseY };
        mPos = vec2_sub(mPos, cb_offset);
        mPos = vec2_scale(mPos, 1 / cb_scale);
        //screenToWorld(&mPos.x, &mPos.y);
        for (unsigned i = this->blocks->size - 1; i < this->blocks->size; i--) {
            CodeBlock **block = this->blocks->items+i;
            vec2_t pos = *(vec2_t*)this->blockPos->items[i];
            CBGrabResult grabRes = CodeBlock_grab(*block, vec2_sub(mPos, pos));
            if (grabRes.either) {
                if (grabRes.parent) {
                    List_remove_nodelete(this->blocks, i);
                    List_remove_nodelete(this->blockPos, i);
                }
                return;
            }
        }
    }
}

/**
 * @brief Draw CodeBlockBoard
 * @param this CodeBlockBoard to draw
 */
void _CodeBlockBoard_draw(CodeBlockBoard *this) {
    translate(cb_offset.x, cb_offset.y);
    scale(cb_scale);
    for (unsigned i = 0; i < this->blocks->size; i++)
        CodeBlock_draw(this->blocks->items[i], *(vec2_t*)this->blockPos->items[i]);
    scale(1 / cb_scale);
    translate(-cb_offset.x, -cb_offset.y);
}

/**
 * @brief Add CodeBlock to CodeBlockBoard
 * @param this  CodeBlockBoard to add to
 * @param block CodeBlock to add
 * @param pos   Position to add at
 */
void CodeBlockBoard_addBlock(CodeBlockBoard *this, CodeBlock *block, vec2_t pos) {
    pos = vec2_sub(pos, cb_offset);
    pos = vec2_scale(pos, 1 / cb_scale);
    vec2_t offPos = pos;
    for (unsigned i = 0; i < this->blocks->size; i++) {
        CodeBlock *bBlock = this->blocks->items[i];
        vec2_t bPos = *(vec2_t*)this->blockPos->items[i];
        vec2_t subPos = vec2_sub(pos, bPos);
        if (CodeBlock_drop(bBlock, block, subPos))
            return;
        vec2_t bSize = CodeBlock_getsize(bBlock);
        vec2_t bEndPos = vec2_add(bPos, bSize);
        if (vec2_in_range(offPos, bPos, bEndPos))
            offPos.x = bEndPos.x;
    }
    if (isOnList(pos)) {
        CodeBlock_delete(block);
        return;
    }
    List_push_back(this->blocks, block);
    vec2_t *newPos = malloc(sizeof(vec2_t));
    *newPos = offPos;
    List_push_back(this->blockPos, newPos);
}

/// @}


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

#include "codeblock.h"

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

    this->blocks = List_new(10, _CodeBlock_clone, _CodeBlock_delete);
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
        screenToWorld(&mPos.x, &mPos.y);
        for (int i = 0; i < this->blocks->size; i++) {
            CodeBlock **block = this->blocks->items+i;
            vec2_t pos = *(vec2_t*)this->blockPos->items[i];
            if (CodeBlock_grab(*block, vec2_sub(mPos, pos))) {
                List_remove_nodelete(this->blocks, i);
                List_remove_nodelete(this->blockPos, i);
                break;
            }
        }
    }
}

/**
 * @brief Draw CodeBlockBoard
 * @param this CodeBlockBoard to draw
 */
void _CodeBlockBoard_draw(CodeBlockBoard *this) {
    for (unsigned i = 0; i < this->blocks->size; i++)
        CodeBlock_draw(this->blocks->items[i], *(vec2_t*)this->blockPos->items[i]);
}

/**
 * @brief Add CodeBlock to CodeBlockBoard
 * @param this  CodeBlockBoard to add to
 * @param block CodeBlock to add
 * @param pos   Position to add at
 */
void CodeBlockBoard_addBlock(CodeBlockBoard *this, CodeBlock *block, vec2_t pos) {
    List_push_back(this->blocks, block);
    vec2_t *newPos = malloc(sizeof(vec2_t));
    *newPos = pos;
    List_push_back(this->blockPos, newPos);
}

/// @}


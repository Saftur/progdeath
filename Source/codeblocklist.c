/**
 * @file codeblocklist.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/31/18
 * @brief CodeBlockList implementation
 * @addtogroup Components
 * @{
 */
#include "codeblocklist.h"

#include <Engine/engine.h>
#include <Engine/util.h>

#include "codeblock.h"
#include "editorscreen.h"

#define BORDER 20

/**
 * @brief Create new CodeBlockList
 * @return New CodeBlockList
 */
CodeBlockList *CodeBlockList_new() {
    CodeBlockList *this = malloc(sizeof(CodeBlockList));
    this->comp.typeName = "CodeBlockList";
    this->comp.typeId = CODEBLOCKLIST;
    this->comp.clone = _CodeBlockList_clone;
    this->comp.delete = _CodeBlockList_delete;
    this->comp.update = _CodeBlockList_update;
    this->comp.draw = _CodeBlockList_draw;
    this->comp.collides = false;
    this->comp.coll_resolve = NULL;
    this->comp.owner = NULL;

    this->size = (vec2_t){ 0, 0 };
    this->blocks = List_new(CB_NUM_TYPES, CodeBlock_clone, CodeBlock_delete);
    this->blockPos = List_new(CB_NUM_TYPES, vec2_copy, free);
    this->blockSize = List_new(CB_NUM_TYPES, vec2_copy, free);
    for (int type = 0; type < CB_NUM_TYPES; type++) {
        switch (type) {
        case CB_EMPTY: case CB_VAR:
            break;
        default:
            CodeBlockList_addBlock(this, CodeBlock_new(type, NULL, 0));
        }
    }

    return this;
}

/**
 * @brief Clone CodeBlockList
 * @param this CodeBlockList to clone
 * @return Cloned CodeBlockList
 */
CodeBlockList *_CodeBlockList_clone(CodeBlockList *this) {
    CodeBlockList *new = malloc(sizeof(CodeBlockList));

    new->blocks = List_copy(this->blocks);

    return new;
}

/**
 * @brief Delete CodeBlockList
 * @param this CodeBlockList to delete
 */
void _CodeBlockList_delete(CodeBlockList *this) {
    List_delete(this->blocks);
    List_delete(this->blockPos);
    List_delete(this->blockSize);
    free(this);
}

/**
 * @brief Update CodeBlockList
 * @param this CodeBlockList to update
 */
void _CodeBlockList_update(CodeBlockList *this) {
    if (mousePressed(MOUSE_BUTTON_1)) {
        vec2_t mPos = (vec2_t){ mouseX, mouseY };
        screenToWorld(&mPos.x, &mPos.y);
        if (0 <= mPos.x && mPos.x < this->size.x && 0 <= mPos.y && mPos.y < this->size.y) {
            for (int i = 0; i < this->blocks->size; i++) {
                CodeBlock **block = this->blocks->items+i;
                vec2_t pos = *(vec2_t*)this->blockPos->items[i];
                if (CodeBlock_grab(*block, vec2_sub(mPos, pos)).parent) {
                    *block = CodeBlock_clone(*block);
                }
            }
        }
    }
}

/**
 * @brief Draw CodeBlockList
 * @param this CodeBlockList to draw
 */
void _CodeBlockList_draw(CodeBlockList *this) {
    for (int i = 0; i < this->blocks->size; i++) {
        vec2_t pos = *(vec2_t*)this->blockPos->items[i];
        CodeBlock_draw(this->blocks->items[i], pos);
    }
}

/**
 * @brief Add CodeBlock to CodeBlockList
 * @param this  CodeBlockList to add to
 * @param block CodeBlock to add
 */
void CodeBlockList_addBlock(CodeBlockList *this, CodeBlock *block) {
    vec2_t lastPos = (this->blockPos->size > 0) ? *(vec2_t*)this->blockPos->items[this->blockPos->size-1] : (vec2_t){ BORDER, 0 };
    vec2_t lastSize = (this->blockSize->size > 0) ? *(vec2_t*)this->blockSize->items[this->blockPos->size-1] : (vec2_t){ 0, 0 };
    vec2_t *newPos = malloc(sizeof(vec2_t));
    *newPos = lastPos;
    newPos->y += lastSize.y + BORDER;
    vec2_t *newSize = malloc(sizeof(vec2_t));
    *newSize = CodeBlock_getsize(block);
    List_push_back(this->blocks, block);
    List_push_back(this->blockPos, newPos);
    List_push_back(this->blockSize, newSize);
    if (newSize->x + BORDER > this->size.x)
        this->size.x = newSize->x + BORDER;
    this->size.y += newSize->y + BORDER;
}

/// @}


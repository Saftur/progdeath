/**
 * @file codeblocklist.c
 * @author Arthur Bouvier (a.bouvier)
 * @brief CodeBlockList implementation
 * @course GAM100F18
 * Copyright � 2018 DigiPen (USA) Corporation
 * @addtogroup Components
 * @{
 */
#include "codeblocklist.h"

#include <Engine/engine.h>
#include <Engine/util.h>
#include <Engine/focus.h>

#include "codeblock.h"
#include "editorscreen.h"
#include "cb_binaryop.h"
#include "cb_unaryop.h"

#define BORDER 20
#define SCROLLSPD 20

static void mouseCallback(MouseButton button, KeyState state, CodeBlockList *this);
static void wheelCallback(float xoffset, float yoffset, CodeBlockList *this);

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

    this->yscroll = 0;

    this->size = (vec2_t){ 0, 0 };
    this->blocks = List_new(CB_NUM_TYPES, CodeBlock_clone, CodeBlock_delete);
    this->blockPos = List_new(CB_NUM_TYPES, vec2_copy, free);
    this->blockSize = List_new(CB_NUM_TYPES, vec2_copy, free);
    for (int type = 0; type < CB_NUM_TYPES; type++) {
        CodeBlock *block = NULL;
        switch (type) {
        case CB_EMPTY: case CB_NUM:
            break;
        /*case CB_VAR:
            //block = CodeBlock_new(type, "test", 5);
            break;*/
        /*case CB_FUNCTION:
            block = CodeBlock_new(type, "test", 5);
            break;*/
        case CB_STR:
            block = CodeBlock_new(type, NULL, 0);
            removeFocusObject(*(void**)block->data);
            break;
        case CB_BINARYOP:
            for (CB_binaryop_type t = 0; t < CB_BOP_NUMOPS; t++)
                CodeBlockList_addBlock(this, CodeBlock_new(type, &t, sizeof(unsigned)));
            break;
        case CB_UNARYOP:
            for (CB_unaryop_type t = 0; t < CB_UOP_NUMOPS; t++)
                CodeBlockList_addBlock(this, CodeBlock_new(type, &t, sizeof(unsigned)));
            break;
        default:
            block = CodeBlock_new(type, NULL, 0);
        }
        if (block)
            CodeBlockList_addBlock(this, block);
    }

    Engine_addMouseCallback(mouseCallback, this);
    Engine_addWheelCallback(wheelCallback, this);

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
    Engine_removeMouseCallback(mouseCallback, this);
    Engine_removeWheelCallback(wheelCallback, this);
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

}

static void mouseCallback(MouseButton button, KeyState state, CodeBlockList *this) {
    if (button != MOUSE_BUTTON_1 || state != KeyPressed)
        return;
    vec2_t mPos = (vec2_t){ mouseX, mouseY };
    //screenToWorld(&mPos.x, &mPos.y);
    if (0 <= mPos.x && mPos.x < this->size.x && 0 <= mPos.y && mPos.y < this->size.y) {
        for (int i = 0; i < this->blocks->size; i++) {
            CodeBlock **block = this->blocks->items+i;
            vec2_t pos = *(vec2_t*)this->blockPos->items[i];
            vec2_t sPos = vec2_sub(mPos, pos);
            sPos.y += this->yscroll;
            if (CodeBlock_grab_test(*block, sPos, 1).parent) {
                //*block = CodeBlock_clone(*block);
                setGrabbed(CodeBlock_clone(*block), sPos);
            }
        }
    }
}

static void wheelCallback(float xoffset, float yoffset, CodeBlockList *this) {
    if (mouseX >= this->size.x)
        return;
    this->yscroll -= yoffset * SCROLLSPD;
    if (this->yscroll < 0)
        this->yscroll = 0;
    if (this->yscroll > this->size.y - canvasHeight + BORDER)
        this->yscroll = this->size.y - canvasHeight + BORDER;
}

/**
 * @brief Draw CodeBlockList
 * @param this CodeBlockList to draw
 */
void _CodeBlockList_draw(CodeBlockList *this) {
    for (int i = 0; i < this->blocks->size; i++) {
        vec2_t pos = *(vec2_t*)this->blockPos->items[i];
        pos.y -= this->yscroll;
        CodeBlock_draw(this->blocks->items[i], pos);
    }
    noStroke();
    fill(0, 0, 0, 255);
    rect(this->size.x - 5, 0, 10, canvasHeight);
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
        this->size.x = newSize->x + BORDER * 2;
    this->size.y += newSize->y + BORDER;
}

/// @}


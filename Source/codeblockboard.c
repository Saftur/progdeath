/**
 * @file codeblockboard.c
 * @author Arthur Bouvier (a.bouvier)
 * @brief CodeBlockBoard implementation
 * @addtogroup Components
 * @{
 */
#include "codeblockboard.h"

#include <stdio.h>
#include <string.h>
#include <Engine/engine.h>
#include <Engine/util.h>

#include "editorscreen.h"
#include "codeblock.h"

float cb_scale;
vec2_t cb_offset;

vec2_t CB_getMousePos() {
    vec2_t mPos = (vec2_t){ mouseX, mouseY };
    mPos = vec2_sub(mPos, cb_offset);
    mPos = vec2_scale(mPos, 1 / cb_scale);
    return mPos;
}

static void mouseCallback(MouseButton button, KeyState state, CodeBlockBoard *this);

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

    Engine_addMouseCallback(mouseCallback, this);

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
    Engine_removeMouseCallback(mouseCallback, this);
    List_delete(this->blocks);
    List_delete(this->blockPos);
    free(this);
}

/**
 * @brief Update CodeBlockBoard
 * @param this CodeBlockBoard to update
 */
void _CodeBlockBoard_update(CodeBlockBoard *this) {
    for (unsigned i = 0; i < this->blocks->size; i++)
        CodeBlock_update(this->blocks->items[i], *(vec2_t*)this->blockPos->items[i]);
}

static void mouseCallback(MouseButton button, KeyState state, CodeBlockBoard *this) {
    if (button != MOUSE_BUTTON_1 || state != KeyPressed)
        return;
    /*vec2_t mPos = (vec2_t){ mouseX, mouseY };
    mPos = vec2_sub(mPos, cb_offset);
    mPos = vec2_scale(mPos, 1 / cb_scale);*/
    vec2_t mPos = CB_getMousePos();
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
            break;
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
    vec2_t bSize = CodeBlock_getsize(block);
    vec2_t dropOffset = (vec2_t){ min(10, bSize.x * 0.5f), min(10, bSize.y * 0.5f) };//vec2_scale(CodeBlock_getsize(block), 0.1);
    vec2_t offPos = pos;
    pos = vec2_add(pos, dropOffset);
    for (unsigned i = 0; i < this->blocks->size; i++) {
        CodeBlock *bBlock = this->blocks->items[i];
        vec2_t bPos = *(vec2_t*)this->blockPos->items[i];
        vec2_t subPos = vec2_sub(pos, bPos);
        if (CodeBlock_drop(bBlock, block, subPos))
            return;
        vec2_t bSize = CodeBlock_getsize(bBlock);
        vec2_t bEndPos = vec2_add(bPos, bSize);
        if (vec2_in_range(vec2_add(offPos, dropOffset), bPos, bEndPos))
            offPos.x = bEndPos.x;
    }
    if (isOnList(pos)) {
        CodeBlock_delete(block);
        return;
    }
    CodeBlock *sequence = CodeBlock_new_nodata(CB_SEQUENCE);
    List_push_back(sequence->blocks, block);
    List_push_back(this->blocks, sequence);
    vec2_t *newPos = malloc(sizeof(vec2_t));
    *newPos = offPos;
    List_push_back(this->blockPos, newPos);
}

/**
 * @brief Convert all CodeBlock s in board to text
 * @param this CodeBlockBoard to convert from
 * @return Newly allocated string
 */
char *CodeBlockBoard_totext(CodeBlockBoard *this) {
    List *texts = List_new(100, NULL, free);
    size_t len = 0;
    for (unsigned i = 0; i < this->blocks->size; i++) {
        CodeBlock *block = this->blocks->items[i];
        vec2_t pos = *(vec2_t*)this->blockPos->items[i];
        char buf[32];
        sprintf(buf, "-- %6.4f, %6.4f\n", pos.x, pos.y);
        size_t commLen = strlen(buf);
        char *comment = malloc((commLen+1) * sizeof(char));
        strcpy(comment, buf);
        List_push_back(texts, comment);
        len += commLen;
        char *bText = CodeBlock_totext(block);
        List_push_back(texts, bText);
        len += strlen(bText);
        char *nl = malloc(2 * sizeof(char));
        nl[0] = '\n'; nl[1] = 0;
        List_push_back(texts, nl);
        len += 2;
    }
    
    char *text = malloc((len+1) * sizeof(char));
    char *t = text;
    for (unsigned i = 0; i < texts->size; i++) {
        char *subText = texts->items[i];
        strcpy(t, subText);
        t += strlen(subText);
    }
    *t = 0;

    List_delete(texts);

    return text;
}

void CodeBlockBoard_load(CodeBlockBoard *this, const char *script) {

}

void CodeBlockBoard_save(CodeBlockBoard *this, const char *script) {
    FILE *file = fopen(script, "w");
    if (!file)
        return;
    char *text = CodeBlockBoard_totext(this);

    fwrite(text, sizeof(char), strlen(text), file);

    fclose(file);
    free(text);
}

/// @}


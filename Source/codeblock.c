/**
 * @file codeblock.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/31/18
 * @brief CodeBlock implementation
 * @addtogroup Components
 * @{
 */
#include "codeblock.h"

#include <Engine/transform.h>

#include <string.h>


/**
 * @brief Create new CodeBlock
 * @param type     Type of CodeBlock
 * @param data     Data of CodeBlock
 * @param dataSize Size of data
 * @return New CodeBlock
 */
CodeBlock *CodeBlock_new(CodeBlockType type, void *data, size_t dataSize) {
    CodeBlock *this = malloc(sizeof(CodeBlock));
    this->comp.typeName = "CodeBlock";
    this->comp.typeId = CODEBLOCK;
    this->comp.clone = _CodeBlock_clone;
    this->comp.delete = _CodeBlock_delete;
    this->comp.update = _CodeBlock_update;
    this->comp.draw = _CodeBlock_draw;
    this->comp.collides = false;
    this->comp.coll_resolve = NULL;
    this->comp.owner = NULL;

    this->type = type;
    this->blocks = List_new(10, _CodeBlock_clone, _CodeBlock_delete);
    List_resize(this->blocks, CodeBlock_types[type].numArgs, NULL);
    for (unsigned i = 0; i < this->blocks->size; i++)
        this->blocks->items[i] = CodeBlock_new(CB_EMPTY, NULL, 0);
    if (data) {
        this->data = malloc(dataSize);
        memcpy(this->data, data, dataSize);
    } else this->data = NULL;
    this->dataSize = dataSize;

    return this;
}

/**
 * @brief Clone CodeBlock
 * @param this CodeBlock to clone
 * @return Cloned CodeBlock
 */
CodeBlock *_CodeBlock_clone(CodeBlock *this) {
    CodeBlock *new = malloc(sizeof(CodeBlock));

    new->type = this->type;
    new->blocks = List_copy(this->blocks);
    new->data = malloc(this->dataSize);
    memcpy(new->data, this->data, this->dataSize);
    new->dataSize = this->dataSize;

    return new;
}

/**
 * @brief Delete CodeBlock
 * @param this CodeBlock to delete
 */
void _CodeBlock_delete(CodeBlock *this) {
    if (!this)
        return;
    List_delete(this->blocks);
    free(this->data);
    free(this);
}

/**
 * @brief Update CodeBlock
 * @param this CodeBlock to update
 */
void _CodeBlock_update(CodeBlock *this) {

}

/**
 * @brief Draw CodeBlock
 * @param this CodeBlock to draw
 */
void _CodeBlock_draw(CodeBlock *this) {
    Transform *trs = Object_getComp(this->comp.owner, TRANSFORM);
    if (trs)
        CodeBlock_draw(this, trs->pos);
}

/**
 * @brief Set CodeBlock 's sub CodeBlock num to given block
 * @param this  CodeBlock to add to
 * @param num   Index to add at
 * @param block CodeBlock to add
 */
void CodeBlock_setblock(CodeBlock *this, unsigned num, CodeBlock *block) {
    if (num >= this->blocks->size) return;
    CodeBlock **bp = this->blocks->items+num;
    if (bp)
        _CodeBlock_delete(*bp);
    *bp = block;
}

/**
 * @brief Add CodeBlock to another CodeBlock
 * @param this  CodeBlock to add to
 * @param block CodeBlock to add
 */
void CodeBlock_addblock(CodeBlock *this, CodeBlock *block) {
    List_push_back(this->blocks, block);
}

/**
 * @brief Get size taken up by CodeBlock
 * @param this CodeBlock to get size of
 * @return Size of CodeBlock
 */
vec2_t CodeBlock_getsize(CodeBlock *this) {
    return CodeBlock_types[this->type].getsize(this);
}

/**
 * @brief Attempt to grab this CodeBlock
 * @param this CodeBlock to grab
 * @param p    Mouse position on CodeBlock
 * @return Whether it was grabbed
 */
int CodeBlock_grab(CodeBlock *this, vec2_t p) {
    return CodeBlock_types[this->type].grab(this, p);
}

/**
 * @brief Attempt to drop a CodeBlock onto another
 * @param this    CodeBlock to drop on
 * @param dropped CodeBlock to drop
 * @param p       Mouse position on CodeBlock
 * @return Whether it was dropped
 */
int CodeBlock_drop(CodeBlock *this, CodeBlock *dropped, vec2_t p) {
    return CodeBlock_types[this->type].drop(this, dropped, p);
}

/**
 * @brief Draw CodeBlock at given position
 * @param this CodeBlock to draw
 * @param pos  vec2_t position to use
 * @return Size of drawn CodeBlock
 */
vec2_t CodeBlock_draw(CodeBlock *this, vec2_t pos) {
    return CodeBlock_types[this->type].draw(this, pos);
}

/**
 * @brief Convert CodeBlock to text
 * @param this CodeBlock to convert
 * @return Newly allocated string
 */
char *CodeBlock_text(CodeBlock *this) {
    return CodeBlock_types[this->type].text(this);
}

/// @}


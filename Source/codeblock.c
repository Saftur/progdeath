/**
 * @file codeblock.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/31/18
 * @brief CodeBlock implementation
 * @addtogroup Components
 * @{
 */
#include "codeblock.h"

#include <string.h>

#include "cb_empty.h"
#include "cb_sequence.h"
#include "cb_setvar.h"
#include "cb_var.h"
#include "cb_num.h"
#include "cb_str.h"
#include "cb_binaryop.h"
#include "cb_unaryop.h"
#include "cb_if.h"
#include "cb_while.h"
#include "cb_for.h"
#include "cb_foreach.h"

static CodeBlock_newfunc new_funcs[] = {
    cb_empty_new,
    cb_sequence_new,
    cb_setvar_new,
    cb_var_new,
    cb_num_new,
    cb_str_new,
    cb_binaryop_new,
    cb_unaryop_new,
    cb_if_new,
    cb_while_new,
    cb_for_new,
    cb_foreach_new,
};

/**
 * @brief Create new CodeBlock
 * @param type     Type of CodeBlock
 * @param data     Data of CodeBlock
 * @param dataSize Size of data
 * @return New CodeBlock
 */
CodeBlock *CodeBlock_new(CodeBlockType type, void *data, size_t dataSize) {
    CodeBlock *this = malloc(sizeof(CodeBlock));
    this->typeData = (CodeBlockTypeData){0};
    new_funcs[type](this);

    this->type = type;
    this->blocks = List_new(10, CodeBlock_clone, CodeBlock_delete);
    if (data) {
        this->data = malloc(dataSize);
        memcpy(this->data, data, dataSize);
    } else this->data = NULL;
    this->dataSize = dataSize;

    if (this->typeData.init)
        this->typeData.init(this);

    return this;
}

/**
 * @brief Clone CodeBlock
 * @param this CodeBlock to clone
 * @return Cloned CodeBlock
 */
CodeBlock *CodeBlock_clone(CodeBlock *this) {
    CodeBlock *new = malloc(sizeof(CodeBlock));
    new->typeData = this->typeData;

    new->type = this->type;
    new->blocks = List_copy(this->blocks);
    if (!new->typeData.ignData) {
        new->data = malloc(this->dataSize);
        memcpy(new->data, this->data, this->dataSize);
        new->dataSize = this->dataSize;
    }

    if (new->typeData.clone)
        new->typeData.clone(new, this);

    return new;
}

/**
 * @brief Delete CodeBlock
 * @param this CodeBlock to delete
 */
void CodeBlock_delete(CodeBlock *this) {
    if (!this)
        return;
    if (this->typeData.delete)
        this->typeData.delete(this);
    List_delete(this->blocks);
    if (!this->typeData.ignData)
        free(this->data);
    free(this);
}

/**
 * @brief Update CodeBlock
 * @param this CodeBlock to update
 * @param pos  Current position of CodeBlock
 */
void CodeBlock_update(CodeBlock *this, vec2_t pos) {
    if (!this) return;
    if (this->typeData.update)
        this->typeData.update(this, pos);
    /*for (unsigned i = 0; i < this->blocks->size; i++)
        CodeBlock_update(this->blocks->items[i]);*/
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
        CodeBlock_delete(*bp);
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
    return this->typeData.getsize(this);
}

/**
 * @brief Attempt to grab this CodeBlock
 * @param this CodeBlock to grab
 * @param p    Mouse position on CodeBlock
 * @return Whether it was grabbed
 */
CBGrabResult CodeBlock_grab(CodeBlock *this, vec2_t p) {
    return this->typeData.grab(this, p, 0);
}

/**
 * @brief Attempt to grab this CodeBlock
 * @param this CodeBlock to grab
 * @param p    Mouse position on CodeBlock
 * @param test Whether or not it should actually grab or just check
 * @return Whether it was grabbed
 */
CBGrabResult CodeBlock_grab_test(CodeBlock *this, vec2_t p, int test) {
    return this->typeData.grab(this, p, test);
}

/**
 * @brief Attempt to drop a CodeBlock onto another
 * @param this    CodeBlock to drop on
 * @param dropped CodeBlock to drop
 * @param p       Mouse position on CodeBlock
 * @return Whether it was dropped
 */
int CodeBlock_drop(CodeBlock *this, CodeBlock *dropped, vec2_t p) {
    if (this->typeData.drop)
        return this->typeData.drop(this, dropped, p);
    return 0;
}

/**
 * @brief Draw CodeBlock at given position
 * @param this CodeBlock to draw
 * @param pos  vec2_t position to use
 * @return Size of drawn CodeBlock
 */
vec2_t CodeBlock_draw(CodeBlock *this, vec2_t pos) {
    return this->typeData.draw(this, pos);
}

/**
 * @brief Convert CodeBlock to text
 * @param this CodeBlock to convert
 * @return Newly allocated string
 */
char *CodeBlock_totext(CodeBlock *this) {
    return this->typeData.totext(this);
}

/// @}


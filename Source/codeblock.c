/**
 * @file codeblock.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/28/18
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
    this->comp.typeId = ILLEGAL_COMP;
    this->comp.delete = _CodeBlock_delete;
    this->comp.update = _CodeBlock_update;
    this->comp.draw = _CodeBlock_draw;
    this->comp.collides = false;
    this->comp.coll_resolve = NULL;

    this->type = type;
    this->blocks = List_new(10, _CodeBlock_delete);
    if (data) {
        this->data = malloc(dataSize);
        memcpy(this->data, data, dataSize);
    } else this->data = NULL;

    return this;
}

/**
 * @brief Delete CodeBlock
 * @param this CodeBlock to delete
 */
void _CodeBlock_delete(CodeBlock *this) {
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
 * @brief Get size taken up by CodeBlock
 * @param this CodeBlock to get size of
 * @return Size of CodeBlock
 */
vec2_t CodeBlock_getsize(CodeBlock *this) {
    return CodeBlock_getsizefuncs[this->type](this);
}

/**
 * @brief Draw CodeBlock
 * @param this CodeBlock to draw
 */
void _CodeBlock_draw(CodeBlock *this) {
    Transform *trs = Object_getComp(this->comp.owner, TRANSFORM);
    if (trs)
        CodeBlock_draw(this, trs->pos/*, CBDM_FULL*/);
}

/**
 * @brief Draw CodeBlock at given position
 * @param this CodeBlock to draw
 * @param pos  vec2_t position to use
 * @return Size of drawn CodeBlock
 */
vec2_t CodeBlock_draw(CodeBlock *this, vec2_t pos) {
    return CodeBlock_drawfuncs[this->type](this, pos);
}

/**
 * @brief Convert CodeBlock to text
 * @param this CodeBlock to convert
 * @return Newly allocated string
 */
char *CodeBlock_text(CodeBlock *this) {
    return CodeBlock_textfuncs[this->type](this);
}

/// @}


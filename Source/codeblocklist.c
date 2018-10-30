/**
 * @file codeblocklist.c
 * @author Name (username)
 * @date 
 * @brief CodeBlockList implementation
 * @addtogroup Components
 * @{
 */
#include "codeblocklist.h"

#include "codeblock.h"

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

    this->cBlocks = List_new(30, _CodeBlock_clone, _CodeBlock_delete);

    return this;
}

/**
 * @brief Clone CodeBlockList
 * @param this CodeBlockList to clone
 * @return Cloned CodeBlockList
 */
CodeBlockList *_CodeBlockList_clone(CodeBlockList *this) {
    CodeBlockList *new = malloc(sizeof(CodeBlockList));

    new->cBlocks = List_copy(this->cBlocks);

    return new;
}

/**
 * @brief Delete CodeBlockList
 * @param this CodeBlockList to delete
 */
void _CodeBlockList_delete(CodeBlockList *this) {
    List_delete(this->cBlocks);
    free(this);
}

/**
 * @brief Update CodeBlockList
 * @param this CodeBlockList to update
 */
void _CodeBlockList_update(CodeBlockList *this) {

}

/**
 * @brief Draw CodeBlockList
 * @param this CodeBlockList to draw
 */
void _CodeBlockList_draw(CodeBlockList *this) {

}

/// @}


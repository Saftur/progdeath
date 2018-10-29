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
    this->comp.delete = _CodeBlockList_delete;
    this->comp.update = _CodeBlockList_update;
    this->comp.draw = _CodeBlockList_draw;
    this->comp.collides = false;
    this->comp.coll_resolve = NULL;

    this->cBlocks = List_new(30, _CodeBlock_delete);

    return this;
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


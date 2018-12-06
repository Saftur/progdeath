/**
 * @file codeblocklist.h
 * @author Arthur Bouvier (a.bouvier)
 * @brief CodeBlockList Component
 * @course GAM100F18
 * Copyright © 2018 DigiPen (USA) Corporation
 * @addtogroup Components
 * @{
 */
#pragma once

#include <Engine/component.h>
#include "gamecomps.h"

typedef struct CodeBlock CodeBlock; ///< @brief CodeBlock forward declaration

/**
 * @brief CodeBlockList Component
 */
typedef struct CodeBlockList {
    Component comp; ///< @brief Component data

    float yscroll;

    List *blocks;    ///< @brief List of CodeBlock s
    List *blockPos;  ///< @brief CodeBlock positions in CodeBlockList
    List *blockSize; ///< @brief CodeBlock sizes in CodeBlockList
    vec2_t size;     ///< @brief Size of CodeBlockList
} CodeBlockList;

/**
 * @brief Create new CodeBlockList
 * @return New CodeBlockList
 */
CodeBlockList *CodeBlockList_new();

/**
 * @brief Clone CodeBlockList
 * @param this CodeBlockList to clone
 * @return Cloned CodeBlockList
 */
CodeBlockList *_CodeBlockList_clone(CodeBlockList *this);

/**
 * @brief Delete CodeBlockList
 * @param this CodeBlockList to delete
 */
void _CodeBlockList_delete(CodeBlockList *this);

/**
 * @brief Update CodeBlockList
 * @param this CodeBlockList to update
 */
void _CodeBlockList_update(CodeBlockList *this);
/**
 * @brief Draw CodeBlockList
 * @param this CodeBlockList to draw
 */
void _CodeBlockList_draw(CodeBlockList *this);

/**
 * @brief Add CodeBlock to CodeBlockList
 * @param this  CodeBlockList to add to
 * @param block CodeBlock to add
 */
void CodeBlockList_addBlock(CodeBlockList *this, CodeBlock *block);

/// @}

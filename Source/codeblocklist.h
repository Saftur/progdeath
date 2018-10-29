/**
 * @file codeblocklist.h
 * @author Name (username)
 * @date 
 * @brief CodeBlockList Component
 * @addtogroup Components
 * @{
 */
#pragma once

#include <Engine/component.h>
#include "gamecomps.h"

/**
 * @brief CodeBlockList Component
 */
typedef struct CodeBlockList {
    Component comp; ///< @brief Component data

    List *cBlocks; ///< @brief List of CodeBlock s
} CodeBlockList;

/**
 * @brief Create new CodeBlockList
 * @return New CodeBlockList
 */
CodeBlockList *CodeBlockList_new();

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

/// @}

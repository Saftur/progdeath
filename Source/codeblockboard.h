/**
 * @file codeblockboard.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/31/18
 * @brief CodeBlockBoard Component
 * @addtogroup Components
 * @{
 */
#pragma once

#include <Engine/component.h>
#include "gamecomps.h"

extern float cb_scale;
extern vec2_t cb_offset;

typedef struct CodeBlock CodeBlock; ///< @brief CodeBlock forward declaration

/**
 * @brief CodeBlockBoard Component
 */
typedef struct CodeBlockBoard {
    Component comp; ///< @brief Component data

    List *blocks;
    List *blockPos;
} CodeBlockBoard;

/**
 * @brief Create new CodeBlockBoard
 * @return New CodeBlockBoard
 */
CodeBlockBoard *CodeBlockBoard_new();

/**
 * @brief Clone CodeBlockBoard
 * @param this CodeBlockBoard to clone
 * @return Cloned CodeBlockBoard
 */
CodeBlockBoard *_CodeBlockBoard_clone(CodeBlockBoard *this);

/**
 * @brief Delete CodeBlockBoard
 * @param this CodeBlockBoard to delete
 */
void _CodeBlockBoard_delete(CodeBlockBoard *this);

/**
 * @brief Update CodeBlockBoard
 * @param this CodeBlockBoard to update
 */
void _CodeBlockBoard_update(CodeBlockBoard *this);
/**
 * @brief Draw CodeBlockBoard
 * @param this CodeBlockBoard to draw
 */
void _CodeBlockBoard_draw(CodeBlockBoard *this);

/**
 * @brief Add CodeBlock to CodeBlockBoard
 * @param this  CodeBlockBoard to add to
 * @param block CodeBlock to add
 * @param pos   Position to add at
 */
void CodeBlockBoard_addBlock(CodeBlockBoard *this, CodeBlock *block, vec2_t pos);

/// @}

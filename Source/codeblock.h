/**
 * @file codeblock.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/31/18
 * @brief CodeBlock Component
 * @addtogroup Components
 * @{
 */
#pragma once

#include <Engine/component.h>
#include "gamecomps.h"

#include "codeblocktypes.h"

/**
 * @brief CodeBlock Component
 */
typedef struct CodeBlock {
    Component comp; ///< @brief Component data

    CodeBlockType type; ///< @brief CodeBlock type
    List *blocks;       ///< @brief Contained CodeBlock s
    void *data;         ///< @brief CodeBlock data
    size_t dataSize;    ///< @brief Size of CodeBlock data
} CodeBlock;

/**
 * @brief Create new CodeBlock
 * @param type     Type of CodeBlock
 * @param data     Data of CodeBlock
 * @param dataSize Size of data
 * @return New CodeBlock
 */
CodeBlock *CodeBlock_new(CodeBlockType type, void *data, size_t dataSize);

/**
 * @brief Clone CodeBlock
 * @param this CodeBlock to clone
 * @return Cloned CodeBlock
 */
CodeBlock *_CodeBlock_clone(CodeBlock *this);

/**
 * @brief Delete CodeBlock
 * @param this CodeBlock to delete
 */
void _CodeBlock_delete(CodeBlock *this);

/**
 * @brief Update CodeBlock
 * @param this CodeBlock to update
 */
void _CodeBlock_update(CodeBlock *this);
/**
 * @brief Draw CodeBlock
 * @param this CodeBlock to draw
 */
void _CodeBlock_draw(CodeBlock *this);

/**
 * @brief Set CodeBlock 's sub CodeBlock num to given block
 * @param this  CodeBlock to add to
 * @param num   Index to add at
 * @param block CodeBlock to add
 */
void CodeBlock_setblock(CodeBlock *this, unsigned num, CodeBlock *block);
/**
 * @brief Add CodeBlock to another CodeBlock
 * @param this  CodeBlock to add to
 * @param block CodeBlock to add
 */
void CodeBlock_addblock(CodeBlock *this, CodeBlock *block);

/**
 * @brief Get size taken up by CodeBlock
 * @param this CodeBlock to get size of
 * @return Size of CodeBlock
 */
vec2_t CodeBlock_getsize(CodeBlock *this);
/**
 * @brief Attempt to grab this CodeBlock
 * @param this CodeBlock to grab
 * @param p    Mouse position on CodeBlock
 * @return Whether it was grabbed
 */
int CodeBlock_grab(CodeBlock *this, vec2_t p);
/**
 * @brief Attempt to drop a CodeBlock onto another
 * @param this    CodeBlock to drop on
 * @param dropped CodeBlock to drop
 * @param p       Mouse position on CodeBlock
 * @return Whether it was dropped
 */
int CodeBlock_drop(CodeBlock *this, CodeBlock *dropped, vec2_t p);

/**
 * @brief Draw CodeBlock at given position
 * @param this CodeBlock to draw
 * @param pos  vec2_t position to use
 * @return Size of drawn CodeBlock
 */
vec2_t CodeBlock_draw(CodeBlock *this, vec2_t pos);

/**
 * @brief Convert CodeBlock to text
 * @param this CodeBlock to convert
 * @return Newly allocated string
 */
char *CodeBlock_text(CodeBlock *this);

/// @}

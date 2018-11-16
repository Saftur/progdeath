/**
 * @file codeblock.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/31/18
 * @brief CodeBlock Component
 * @addtogroup Components
 * @{
 */
#pragma once

#include <stddef.h>

#include <C_Processing.h>
#include <Engine/list.h>

//#include "codeblocktypes.h"

typedef struct CodeBlock CodeBlock; ///< @brief CodeBlock forward declaration

typedef enum CodeBlockType {
    CB_EMPTY,
    CB_SEQUENCE,

    CB_FUNCTION,
    CB_SETVAR,

    CB_VAR,
    CB_NUM,
    CB_STR,
    CB_BINARYOP,
    CB_UNARYOP,

    CB_IF,
    CB_WHILE,
    CB_FOR,
    CB_FOREACH,

    CB_NUM_TYPES
} CodeBlockType;

typedef struct CBGrabResult {
    int either;
    int parent;
    int child;
} CBGrabResult;

extern int tablevel;

void incTab();
void decTab();

typedef void(*CodeBlock_newfunc)(CodeBlock*);

typedef void(*CodeBlock_initfunc)(CodeBlock*);
typedef void(*CodeBlock_clonefunc)(CodeBlock*,CodeBlock*);
typedef void(*CodeBlock_deletefunc)(CodeBlock*);
typedef vec2_t(*CodeBlock_getsizefunc)(CodeBlock*);
typedef void(*CodeBlock_updatefunc)(CodeBlock*,vec2_t);
typedef CBGrabResult(*CodeBlock_grabfunc)(CodeBlock*,vec2_t,int);
typedef int(*CodeBlock_dropfunc)(CodeBlock*,CodeBlock*,vec2_t);
typedef vec2_t(*CodeBlock_drawfunc)(CodeBlock*,vec2_t);
typedef char*(*CodeBlock_textfunc)(CodeBlock*);

typedef struct CodeBlockTypeData {
    int isDir;
    int isArg;
    int isLValue;
    int ignData;
    CodeBlock_initfunc init;
    CodeBlock_clonefunc clone;
    CodeBlock_deletefunc delete;
    CodeBlock_getsizefunc getsize;
    CodeBlock_updatefunc update;
    CodeBlock_grabfunc grab;
    CodeBlock_dropfunc drop;
    CodeBlock_drawfunc draw;
    CodeBlock_textfunc totext;
} CodeBlockTypeData;

/**
 * @brief CodeBlock Component
 */
typedef struct CodeBlock {
    CodeBlockTypeData typeData;

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
#define CodeBlock_new_nodata(type) CodeBlock_new(type, NULL, 0)

/**
 * @brief Clone CodeBlock
 * @param this CodeBlock to clone
 * @return Cloned CodeBlock
 */
CodeBlock *CodeBlock_clone(CodeBlock *this);

/**
 * @brief Delete CodeBlock
 * @param this CodeBlock to delete
 */
void CodeBlock_delete(CodeBlock *this);

/**
 * @brief Update CodeBlock
 * @param this CodeBlock to update
 * @param pos  Current position of CodeBlock
 */
void CodeBlock_update(CodeBlock *this, vec2_t pos);

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
CBGrabResult CodeBlock_grab(CodeBlock *this, vec2_t p);
/**
 * @brief Attempt to grab this CodeBlock
 * @param this CodeBlock to grab
 * @param p    Mouse position on CodeBlock
 * @param test Whether or not it should actually grab or just check
 * @return Whether it was grabbed
 */
CBGrabResult CodeBlock_grab_test(CodeBlock *this, vec2_t p, int test);
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
char *CodeBlock_totext(CodeBlock *this);

/// @}

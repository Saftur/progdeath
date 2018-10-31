/**
 * @file codeblocktypes.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/31/18
 * @brief CodeBlock types
 */
#pragma once

#include <stdlib.h>

#include <C_Processing.h>

typedef struct CodeBlock CodeBlock; ///< @brief CodeBlock forward declaration

typedef enum CodeBlockType {
    CB_EMPTY,

    CB_SETVAR,
    CB_VAR,

    CB_IF,

    CB_NUM_TYPES
} CodeBlockType;

extern int tablevel;

void incTab();
void decTab();

typedef vec2_t(*CodeBlock_getsizefunc)(CodeBlock*);
typedef int(*CodeBlock_grabfunc)(CodeBlock*,vec2_t);
typedef vec2_t(*CodeBlock_drawfunc)(CodeBlock*,vec2_t);
typedef char*(*CodeBlock_textfunc)(CodeBlock*);

typedef struct CodeBlockTypeData {
    size_t numArgs;
    CodeBlock_getsizefunc getsize;
    CodeBlock_grabfunc grab;
    CodeBlock_drawfunc draw;
    CodeBlock_textfunc text;
} CodeBlockTypeData;

extern CodeBlockTypeData CodeBlock_types[];

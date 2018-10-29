/**
 * @file codeblocktypes.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/29/18
 * CodeBlock types
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
typedef vec2_t(*CodeBlock_drawfunc)(CodeBlock*,vec2_t);
typedef char*(*CodeBlock_textfunc)(CodeBlock*);

extern size_t CodeBlock_numargs[];
extern CodeBlock_getsizefunc CodeBlock_getsizefuncs[];
extern CodeBlock_drawfunc CodeBlock_drawfuncs[];
extern CodeBlock_textfunc CodeBlock_textfuncs[];

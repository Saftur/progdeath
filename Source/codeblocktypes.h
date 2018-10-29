/**
 * @file codeblocktypes.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/28/18
 * CodeBlock types
 */
#pragma once

#include <stdlib.h>

#include <C_Processing.h>

typedef struct CodeBlock CodeBlock; ///< @brief CodeBlock forward declaration

typedef enum CodeBlockType {
    CB_SETVAR,
    CB_VAR,

    CB_IF,
} CodeBlockType;

extern int tablevel;

void incTab();
void decTab();

typedef vec2_t(*CodeBlock_getsizefunc)(CodeBlock*);
typedef vec2_t(*CodeBlock_drawfunc)(CodeBlock*,vec2_t);
typedef char*(*CodeBlock_textfunc)(CodeBlock*);

extern CodeBlock_getsizefunc CodeBlock_getsizefuncs[];
extern CodeBlock_drawfunc CodeBlock_drawfuncs[];
extern CodeBlock_textfunc CodeBlock_textfuncs[];

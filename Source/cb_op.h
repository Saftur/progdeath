/**
 * @file cb_op.h
 * @author Arthur Bouvier
 * 
 */
#pragma once

#include <stddef.h>

typedef struct CB_Op {
    const char *str;
    size_t len;
    const char *codeStr;
    size_t codeLen;
    int noParen;
} CB_Op;

#define OP_ID(block) (*(unsigned*)block->data)
#define OP(block) (ops[OP_ID(block)])
#define OP_STR(block) (OP(block).str)
#define OP_LEN(block) (OP(block).len)
#define OP_CODESTR(block) (OP(block).codeStr ? OP(block).codeStr : OP(block).str)
#define OP_CODELEN(block) (OP(block).codeStr ? OP(block).codeLen : OP(block).len)
#define OP_NOPAREN(block) (OP(block).noParen)

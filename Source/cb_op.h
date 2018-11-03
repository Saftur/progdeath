/**
 * @file cb_op.h
 * @author Arthur Bouvier
 * @date 
 * 
 */
#pragma once

#include <stddef.h>

typedef struct CB_Op {
    const char *str;
    size_t len;
    const char *codeStr;
    size_t codeLen;
} CB_Op;

#define OP(block) (ops[*(unsigned*)block->data])
#define OP_STR(block) (OP(block).str)
#define OP_LEN(block) (OP(block).len)
#define OP_CODESTR(block) (OP(block).codeStr ? OP(block).codeStr : OP(block).str)
#define OP_CODELEN(block) (OP(block).codeStr ? OP(block).codeLen : OP(block).len)

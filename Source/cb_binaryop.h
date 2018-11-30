/**
 * @file cb_binaryop.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/31/18
 * 
 */
#pragma once

typedef struct CodeBlock CodeBlock;

typedef enum CB_binaryop_type {
    CB_BOP_AND,
    CB_BOP_OR,

    CB_BOP_EQ,
    CB_BOP_NEQ,
    CB_BOP_LT,
    CB_BOP_GT,
    CB_BOP_LEQ,
    CB_BOP_GEQ,

    CB_BOP_ADD,
    CB_BOP_SUB,
    CB_BOP_MUL,
    CB_BOP_DIV,
    CB_BOP_MOD,

    CB_BOP_CMA,
    CB_BOP_PER,

    CB_BOP_NUMOPS
} CB_binaryop_type;

void cb_binaryop_new(CodeBlock *block);

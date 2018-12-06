/**
 * @file cb_unaryop.h
 * @author Arthur Bouvier (a.bouvier)
 * @course GAM100F18
 * Copyright © 2018 DigiPen (USA) Corporation
 */
#pragma once

typedef struct CodeBlock CodeBlock;

typedef enum CB_unaryop_type {
    CB_UOP_NOT,

    CB_UOP_NEG,
    CB_UOP_LEN,

    CB_UOP_LOCAL,

    CB_UOP_NUMOPS
} CB_unaryop_type;

void cb_unaryop_new(CodeBlock *block);

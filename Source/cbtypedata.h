/**
 * @file cbtypedata.h
 * @author Arthur Bouvier
 * @date 
 * 
 */
#pragma once

#include <C_Processing.h>
#include "codeblock.h"

#define PADD 2

#define HEIGHT 30
#define INNER_HEIGHT (HEIGHT - PADD * 2)

#define COLOR_TEXT      (PColor){  0,   0,   0, 255}
#define COLOR_EMPTY     (PColor){255, 255, 255, 255}
#define COLOR_CONTROL   (PColor){255, 255,   0, 255}
#define COLOR_DIRECTIVE (PColor){ 47,  47, 255, 255}
#define COLOR_ARG       (PColor){255, 191, 127, 255}

#define TEXT_SIZE (HEIGHT - PADD * 2)
#define TEXT_W(num) ((TEXT_SIZE / 2) * num)
#define TEXT_H() TEXT_SIZE
#define TEXT_YOFFSET (TEXT_SIZE * 2 / 6)

#define TABS_AS_SPACES 1

#if TABS_AS_SPACES
#define TABSIZE 4
#else
#define TABSIZE 1
#endif

extern int tablevel;

void incTab();

void decTab();

#define CURR_TABSIZE (TABSIZE * tablevel)

void addTabs(char *txt);

#define GRABRES(parent, child) (CBGrabResult){ parent || child, parent, child }
#define GRABRES_NEITHER GRABRES(0, 0)
#define GRABRES_PARENT  GRABRES(1, 0)
#define GRABRES_CHILD   GRABRES(0, 1)

int is_directive(CodeBlock *block);

int is_arg(CodeBlock *block);

CodeBlock *empty_new();

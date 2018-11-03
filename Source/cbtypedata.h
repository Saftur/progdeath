/**
 * @file cbtypedata.h
 * @author Arthur Bouvier
 * @date 
 * 
 */
#pragma once

#include <C_Processing.h>
#include "codeblock.h"
#include "editorscreen.h"

#define PADD 2
#define HEIGHT 40
#define INNER_HEIGHT (HEIGHT - PADD * 2)
#define RECT_RADIUS 4
#define ARG_RND_PADD 8
#define ARG_RND_BOX_PADD(height) ((height) * 0.40)

#define COLOR_TEXT      (PColor){  0,   0,   0, 255}
#define COLOR_EMPTY     (PColor){255, 255, 255, 255}
#define COLOR_CONTROL   (PColor){255, 255,   0, 255}
#define COLOR_DIRECTIVE (PColor){ 47,  47, 255, 255}
#define COLOR_ARG       (PColor){255, 191, 127, 255}
#define COLOR_OP        (PColor){  0, 255,   0, 255}

#define TEXT_SIZE (HEIGHT - 12)
#define TEXT_W(num) ((TEXT_SIZE / 2) * (num))
#define TEXT_H() TEXT_SIZE
#define TEXT_YOFFSET(boxHeight) ((boxHeight) / 2 + TEXT_SIZE * 0.33)

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

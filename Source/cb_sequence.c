/**
 * @file cb_sequence.c
 * @author Arthur Bouvier (a.bouvier)
 * @course GAM100F18
 * Copyright © 2018 DigiPen (USA) Corporation
 */
#include "cb_sequence.h"

#include <stdio.h>
#include <string.h>

#include "cbtypedata.h"

#define EXTRA_DROP_AREA (HEIGHT / 2)

static vec2_t getsize(CodeBlock *block) {
    vec2_t size = (vec2_t){ 100, 0 };
    for (unsigned i = 0; i < block->blocks->size; i++) {
        vec2_t innSize = CodeBlock_getsize(block->blocks->items[i]);
        size.x = max(size.x, innSize.x);
        size.y += innSize.y;
    }
    if (size.y == 0)
        size.y = EXTRA_DROP_AREA;
    return size;
}

static void update(CodeBlock *block, vec2_t pos) {
    for (unsigned i = 0; i < block->blocks->size; i++) {
        CodeBlock *b = block->blocks->items[i];
        CodeBlock_update(b, pos);
        pos.y += CodeBlock_getsize(b).y;
    }
}

static CBGrabResult grab(CodeBlock *block, vec2_t p, int test) {
    CBGrabResult res;
    vec2_t subP = p;
    for (unsigned i = 0; i < block->blocks->size; i++) {
        CodeBlock *b = block->blocks->items[i];
        res = CodeBlock_grab_test(b, subP, test);
        if (res.either) {
            if (res.parent && !test) {
                List_remove_nodelete(block->blocks, i);
                if (block->blocks->size == 0)
                    return GRABRES_PARENT; // For sequence, parent grab means it is now empty
            }
            return GRABRES_CHILD;
        }
        subP.y -= CodeBlock_getsize(b).y;
    }
    return GRABRES_NEITHER;
}

static int drop(CodeBlock *block, CodeBlock *dropped, vec2_t p) {
    vec2_t size = getsize(block);
    vec2_t subP = p;
    float prevInsAdd = EXTRA_DROP_AREA;
    vec2_t ins = (vec2_t){ 0, -prevInsAdd };
    vec2_t nextIns = (vec2_t){ size.x, 0 };
    for (unsigned i = 0; i < block->blocks->size; i++) {
        CodeBlock *b = block->blocks->items[i];
        float bHeight = CodeBlock_getsize(b).y;
        nextIns.y = ins.y + prevInsAdd + (bHeight / 2);
        prevInsAdd = bHeight / 2;
        if (CodeBlock_drop(b, dropped, subP)) {
            return 1;
        } else if (is_directive(dropped) && vec2_in_range(p, ins, nextIns)) {
            List_insert(block->blocks, i, dropped);
            return 1;
        }
        subP.y -= bHeight;
        ins.y = nextIns.y;
    }
    nextIns.y = ins.y + prevInsAdd + EXTRA_DROP_AREA;
    if (is_directive(dropped) && vec2_in_range(p, ins, nextIns)) {
        List_push_back(block->blocks, dropped);
        return 1;
    }
    return 0;
}

static vec2_t draw(CodeBlock *block, vec2_t pos) {
    vec2_t size = vec2_zero();
    vec2_t innPos = vec2_add(pos, vec2_zero());

    for (unsigned i = 0; i < block->blocks->size; i++) {
        vec2_t innSize = CodeBlock_draw(block->blocks->items[i], innPos);
        size.y += innSize.y;
        if (innSize.x > size.x)
            size.x = innSize.x;
        innPos.y += innSize.y;
    }

    if (size.y == 0)
        size.y = EXTRA_DROP_AREA;
    return size;
}

static char *totext(CodeBlock *block) {
    size_t len = 0;
    List *txts = List_new(20, NULL, free);
    List *txtLens = List_new(20, NULL, NULL);
    char *tabs = getTabs();

    for (unsigned i = 0; i < block->blocks->size; i++) {
        char *subText = CodeBlock_totext(block->blocks->items[i]);
        size_t subLen = strlen(subText);
        size_t txtLen = CURR_TABSIZE + subLen + 1;
        char *txt = malloc((txtLen+1) * sizeof(char));
        len += txtLen;
        snprintf(txt, txtLen+1, "%s%s\n", tabs, subText);
        free(subText);
        List_push_back(txts, txt);
        List_push_back(txtLens, txtLen);
    }

    free(tabs);

    char *txt = malloc((len+1) * sizeof(char));
    char *t = txt;
    for (unsigned i = 0; i < txts->size; i++) {
        strncpy(t, txts->items[i], (size_t)txtLens->items[i]);
        t += (size_t)txtLens->items[i];
    }
    txt[len] = 0;

    List_delete(txts);
    List_delete(txtLens);

    return txt;
}

void cb_sequence_new(CodeBlock *block) {
    block->typeData.isDir = 1;
    block->typeData.isArg = 0;
    block->typeData.init = NULL;
    block->typeData.delete = NULL;
    block->typeData.getsize = getsize;
    block->typeData.update = update;
    block->typeData.grab = grab;
    block->typeData.drop = drop;
    block->typeData.draw = draw;
    block->typeData.totext = totext;
}

/**
 * @file cb_if.c
 * @author Arthur Bouvier (a.bouvier)
 * @course GAM100F18
 * Copyright © 2018 DigiPen (USA) Corporation
 */
#include "cb_if.h"

#include <stdio.h>
#include <string.h>

#include "cbtypedata.h"

#define BORDER (HEIGHT / 5)
#define IF_TEXT_W TEXT_W(2.75f)

static void init(CodeBlock *block) {
    List_push_back(block->blocks, CodeBlock_new(CB_EMPTY, NULL, 0));
    List_push_back(block->blocks, CodeBlock_new(CB_SEQUENCE, NULL, 0));
}

static vec2_t gettopsize(CodeBlock *block) {
    vec2_t condSize = CodeBlock_getsize(sub_block(0));
    return (vec2_t){ IF_TEXT_W + condSize.x + PADD * 2, max(HEIGHT, condSize.y + PADD * 2) };
}

static vec2_t getsize(CodeBlock *block) {
    vec2_t topSize = gettopsize(block);
    vec2_t seqSize = vec2_add(CodeBlock_getsize(sub_block(1)), (vec2_t){ BORDER, BORDER });
    return (vec2_t){ max(topSize.x, BORDER + seqSize.x), topSize.y + seqSize.y + BORDER };
}

static vec2_t condpos() {
    return (vec2_t){ IF_TEXT_W + PADD, PADD };
}

static vec2_t seqpos(CodeBlock *block) {
    return (vec2_t){ BORDER, gettopsize(block).y };
}

static void update(CodeBlock *block, vec2_t pos) {
    CodeBlock *cond = sub_block(0);
    vec2_t condPos = vec2_add(pos, condpos());
    CodeBlock_update(cond, condPos);
    CodeBlock *sequence = sub_block(1);
    vec2_t seqPos = vec2_add(pos, seqpos(block));
    CodeBlock_update(sequence, seqPos);
}

static CBGrabResult grab(CodeBlock *block, vec2_t p, int test) {
    CodeBlock *cond = sub_block(0);
    CBGrabResult res = CodeBlock_grab_test(cond, vec2_sub(p, condpos()), test);
    if (res.either) {
        if (res.parent && !test)
            sub_block(0) = empty_new();
        return GRABRES_CHILD;
    }
    CodeBlock *sequence = sub_block(1);
    res = CodeBlock_grab_test(sequence, vec2_sub(p, seqpos(block)), test);
    if (res.either) {
        return GRABRES_CHILD;
    }
    if (vec2_in_range(p, vec2_zero(), (vec2_t){ getsize(block).x, gettopsize(block).y })) {
        if (!test) setGrabbed(block, p);
        return GRABRES_PARENT;
    }
    return GRABRES_NEITHER;
}

static int drop(CodeBlock *block, CodeBlock *dropped, vec2_t p) {
    CodeBlock *cond = sub_block(0);
    vec2_t condSize = CodeBlock_getsize(cond);
    vec2_t subP = vec2_sub(p, condpos());
    if (CodeBlock_drop(cond, dropped, subP)) {
        return 1;
    } else if (is_arg(dropped) && vec2_in_range(subP, vec2_zero(), condSize)) {
        CodeBlock_delete(cond);
        block->blocks->items[0] = dropped;
        return 1;
    }
    CodeBlock *sequence = sub_block(1);
    vec2_t seqSize = CodeBlock_getsize(sequence);
    if (CodeBlock_drop(sequence, dropped, vec2_sub(p, seqpos(block))))
        return 1;
    return 0;
}

static vec2_t draw(CodeBlock *block, vec2_t pos) {
    vec2_t topSize = gettopsize(block);
    vec2_t seqSize = CodeBlock_draw(sub_block(1), vec2_add(pos, seqpos(block)));
    vec2_t size = (vec2_t){ max(topSize.x, BORDER + seqSize.x), topSize.y + seqSize.y + BORDER };
    noStroke();
    fillColor(COLOR_CONTROL);
    rectRounded(pos.x, pos.y, size.x, topSize.y, RECT_RADIUS);
    rectRounded(pos.x, pos.y + size.y - BORDER, size.x, BORDER, RECT_RADIUS);
    rect(pos.x, pos.y + topSize.y - RECT_RADIUS, BORDER, seqSize.y + RECT_RADIUS * 2);

    textSize(TEXT_SIZE);
    fillColor(COLOR_TEXT);
    text("if", pos.x + PADD + TEXT_W(0.25f), pos.y + TEXT_YOFFSET(topSize.y));
    CodeBlock_draw(sub_block(0), vec2_add(pos, condpos()));
    return size;
}

static char *totext(CodeBlock *block) {
    char *cond = CodeBlock_totext(sub_block(0));
    incTab();
    char *seq = CodeBlock_totext(sub_block(1));
    decTab();
    size_t len = 13 + CURR_TABSIZE + strlen(cond) + strlen(seq);

    char *txt = malloc((len+1) * sizeof(char));
    char *tabs = getTabs();
    sprintf(txt, "if %s then\n%s\n%send", cond, seq, tabs);

    free(cond);
    free(seq);
    free(tabs);

    return txt;
}

void cb_if_new(CodeBlock *block) {
    block->typeData.isDir = 1;
    block->typeData.isArg = 0;
    block->typeData.init = init;
    block->typeData.delete = NULL;
    block->typeData.getsize = getsize;
    block->typeData.update = update;
    block->typeData.grab = grab;
    block->typeData.drop = drop;
    block->typeData.draw = draw;
    block->typeData.totext = totext;
}

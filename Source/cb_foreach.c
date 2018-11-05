/**
 * @file cb_foreach.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/31/18
 * 
 */
#include "cb_foreach.h"

#include <stdio.h>
#include <string.h>

#include "cbtypedata.h"

#define BORDER (HEIGHT / 5)
#define FOR_TEXT_W TEXT_W(3.75f)
#define NUMARGS 2

static void init(CodeBlock *block) {
    List_push_back(block->blocks, empty_new());
    List_push_back(block->blocks, empty_new());
    List_push_back(block->blocks, CodeBlock_new(CB_SEQUENCE, NULL, 0));
}

static vec2_t gettopsize(CodeBlock *block) {
    vec2_t size = (vec2_t){ FOR_TEXT_W + TEXT_W(2 + 2 * 2) + PADD * 2, HEIGHT };
    for (unsigned i = 0; i < NUMARGS; i++) {
        vec2_t argSize = CodeBlock_getsize(sub_block(i));
        size.y = max(size.y, argSize.y + PADD * 2);
        size.x += argSize.x;
    }
    return size;
}

static vec2_t getsize(CodeBlock *block) {
    vec2_t topSize = gettopsize(block);
    vec2_t seqSize = vec2_add(CodeBlock_getsize(sub_block(NUMARGS)), (vec2_t){ BORDER, BORDER });
    return (vec2_t){ max(topSize.x, BORDER + seqSize.x), topSize.y + seqSize.y + BORDER };
}

static vec2_t varpos() {
    return (vec2_t){ PADD + FOR_TEXT_W, PADD };
}

static vec2_t argpos(vec2_t varSize) {
    vec2_t pos = (vec2_t){ PADD + FOR_TEXT_W + TEXT_W(3) + varSize.x, PADD };
    return pos;
}

static vec2_t seqpos(CodeBlock *block) {
    return (vec2_t){ BORDER, gettopsize(block).y };
}

static void update(CodeBlock *block, vec2_t pos) {
    CodeBlock *var = sub_block(0);
    vec2_t varPos = varpos();
    CodeBlock_update(var, vec2_add(pos, varPos));
    vec2_t varSize = CodeBlock_getsize(var);

    CodeBlock *arg = sub_block(1);
    CodeBlock_update(arg, vec2_add(pos, argpos(varSize)));

    CodeBlock *sequence = sub_block(NUMARGS);
    vec2_t seqPos = vec2_add(pos, seqpos(block));
    CodeBlock_update(sequence, seqPos);
}

static CBGrabResult grab(CodeBlock *block, vec2_t p, int test) {
    CBGrabResult res;
    CodeBlock *var = sub_block(0);
    vec2_t varPos = varpos();
    res = CodeBlock_grab_test(var, vec2_sub(p, varPos), test);
    if (res.either) {
        if (res.parent && !test)
            sub_block(0) = empty_new();
        return GRABRES_CHILD;
    }
    vec2_t varSize = CodeBlock_getsize(var);
    
    CodeBlock *arg = sub_block(1);
    vec2_t argPos = argpos(varSize);
    res = CodeBlock_grab_test(arg, vec2_sub(p, argPos), test);
    if (res.either) {
        if (res.parent && !test)
            sub_block(1) = empty_new();
        return GRABRES_CHILD;
    }

    CodeBlock *sequence = sub_block(NUMARGS);
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
    CodeBlock *var = sub_block(0);
    vec2_t varPos = varpos();
    vec2_t varSize = CodeBlock_getsize(var);
    vec2_t subP = vec2_sub(p, varPos);
    if (CodeBlock_drop(var, dropped, subP)) {
        return 1;
    } else if (is_lvalue(dropped) && vec2_in_range(subP, vec2_zero(), varSize)) {
        CodeBlock_delete(var);
        sub_block(0) = dropped;
        return 1;
    }

    CodeBlock *arg = sub_block(1);
    vec2_t argPos = argpos(varSize);
    vec2_t argSize = CodeBlock_getsize(arg);
    subP = vec2_sub(p, argPos);
    if (CodeBlock_drop(arg, dropped, subP)) {
        return 1;
    } else if (is_arg(dropped) && vec2_in_range(subP, vec2_zero(), argSize)) {
        CodeBlock_delete(arg);
        sub_block(1) = dropped;
        return 1;
    }

    CodeBlock *sequence = sub_block(NUMARGS);
    vec2_t seqSize = CodeBlock_getsize(sequence);
    if (CodeBlock_drop(sequence, dropped, vec2_sub(p, seqpos(block))))
        return 1;
    return 0;
}

static vec2_t draw(CodeBlock *block, vec2_t pos) {
    vec2_t topSize = gettopsize(block);
    vec2_t seqSize = CodeBlock_draw(sub_block(NUMARGS), vec2_add(pos, seqpos(block)));
    vec2_t size = (vec2_t){ max(topSize.x, BORDER + seqSize.x), topSize.y + seqSize.y + BORDER };
    noStroke();
    fillColor(COLOR_CONTROL);
    rectRounded(pos.x, pos.y, size.x, topSize.y, RECT_RADIUS);
    rectRounded(pos.x, pos.y + size.y - BORDER, size.x, BORDER, RECT_RADIUS);
    rect(pos.x, pos.y + topSize.y - RECT_RADIUS, BORDER, seqSize.y + RECT_RADIUS * 2);

    vec2_t varSize = CodeBlock_draw(sub_block(0), vec2_add(pos, varpos()));
    textSize(TEXT_SIZE);
    fillColor(COLOR_TEXT);
    text("for", pos.x + PADD + TEXT_W(0.25f), pos.y + TEXT_YOFFSET(topSize.y));
    text("in", pos.x + PADD + FOR_TEXT_W + TEXT_W(0.5f) + varSize.x, pos.y + TEXT_YOFFSET(topSize.y));
    CodeBlock_draw(sub_block(1), vec2_add(pos, argpos(varSize)));

    return size;
}

static char *totext(CodeBlock *block) {
    size_t len = 16 + CURR_TABSIZE;

    char *argTexts[NUMARGS];
    for (unsigned i = 0; i < NUMARGS; i++)
        len += strlen(argTexts[i] = CodeBlock_totext(sub_block(i)));

    incTab();
    char *seq = CodeBlock_totext(sub_block(NUMARGS));
    decTab();
    len += strlen(seq);

    char *txt = malloc((len+1) * sizeof(char));
    char *tabs = getTabs();
    sprintf(txt, "for %s in %s do\n%s\n%send", argTexts[0], argTexts[1], seq, tabs);

    for (unsigned i = 0; i < NUMARGS; i++)
        free(argTexts[i]);
    free(seq);
    free(tabs);

    return txt;
}

void cb_foreach_new(CodeBlock *block) {
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

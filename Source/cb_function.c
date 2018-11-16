/**
 * @file cb_function.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/31/18
 * 
 */
#include "cb_function.h"

#include "cbtypedata.h"

#include <stdio.h>
#include <string.h>

static void init(CodeBlock *block) {
    List_push_back(block->blocks, CodeBlock_new(CB_NUM, NULL, 0));
}

static vec2_t getsize(CodeBlock *block) {
    CodeBlock *var = block->blocks->items[0];
    vec2_t varSize = CodeBlock_getsize(var);
    return (vec2_t){ varSize.x + TEXT_W(strlen(block->data)+2) + TEXT_ARG_RND_PADD * 2, max(HEIGHT, varSize.y + PADD * 2) };
}

static vec2_t varpos(vec2_t size, vec2_t varSize, float textSize) {
    return (vec2_t){ TEXT_ARG_RND_PADD + textSize, size.y / 2 - varSize.y / 2 };
}

static void update(CodeBlock *block, vec2_t pos) {
    vec2_t size = getsize(block);
    CodeBlock *var = block->blocks->items[0];
    vec2_t varSize = CodeBlock_getsize(var);

    CodeBlock_update(var, vec2_add(pos, varpos(size, varSize, TEXT_W(strlen(block->data)+1))));
}

static CBGrabResult grab(CodeBlock *block, vec2_t p, int test) {
    vec2_t size = getsize(block);
    CodeBlock *var = block->blocks->items[0];
    vec2_t varSize = CodeBlock_getsize(var);
    vec2_t varPos = varpos(size, varSize, TEXT_W(strlen(block->data)+1));
    CBGrabResult res;
    res = CodeBlock_grab_test(var, vec2_sub(p, varPos), test);
    if (res.either) {
        if (res.parent && !test)
            block->blocks->items[0] = empty_new();
        return GRABRES_CHILD;
    }
    if (vec2_in_range(p, vec2_zero(), getsize(block))) {
        if (!test) setGrabbed(block, p);
        return GRABRES_PARENT;
    }
    return GRABRES_NEITHER;
}

static int drop(CodeBlock *block, CodeBlock *dropped, vec2_t p) {
    vec2_t size = getsize(block);
    CodeBlock *var = block->blocks->items[0];
    vec2_t varSize = CodeBlock_getsize(var);
    vec2_t subP;
    vec2_t varPos = varpos(size, varSize, TEXT_W(strlen(block->data)+1));
    subP = vec2_sub(p, varPos);
    if (CodeBlock_drop(var, dropped, subP)) {
        return 1;
    } else if (is_lvalue(dropped) && vec2_in_range(subP, vec2_zero(), varSize)) {
        CodeBlock_delete(var);
        block->blocks->items[0] = dropped;
        return 1;
    }
    return 0;
}

static vec2_t draw(CodeBlock *block, vec2_t pos) {
    CodeBlock *var = block->blocks->items[0];
    vec2_t varSize = CodeBlock_getsize(var);
    vec2_t size = getsize(block);
    strokeColor(COLOR_DIR_STROKE);
    strokeWeight(1);
    fillColor(COLOR_DIRECTIVE);
    rectRounded(pos.x, pos.y, size.x, size.y, size.y / 2);
    textSize(TEXT_SIZE);
    fillColor(COLOR_TEXT);
    text(block->data, pos.x + TEXT_ARG_RND_PADD, pos.y + TEXT_YOFFSET(size.y));
    text("(", pos.x + TEXT_ARG_RND_PADD + TEXT_W(strlen(block->data)), pos.y + TEXT_YOFFSET(size.y));
    text(")", pos.x + TEXT_ARG_RND_PADD + TEXT_W(strlen(block->data) + 1) + varSize.x, pos.y + TEXT_YOFFSET(size.y));
    CodeBlock_draw(var, vec2_add(pos, varpos(size, varSize, TEXT_W(strlen(block->data)+1))));
    return size;
}

static char *totext(CodeBlock *block) {
    char *var = CodeBlock_totext(block->blocks->items[0]);
    size_t varlen = strlen(var);
    size_t len = 5 + varlen + 1;
    char *txt = malloc((len+1) * sizeof(char));

    sprintf(txt, "%s(%s)", block->data, var);

    free(var);

    return txt;
}

void cb_function_new(CodeBlock *block) {
    block->typeData.isDir = 1;
    block->typeData.isArg = 1;
    block->typeData.init = init;
    block->typeData.delete = NULL;
    block->typeData.getsize = getsize;
    block->typeData.update = update;
    block->typeData.grab = grab;
    block->typeData.drop = drop;
    block->typeData.draw = draw;
    block->typeData.totext = totext;
}

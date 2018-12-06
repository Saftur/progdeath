/**
 * @file cb_setvar.c
 * @author Arthur Bouvier (a.bouvier)
 * 
 */
#include "cb_setvar.h"

#include "cbtypedata.h"

static void init(CodeBlock *block) {
    List_push_back(block->blocks, CodeBlock_new(CB_EMPTY, NULL, 0));
    List_push_back(block->blocks, CodeBlock_new(CB_NUM, NULL, 0));
}

static vec2_t getsize(CodeBlock *block) {
    CodeBlock *var = block->blocks->items[0];
    CodeBlock *val = block->blocks->items[1];
    vec2_t varSize = CodeBlock_getsize(var);
    vec2_t valSize = CodeBlock_getsize(val);
    return (vec2_t){ varSize.x + valSize.x + TEXT_W(2) + PADD * 2, max(HEIGHT, max(varSize.y, valSize.y) + PADD * 2) };
}

static vec2_t varpos(vec2_t size, vec2_t varSize) {
    return (vec2_t){ PADD, size.y / 2 - varSize.y / 2 };
}

static vec2_t valpos(vec2_t size, vec2_t varSize, vec2_t valSize) {
    return (vec2_t){ PADD + varSize.x + TEXT_W(2), size.y / 2 - valSize.y / 2 };
}

static void update(CodeBlock *block, vec2_t pos) {
    vec2_t size = getsize(block);
    CodeBlock *var = block->blocks->items[0];
    CodeBlock *val = block->blocks->items[1];
    vec2_t varSize = CodeBlock_getsize(var);
    vec2_t valSize = CodeBlock_getsize(val);

    CodeBlock_update(var, vec2_add(pos, varpos(size, varSize)));
    CodeBlock_update(val, vec2_add(pos, valpos(size, varSize, valSize)));
}

static CBGrabResult grab(CodeBlock *block, vec2_t p, int test) {
    vec2_t size = getsize(block);
    CodeBlock *var = block->blocks->items[0];
    CodeBlock *val = block->blocks->items[1];
    vec2_t varSize = CodeBlock_getsize(var);
    vec2_t valSize = CodeBlock_getsize(val);
    vec2_t varPos = varpos(size, varSize);
    CBGrabResult res;
    res = CodeBlock_grab_test(var, vec2_sub(p, varPos), test);
    if (res.either) {
        if (res.parent && !test)
            block->blocks->items[0] = empty_new();
        return GRABRES_CHILD;
    }
    vec2_t valPos = valpos(size, varSize, valSize);
    res = CodeBlock_grab_test(val, vec2_sub(p, valPos), test);
    if (res.either) {
        if (res.parent && !test)
            block->blocks->items[1] = num_new();
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
    CodeBlock *val = block->blocks->items[1];
    vec2_t varSize = CodeBlock_getsize(var);
    vec2_t valSize = CodeBlock_getsize(val);
    vec2_t subP;
    vec2_t varPos = varpos(size, varSize);
    subP = vec2_sub(p, varPos);
    if (CodeBlock_drop(var, dropped, subP)) {
        return 1;
    } else if (is_lvalue(dropped) && vec2_in_range(subP, vec2_zero(), varSize)) {
        CodeBlock_delete(var);
        block->blocks->items[0] = dropped;
        return 1;
    }
    vec2_t valPos = valpos(size, varSize, valSize);
    subP = vec2_sub(p, valPos);
    if (CodeBlock_drop(val, dropped, subP)) {
        return 1;
    } else if (is_arg(dropped) && vec2_in_range(subP, vec2_zero(), valSize)) {
        CodeBlock_delete(val);
        block->blocks->items[1] = dropped;
        return 1;
    }
    return 0;
}

static vec2_t draw(CodeBlock *block, vec2_t pos) {
    CodeBlock *var = block->blocks->items[0];
    CodeBlock *val = block->blocks->items[1];
    vec2_t varSize = CodeBlock_getsize(var);
    vec2_t valSize = CodeBlock_getsize(val);
    vec2_t size = getsize(block);
    noStroke();
    fillColor(COLOR_DIRECTIVE);
    rectRounded(pos.x, pos.y, size.x, size.y, RECT_RADIUS);
    CodeBlock_draw(var, vec2_add(pos, varpos(size, varSize)));
    textSize(TEXT_SIZE);
    fillColor(COLOR_TEXT);
    text("=", pos.x + varSize.x + PADD + TEXT_W(0.5f), pos.y + TEXT_YOFFSET(size.y));
    CodeBlock_draw(val, vec2_add(pos, valpos(size, varSize, valSize)));
    return size;
}

static char *totext(CodeBlock *block) {
    char *var = CodeBlock_totext(block->blocks->items[0]);
    char *val = CodeBlock_totext(block->blocks->items[1]);
    size_t varlen = strlen(var);
    size_t vallen = strlen(val);
    size_t len = varlen + 3 + vallen;
    char *txt = malloc((len+1) * sizeof(char));

    strncpy(txt, var, varlen);
    strncpy(txt+varlen, " = ", 3);
    strncpy(txt+varlen+3, val, vallen);
    txt[len] = 0;

    free(var);
    free(val);

    return txt;
}

void cb_setvar_new(CodeBlock *block) {
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

/**
 * @file cb_setvar.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/31/18
 * 
 */
#include "cb_setvar.h"

#include "cbtypedata.h"

static vec2_t getsize(CodeBlock *block) {
    CodeBlock *var = (block->blocks->size >= 1) ? block->blocks->items[0] : NULL;
    CodeBlock *val = (block->blocks->size >= 2) ? block->blocks->items[1] : NULL;
    vec2_t varSize = var ? CodeBlock_getsize(var) : (vec2_t){ 0, 0 };
    vec2_t valSize = val ? CodeBlock_getsize(val) : (vec2_t){ 0, 0 };
    return (vec2_t){ varSize.x + valSize.x + TEXT_W(2) + PADD * 2, max(HEIGHT, max(varSize.y, valSize.y) + PADD * 2) };
}

static vec2_t varpos(CodeBlock *block, vec2_t size, vec2_t varSize) {
    return (vec2_t){ PADD, size.y / 2 - varSize.y / 2 };
}

static vec2_t valpos(CodeBlock *block, vec2_t size, vec2_t varSize, vec2_t valSize) {
    return (vec2_t){PADD + varSize.x + TEXT_W(2), size.y / 2 - valSize.y / 2};
}

static CBGrabResult grab(CodeBlock *block, vec2_t p) {
    vec2_t size = getsize(block);
    CodeBlock *var = (block->blocks->size >= 1) ? block->blocks->items[0] : NULL;
    CodeBlock *val = (block->blocks->size >= 2) ? block->blocks->items[1] : NULL;
    vec2_t varSize = var ? CodeBlock_getsize(var) : (vec2_t){ 0, 0 };
    vec2_t valSize = val ? CodeBlock_getsize(val) : (vec2_t){ 0, 0 };
    if (var) {
        vec2_t varPos = varpos(block, size, varSize);
        if (CodeBlock_grab(var, vec2_sub(p, varPos)).parent) {
            block->blocks->items[0] = empty_new();
            return GRABRES_CHILD;
        }
    }
    if (val) {
        vec2_t valPos = valpos(block, size, varSize, valSize);
        if (CodeBlock_grab(val, vec2_sub(p, valPos)).parent) {
            block->blocks->items[1] = empty_new();
            return GRABRES_CHILD;
        }
    }
    if (vec2_in_range(p, vec2_zero(), getsize(block))) {
        setGrabbed(block);
        return GRABRES_PARENT;
    }
    return GRABRES_NEITHER;
}

static int drop(CodeBlock *block, CodeBlock *dropped, vec2_t p) {
    vec2_t size = getsize(block);
    CodeBlock *var = (block->blocks->size >= 1) ? block->blocks->items[0] : NULL;
    CodeBlock *val = (block->blocks->size >= 2) ? block->blocks->items[1] : NULL;
    vec2_t varSize = var ? CodeBlock_getsize(var) : (vec2_t){ 0, 0 };
    vec2_t valSize = val ? CodeBlock_getsize(val) : (vec2_t){ 0, 0 };
    if (var) {
        vec2_t varPos = varpos(block, size, varSize);
        vec2_t subP = vec2_sub(p, varPos);
        if (CodeBlock_drop(var, dropped, subP)) {
            return 1;
        } else if (is_arg(dropped) && vec2_in_range(subP, vec2_zero(), varSize)) {
            CodeBlock_delete(var);
            block->blocks->items[0] = dropped;
            return 1;
        }
    }
    if (val) {
        vec2_t valPos = valpos(block, size, varSize, valSize);
        vec2_t subP = vec2_sub(p, valPos);
        if (CodeBlock_drop(val, dropped, subP)) {
            return 1;
        } else if (is_arg(dropped) && vec2_in_range(subP, vec2_zero(), valSize)) {
            CodeBlock_delete(val);
            block->blocks->items[1] = dropped;
            return 1;
        }
    }
    return 0;
}

static vec2_t draw(CodeBlock *block, vec2_t pos) {
    CodeBlock *var = (block->blocks->size >= 1) ? block->blocks->items[0] : NULL;
    CodeBlock *val = (block->blocks->size >= 2) ? block->blocks->items[1] : NULL;
    vec2_t varSize = var ? CodeBlock_getsize(var) : (vec2_t){ 0, 0 };
    vec2_t valSize = val ? CodeBlock_getsize(val) : (vec2_t){ 0, 0 };
    vec2_t size = getsize(block);
    noStroke();
    fillColor(COLOR_DIRECTIVE);
    rect(pos.x, pos.y, size.x, size.y);
    if (var)
        CodeBlock_draw(var, vec2_add(pos, varpos(block, size, varSize)));
    textSize(TEXT_SIZE);
    fillColor(COLOR_TEXT);
    text("=", pos.x + varSize.x + PADD + TEXT_W(1/2), pos.y + size.y / 2 + TEXT_YOFFSET);
    if (val)
        CodeBlock_draw(val, vec2_add(pos, valpos(block, size, varSize, valSize)));
    return size;
}

static char *totext(CodeBlock *block) {
    if (block->blocks->size < 2)
        return NULL;

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

void cb_setvar_init(CodeBlock *block) {
    block->typeData.numArgs = 2;
    block->typeData.update = NULL;
    block->typeData.getsize = getsize;
    block->typeData.grab = grab;
    block->typeData.drop = drop;
    block->typeData.draw = draw;
    block->typeData.totext = totext;
}

/**
 * @file cb_empty.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/31/18
 * 
 */
#include "cb_empty.h"

#include "cbtypedata.h"

static vec2_t getsize(CodeBlock *block) {
    return (vec2_t){ 100, INNER_HEIGHT };
}

static CBGrabResult grab(CodeBlock *block, vec2_t p) {
    return GRABRES_NEITHER;
}

static vec2_t draw(CodeBlock *block, vec2_t pos) {
    vec2_t size = getsize(block);
    noStroke();
    fillColor(COLOR_EMPTY);
    rect(pos.x, pos.y, size.x, size.y);
    return size;
}

static char *totext(CodeBlock *block) {
    char *txt = malloc(1 * sizeof(char));
    txt[0] = 0;
    return txt;
}

void cb_empty_init(CodeBlock *block) {
    block->typeData.numArgs = 0;
    block->typeData.update = NULL;
    block->typeData.getsize = getsize;
    block->typeData.grab = grab;
    block->typeData.drop = NULL;
    block->typeData.draw = draw;
    block->typeData.totext = totext;
}

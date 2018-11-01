/**
 * @file cb_var.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/31/18
 * 
 */
#include "cb_var.h"

#include "cbtypedata.h"

static vec2_t getsize(CodeBlock *block) {
    return (vec2_t){ TEXT_W(strlen(block->data)) + PADD * 2, max(INNER_HEIGHT, TEXT_H() + PADD * 2) };
}

static CBGrabResult grab(CodeBlock *block, vec2_t p) {
    if (vec2_in_range(p, vec2_zero(), getsize(block))) {
        setGrabbed(block);
        return GRABRES_PARENT;
    }
    return GRABRES_NEITHER;
}

static vec2_t draw(CodeBlock *block, vec2_t pos) {
    vec2_t size = getsize(block);
    noStroke();
    fillColor(COLOR_ARG);
    rect(pos.x, pos.y, size.x, size.y);
    textSize(TEXT_SIZE);
    fillColor(COLOR_TEXT);
    text(block->data, pos.x + PADD, pos.y + size.y / 2 + TEXT_YOFFSET);
    return size;
}

static char *totext(CodeBlock *block) {
    size_t len = strlen(block->data);
    char *txt = malloc((len+1) * sizeof(char));
    strcpy(txt, block->data);
    txt[len] = 0;
    return txt;
}

void cb_var_init(CodeBlock *block) {
    block->typeData.numArgs = 0;
    block->typeData.update = NULL;
    block->typeData.getsize = getsize;
    block->typeData.grab = grab;
    block->typeData.drop = NULL;
    block->typeData.draw = draw;
    block->typeData.totext = totext;
}

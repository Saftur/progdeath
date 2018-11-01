/**
 * @file cb_var.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/31/18
 * 
 */
#include "cb_var.h"

#include "cbtypedata.h"

static vec2_t getsize(CodeBlock *block) {
    return (vec2_t){ TEXT_W(strlen(block->data)) + ARG_RND_W * 2, max(INNER_HEIGHT, TEXT_H() + PADD * 2) };
}

static CBGrabResult grab(CodeBlock *block, vec2_t p, int test) {
    vec2_t size = getsize(block);
    float halfHeight = size.y / 2;
    int inRect = vec2_in_range(p, (vec2_t){ halfHeight, 0 }, vec2_sub(size, (vec2_t){ halfHeight, 0 }));
    vec2_t cCenter1 = (vec2_t){ halfHeight, halfHeight }, cCenter2 = (vec2_t){ size.x - halfHeight, halfHeight };
    if (inRect || vec2_length(vec2_sub(p, cCenter1)) < halfHeight || vec2_length(vec2_sub(p, cCenter2)) < halfHeight) {
        if (!test) setGrabbed(block, p);
        return GRABRES_PARENT;
    }
    return GRABRES_NEITHER;
}

static vec2_t draw(CodeBlock *block, vec2_t pos) {
    vec2_t size = getsize(block);
    noStroke();
    fillColor(COLOR_ARG);
    rectRounded(pos.x, pos.y, size.x, size.y, size.y / 2);
    textSize(TEXT_SIZE);
    fillColor(COLOR_TEXT);
    text(block->data, pos.x + ARG_RND_W, pos.y + TEXT_YOFFSET(size.y));
    return size;
}

static char *totext(CodeBlock *block) {
    size_t len = strlen(block->data);
    char *txt = malloc((len+1) * sizeof(char));
    strcpy(txt, block->data);
    txt[len] = 0;
    return txt;
}

void cb_var_new(CodeBlock *block) {
    block->typeData.isDir = 0;
    block->typeData.isArg = 1;
    block->typeData.numArgs = 0;
    block->typeData.init = NULL;
    block->typeData.delete = NULL;
    block->typeData.getsize = getsize;
    block->typeData.update = NULL;
    block->typeData.grab = grab;
    block->typeData.drop = NULL;
    block->typeData.draw = draw;
    block->typeData.totext = totext;
}

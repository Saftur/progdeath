/**
 * @file cb_unaryop.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/31/18
 * 
 */
#include "cb_unaryop.h"

#include "cbtypedata.h"
#include "cb_op.h"

static CB_Op ops[] = {
    {"not", 3, "not ", 4},
    {"-", 1},
    {"len", 3, "#", 1},
};

static void init(CodeBlock *block) {
    List_push_back(block->blocks, CodeBlock_new(CB_NUM, NULL, 0));
    List_push_back(block->blocks, CodeBlock_new(CB_NUM, NULL, 0));
}

static vec2_t getsize(CodeBlock *block) {
    CodeBlock *arg = block->blocks->items[0];
    vec2_t argSize = CodeBlock_getsize(arg);
    float height = max(INNER_HEIGHT, argSize.y + PADD * 2);
    return (vec2_t){ argSize.x + TEXT_W(OP_LEN(block)+1) + PADD * 2, height };
}

static vec2_t argpos(vec2_t size, int opLen, vec2_t argSize) {
    return (vec2_t){ PADD + TEXT_W(opLen+1), size.y / 2 - argSize.y / 2 };
}

static void update(CodeBlock *block, vec2_t pos) {
    vec2_t size = getsize(block);
    CodeBlock *arg = block->blocks->items[0];
    vec2_t argSize = CodeBlock_getsize(arg);

    CodeBlock_update(arg, vec2_add(pos, argpos(size, OP_LEN(block), argSize)));
}

static CBGrabResult grab(CodeBlock *block, vec2_t p, int test) {
    vec2_t size = getsize(block);
    CodeBlock *arg = block->blocks->items[0];
    vec2_t argSize = CodeBlock_getsize(arg);
    vec2_t argPos = argpos(size, OP_LEN(block), argSize);
    CBGrabResult res = CodeBlock_grab_test(arg, vec2_sub(p, argPos), test);
    if (res.either) {
        if (res.parent && !test)
            block->blocks->items[0] = num_new();
        return GRABRES_CHILD;
    }
    float halfHeight = size.y / 2;
    int inRect = vec2_in_range(p, (vec2_t){ halfHeight, 0 }, vec2_sub(size, (vec2_t){ halfHeight, 0 }));
    vec2_t cCenter1 = (vec2_t){ halfHeight, halfHeight }, cCenter2 = (vec2_t){ size.x - halfHeight, halfHeight };
    if (inRect || vec2_length(vec2_sub(p, cCenter1)) < halfHeight || vec2_length(vec2_sub(p, cCenter2)) < halfHeight) {
        if (!test) setGrabbed(block, p);
        return GRABRES_PARENT;
    }
    return GRABRES_NEITHER;
}

static int drop(CodeBlock *block, CodeBlock *dropped, vec2_t p) {
    vec2_t size = getsize(block);
    CodeBlock *arg = block->blocks->items[0];
    vec2_t argSize = CodeBlock_getsize(arg);
    vec2_t subP;
    vec2_t argPos = argpos(size, OP_LEN(block), argSize);
    subP = vec2_sub(p, argPos);
    if (CodeBlock_drop(arg, dropped, subP)) {
        return 1;
    } else if (is_arg(dropped) && vec2_in_range(subP, vec2_zero(), argSize)) {
        CodeBlock_delete(arg);
        block->blocks->items[0] = dropped;
        return 1;
    }
    return 0;
}

static vec2_t draw(CodeBlock *block, vec2_t pos) {
    vec2_t size = getsize(block);
    CodeBlock *arg = block->blocks->items[0];
    vec2_t argSize = CodeBlock_getsize(arg);
    //noStroke();
    strokeColor(COLOR_OP_STROKE);
    strokeWeight(1);
    fillColor(COLOR_OP);
    rectRounded(pos.x, pos.y, size.x, size.y, size.y / 2);
    CodeBlock_draw(arg, vec2_add(pos, argpos(size, OP_LEN(block), argSize)));
    textSize(TEXT_SIZE);
    fillColor(COLOR_TEXT);
    text(OP_STR(block), pos.x + PADD + TEXT_W(0.5f), pos.y + TEXT_YOFFSET(size.y));
    return size;
}

static char *totext(CodeBlock *block) {
    char *arg = CodeBlock_totext(block->blocks->items[0]);
    size_t arglen = strlen(arg);
    size_t len = OP_CODELEN(block) + arglen + 2;
    char *txt = malloc((len+1) * sizeof(char));
    char *t = txt;

    *(t++) = '(';
    strncpy(t, OP_CODESTR(block), OP_CODELEN(block));
    t += OP_CODELEN(block);
    strncpy(t, arg, arglen);
    t += arglen;
    *(t++) = ')';
    *t = 0;

    free(arg);

    return txt;
}

void cb_unaryop_new(CodeBlock *block) {
    block->typeData.isDir = 0;
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

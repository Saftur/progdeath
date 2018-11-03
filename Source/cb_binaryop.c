/**
 * @file cb_binaryop.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/31/18
 * 
 */
#include "cb_binaryop.h"

#include "cbtypedata.h"

static struct {
    const char *str;
    size_t len;
} ops[] = {
    {"and", 3},
    {"or", 2},
    {"+", 1},
    {"-", 1},
    {"*", 1},
    {"/", 1},
    {"%", 1},
};

#define OP(block) ops[*(unsigned*)block->data]

static void init(CodeBlock *block) {
    List_push_back(block->blocks, CodeBlock_new(CB_NUM, NULL, 0));
    List_push_back(block->blocks, CodeBlock_new(CB_NUM, NULL, 0));
}

static vec2_t getsize(CodeBlock *block) {
    CodeBlock *arg1 = block->blocks->items[0];
    CodeBlock *arg2 = block->blocks->items[1];
    vec2_t arg1Size = CodeBlock_getsize(arg1);
    vec2_t arg2Size = CodeBlock_getsize(arg2);
    float height = max(INNER_HEIGHT, max(arg1Size.y, arg2Size.y) + PADD * 2);
    return (vec2_t){ arg1Size.x + arg2Size.x + TEXT_W(OP(block).len+1) + ARG_RND_BOX_PADD(height) * 2, height };
}

static vec2_t arg1pos(vec2_t size, vec2_t arg1Size) {
    return (vec2_t){ ARG_RND_BOX_PADD(size.y), size.y / 2 - arg1Size.y / 2 };
}

static vec2_t arg2pos(vec2_t size, int opLen, vec2_t arg1Size, vec2_t arg2Size) {
    return (vec2_t){ ARG_RND_BOX_PADD(size.y) + arg1Size.x + TEXT_W(opLen+1), size.y / 2 - arg2Size.y / 2 };
}

static void update(CodeBlock *block, vec2_t pos) {
    vec2_t size = getsize(block);
    CodeBlock *arg1 = block->blocks->items[0];
    CodeBlock *arg2 = block->blocks->items[1];
    vec2_t arg1Size = CodeBlock_getsize(arg1);
    vec2_t arg2Size = CodeBlock_getsize(arg2);

    CodeBlock_update(arg1, vec2_add(pos, arg1pos(size, arg1Size)));
    CodeBlock_update(arg2, vec2_add(pos, arg2pos(size, OP(block).len, arg1Size, arg2Size)));
}

static CBGrabResult grab(CodeBlock *block, vec2_t p, int test) {
    vec2_t size = getsize(block);
    CodeBlock *arg1 = block->blocks->items[0];
    CodeBlock *arg2 = block->blocks->items[1];
    vec2_t arg1Size = CodeBlock_getsize(arg1);
    vec2_t arg2Size = CodeBlock_getsize(arg2);
    vec2_t arg1Pos = arg1pos(size, arg1Size);
    CBGrabResult res;
    res = CodeBlock_grab_test(arg1, vec2_sub(p, arg1Pos), test);
    if (res.either) {
        if (res.parent && !test)
            block->blocks->items[0] = empty_new();
        return GRABRES_CHILD;
    }
    vec2_t arg2Pos = arg2pos(size, OP(block).len, arg1Size, arg2Size);
    res = CodeBlock_grab_test(arg2, vec2_sub(p, arg2Pos), test);
    if (res.either) {
        if (res.parent && !test)
            block->blocks->items[1] = empty_new();
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
    CodeBlock *arg1 = block->blocks->items[0];
    CodeBlock *arg2 = block->blocks->items[1];
    vec2_t arg1Size = CodeBlock_getsize(arg1);
    vec2_t arg2Size = CodeBlock_getsize(arg2);
    vec2_t subP;
    vec2_t varPos = arg1pos(size, arg1Size);
    subP = vec2_sub(p, varPos);
    if (CodeBlock_drop(arg1, dropped, subP)) {
        return 1;
    } else if (is_arg(dropped) && vec2_in_range(subP, vec2_zero(), arg1Size)) {
        CodeBlock_delete(arg1);
        block->blocks->items[0] = dropped;
        return 1;
    }
    vec2_t valPos = arg2pos(size, OP(block).len, arg1Size, arg2Size);
    subP = vec2_sub(p, valPos);
    if (CodeBlock_drop(arg2, dropped, subP)) {
        return 1;
    } else if (is_arg(dropped) && vec2_in_range(subP, vec2_zero(), arg2Size)) {
        CodeBlock_delete(arg2);
        block->blocks->items[1] = dropped;
        return 1;
    }
    return 0;
}

static vec2_t draw(CodeBlock *block, vec2_t pos) {
    vec2_t size = getsize(block);
    CodeBlock *arg1 = block->blocks->items[0];
    CodeBlock *arg2 = block->blocks->items[1];
    vec2_t arg1Size = CodeBlock_getsize(arg1);
    vec2_t arg2Size = CodeBlock_getsize(arg2);
    noStroke();
    fillColor(COLOR_OP);
    rectRounded(pos.x, pos.y, size.x, size.y, size.y / 2);
    CodeBlock_draw(arg1, vec2_add(pos, arg1pos(size, arg1Size)));
    textSize(TEXT_SIZE);
    fillColor(COLOR_TEXT);
    text(OP(block).str, pos.x + ARG_RND_BOX_PADD(size.y) + arg1Size.x + TEXT_W(0.5f), pos.y + TEXT_YOFFSET(size.y));
    CodeBlock_draw(arg2, vec2_add(pos, arg2pos(size, OP(block).len, arg1Size, arg2Size)));
    return size;
}

static char *totext(CodeBlock *block) {
    char *arg1 = CodeBlock_totext(block->blocks->items[0]);
    char *arg2 = CodeBlock_totext(block->blocks->items[1]);
    size_t arg1len = strlen(arg1);
    size_t arg2len = strlen(arg2);
    size_t len = arg1len + OP(block).len + 4 + arg2len;
    char *txt = malloc((len+1) * sizeof(char));
    char *t = txt;

    *(t++) = '(';
    strncpy(t, arg1, arg1len);
    t += arg1len;
    *(t++) = ' ';
    strncpy(t, OP(block).str, OP(block).len);
    t += OP(block).len;
    *(t++) = ' ';
    strncpy(t, arg2, arg2len);
    t += arg2len;
    *(t++) = ')';
    *t = 0;

    free(arg1);
    free(arg2);

    return txt;
}

void cb_binaryop_new(CodeBlock *block) {
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

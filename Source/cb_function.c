/**
 * @file cb_function.c
 * @author Arthur Bouvier (a.bouvier)
 * 
 */
#include "cb_function.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Engine/util.h>
#include <Engine/focus.h>
#include <Engine/list.h>

#include "cbtypedata.h"
#include "codeblockboard.h"

typedef struct CBStrFocus {
    CodeBlock *block;
    vec2_t pos;
} CBStrFocus;

#define FOCUS(block) ((CBStrFocus*)((List*)block->data)->items[0])
#define STR(block)   ((char*)((List*)block->data)->items[1])

#define STR_OFFSET (TEXT_ARG_RND_PADD * 2)
#define STR_SIZE(block) block->dataSize

#define STR_LEN(block) max(4, strlen(STR(block)))
//#define STR_W(block) TEXT_W(max(4, strlen(STR(block))))

static void addchar(CodeBlock *block, char c) {
    size_t len = strlen(STR(block));
    if ((len + 1) * sizeof(char) >= STR_SIZE(block)) {
        void *old = STR(block);
        size_t oldSize = block->dataSize;
        block->dataSize *= 2;
        STR(block) = malloc(block->dataSize);
        memcpy(STR(block), old, oldSize);
        free(old);
    }
    STR(block)[len] = c;
    STR(block)[len+1] = 0;
}

static void backspace(CodeBlock *block) {
    size_t len = strlen(STR(block));
    if (len == 0) return;
    STR(block)[len-1] = 0;
}

static vec2_t getsize(CodeBlock *block) {
    CodeBlock *var = block->blocks->items[0];
    vec2_t varSize = CodeBlock_getsize(var);
    return (vec2_t){ STR_OFFSET + TEXT_W(max(4, strlen(STR(block))) + 3.5) + varSize.x + TEXT_ARG_RND_PADD, max(INNER_HEIGHT, TEXT_H() + PADD * 2) };
}

static vec2_t varpos(vec2_t size, vec2_t varSize, float textSize) {
    return (vec2_t){ STR_OFFSET + textSize + TEXT_W(2), size.y / 2 - varSize.y / 2 };
}

static int focusCheck(CBStrFocus *focus) {
    vec2_t mPos = CB_getMousePos();
    vec2_t size = getsize(focus->block);
    return vec2_in_range(vec2_sub(mPos, focus->pos), (vec2_t){ STR_OFFSET, PADD }, (vec2_t){ STR_OFFSET + TEXT_W(STR_LEN(focus->block)), size.y - PADD });
}

static void focusCallback(Key key, KeyState state, CBStrFocus *focus) {
    int shift = keyIsDown(KEY_LEFT_SHIFT) || keyIsDown(KEY_RIGHT_SHIFT);
    if ((state != KeyPressed && state != KeyRepeat) || keyIsDown(KEY_LEFT_CONTROL) || keyIsDown(KEY_RIGHT_CONTRO))
        return;
    if (KEY_A <= key && key <= KEY_Z)
        addchar(focus->block, key - KEY_A + (shift ? 'A' : 'a'));
    if (!shift && KEY_0 <= key && key <= KEY_9 && strlen(STR(focus->block)) > 0)
        addchar(focus->block, key - KEY_0 + '0');
    if (shift && key == KEY_MINUS)
        addchar(focus->block, '_');

    if (key == KEY_SPACE)
        addchar(focus->block, ' ');
    if (key == KEY_BACKSPACE)
        backspace(focus->block);
}

static void init(CodeBlock *block) {
    block->data = List_new(2, NULL, free);
    List_push_back(block->data, malloc(sizeof(CBStrFocus)));
    FOCUS(block)->block = block;
    FOCUS(block)->pos = (vec2_t){ 0, 0 };
    block->dataSize = 20 * sizeof(char);
    List_push_back(block->data, malloc(block->dataSize));
    STR(block)[0] = 0;
    addFocusObject(focusCheck, focusCallback, FOCUS(block));

    List_push_back(block->blocks, CodeBlock_new(CB_NUM, NULL, 0));
}

static void clone(CodeBlock *block, CodeBlock *from) {
    block->data = List_new(2, NULL, free);
    List_push_back(block->data, malloc(sizeof(CBStrFocus)));
    FOCUS(block)->block = block;
    FOCUS(block)->pos = (vec2_t){ 0, 0 };
    block->dataSize = from->dataSize;
    List_push_back(block->data, malloc(block->dataSize));
    strcpy(STR(block), STR(from));
    addFocusObject(focusCheck, focusCallback, FOCUS(block));

    List_push_back(block->blocks, CodeBlock_clone(block->blocks->items[0]));
}

static void delete(CodeBlock *block) {
    removeFocusObject(FOCUS(block));
    List_delete(block->data);
}

static void update(CodeBlock *block, vec2_t pos) {
    FOCUS(block)->pos = pos;

    vec2_t size = getsize(block);
    CodeBlock *var = block->blocks->items[0];
    vec2_t varSize = CodeBlock_getsize(var);

    CodeBlock_update(var, vec2_add(pos, varpos(size, varSize, TEXT_W(STR_LEN(block)))));
}

static CBGrabResult grab(CodeBlock *block, vec2_t p, int test) {
    vec2_t size = getsize(block);
    float halfHeight = size.y / 2;
    int inRect = vec2_in_range(p, (vec2_t){ halfHeight, 0 }, vec2_sub(size, (vec2_t){ halfHeight, 0 }));
    vec2_t cCenter1 = (vec2_t){ halfHeight, halfHeight }, cCenter2 = (vec2_t){ size.x - halfHeight, halfHeight };
    int inBlock = inRect || vec2_length(vec2_sub(p, cCenter1)) < halfHeight || vec2_length(vec2_sub(p, cCenter2)) < halfHeight;
    int inStr = vec2_in_range(p, (vec2_t){ STR_OFFSET, PADD }, (vec2_t){ STR_OFFSET + TEXT_W(STR_LEN(block)), size.y - PADD });
    if (inStr)
        return GRABRES_CHILD;
    CodeBlock *var = block->blocks->items[0];
    vec2_t varSize = CodeBlock_getsize(var);
    vec2_t varPos = varpos(size, varSize, TEXT_W(STR_LEN(block)));
    CBGrabResult res;
    res = CodeBlock_grab_test(var, vec2_sub(p, varPos), test);
    if (res.either) {
        if (res.parent && !test)
            block->blocks->items[0] = CodeBlock_new(CB_NUM, NULL, 0);
        return GRABRES_CHILD;
    }
    if (inBlock) {
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
    vec2_t varPos = varpos(size, varSize, TEXT_W(STR_LEN(block)));
    subP = vec2_sub(p, varPos);
    if (CodeBlock_drop(var, dropped, subP)) {
        return 1;
    } else if (is_arg(dropped) && vec2_in_range(subP, vec2_zero(), varSize)) {
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
    //noStroke();
    strokeColor(COLOR_DIR_STROKE);
    strokeWeight(1);
    fillColor(COLOR_DIRECTIVE);
    rectRounded(pos.x, pos.y, size.x, size.y, size.y / 2);
    fillColor(COLOR_EMPTY);
    rectRounded(pos.x + STR_OFFSET, pos.y + PADD, TEXT_W(STR_LEN(block))/*size.x - STR_OFFSET - TEXT_ARG_RND_PADD - TEXT_W(2) - varSize.x*/, size.y - PADD * 2, RECT_RADIUS);
    textSize(TEXT_SIZE);
    fillColor(COLOR_TEXT);
    text(STR(block), pos.x + STR_OFFSET, pos.y + TEXT_YOFFSET(size.y));
    text("(", pos.x + STR_OFFSET + TEXT_W(STR_LEN(block) + 0.5), pos.y + TEXT_YOFFSET(size.y));
    CodeBlock_draw(var, vec2_add(pos, varpos(size, varSize, TEXT_W(STR_LEN(block)))));
    text(")", pos.x + STR_OFFSET + TEXT_W(STR_LEN(block) + 2.5) + varSize.x, pos.y + TEXT_YOFFSET(size.y));
    return size;
}

static char *totext(CodeBlock *block) {
    char *str = STR(block);
    char *var = CodeBlock_totext(block->blocks->items[0]);
    size_t varlen = strlen(var);
    size_t len = strlen(str) + varlen + 2;
    char *txt = malloc((len+1) * sizeof(char));
    sprintf(txt, "%s(%s)", str, var);

    free(var);

    return txt;
}

void cb_function_new(CodeBlock *block) {
    block->typeData.isDir = 1;
    block->typeData.isArg = 1;
    block->typeData.isLValue = 0;
    block->typeData.ignData = 1;
    block->typeData.init = init;
    block->typeData.clone = clone;
    block->typeData.delete = delete;
    block->typeData.getsize = getsize;
    block->typeData.update = update;
    block->typeData.grab = grab;
    block->typeData.drop = drop;
    block->typeData.draw = draw;
    block->typeData.totext = totext;
}

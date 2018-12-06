/**
 * @file cb_num.c
 * @author Arthur Bouvier (a.bouvier)
 * @course GAM100F18
 * Copyright © 2018 DigiPen (USA) Corporation
 */
#include "cb_num.h"

#include <stdlib.h>
#include <string.h>

#include <Engine/util.h>
#include <Engine/focus.h>
#include <Engine/list.h>

#include "cbtypedata.h"
#include "codeblockboard.h"

typedef struct CBNumFocus {
    CodeBlock *block;
    vec2_t pos;
} CBNumFocus;

/*#define FOCUS(block)  (*(CBNumFocus**)block->data)
#define PERIOD(block) (*((int*)block->data+1))
#define STR(block)    ((char*)block->data+2)*/
#define FOCUS(block)     ((CBNumFocus*)((List*)block->data)->items[0])
#define PERIODPTR(block) ((int*)((List*)block->data)->items[1])
#define PERIOD(block)    (*PERIODPTR(block))
#define STR(block)       ((char*)((List*)block->data)->items[2])

#define STR_OFFSET (TEXT_ARG_RND_PADD + TEXT_W(1))
#define STR_SIZE(block) block->dataSize - sizeof(void*)

static void addchar(CodeBlock *block, char c) {
    size_t len = strlen(STR(block));
    if (c == '-' && len != 0)
        return;
    if (c == '.') {
        if (PERIOD(block))
            return;
        else PERIOD(block) = 1;
    }
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
    if (STR(block)[len-1] == '.')
        PERIOD(block) = 0;
    STR(block)[len-1] = 0;
}

static vec2_t getsize(CodeBlock *block) {
    float height = max(INNER_HEIGHT, TEXT_H());
    return (vec2_t){ TEXT_ARG_RND_PADD * 2 + max(TEXT_W(3), TEXT_W(strlen(STR(block)))), height };
}

static int focusCheck(CBNumFocus *focus) {
    vec2_t mPos = CB_getMousePos();
    vec2_t size = getsize(focus->block);
    float halfHeight = size.y / 2;
    vec2_t p = vec2_sub(mPos, focus->pos);
    int inRect = vec2_in_range(p, (vec2_t){ halfHeight, 0 }, vec2_sub(size, (vec2_t){ halfHeight, 0 }));
    vec2_t cCenter1 = (vec2_t){ halfHeight, halfHeight }, cCenter2 = (vec2_t){ size.x - halfHeight, halfHeight };
    int inBlock = inRect || vec2_length(vec2_sub(p, cCenter1)) < halfHeight || vec2_length(vec2_sub(p, cCenter2)) < halfHeight;
    return inBlock;
}

static void focusCallback(Key key, KeyState state, CBNumFocus *focus) {
    if ((state != KeyPressed && state != KeyRepeat) || keyIsDown(KEY_LEFT_SHIFT) || keyIsDown(KEY_RIGHT_SHIFT) || keyIsDown(KEY_LEFT_CONTROL) || keyIsDown(KEY_RIGHT_CONTRO))
        return;
    if (KEY_0 <= key && key <= KEY_9)
        addchar(focus->block, key - KEY_0 + '0');
    if (key == KEY_MINUS)
        addchar(focus->block, '-');
    if (key == KEY_PERIOD)
        addchar(focus->block, '.');
    if (key == KEY_BACKSPACE)
        backspace(focus->block);
}

static void init(CodeBlock *block) {
    block->data = List_new(3, NULL, free);
    List_push_back(block->data, malloc(sizeof(CBNumFocus)));
    FOCUS(block)->block = block;
    FOCUS(block)->pos = (vec2_t){ 0, 0 };
    List_push_back(block->data, malloc(sizeof(int)));
    PERIOD(block) = 0;
    block->dataSize = 20 * sizeof(char);
    List_push_back(block->data, malloc(block->dataSize));
    STR(block)[0] = 0;
    addFocusObject(focusCheck, focusCallback, FOCUS(block));
}

static void clone(CodeBlock *block, CodeBlock *from) {
    block->data = List_new(3, NULL, free);
    List_push_back(block->data, malloc(sizeof(CBNumFocus)));
    FOCUS(block)->block = block;
    FOCUS(block)->pos = (vec2_t){ 0, 0 };
    List_push_back(block->data, malloc(sizeof(int)));
    PERIOD(block) = PERIOD(from);
    block->dataSize = from->dataSize;
    List_push_back(block->data, malloc(block->dataSize));
    strcpy(STR(block), STR(from));
    addFocusObject(focusCheck, focusCallback, FOCUS(block));
}

static void delete(CodeBlock *block) {
    removeFocusObject(FOCUS(block));
    List_delete(block->data);
}

static void update(CodeBlock *block, vec2_t pos) {
    FOCUS(block)->pos = pos;
}

static CBGrabResult grab(CodeBlock *block, vec2_t p, int test) {
    vec2_t size = getsize(block);
    float halfHeight = size.y / 2;
    int inRect = vec2_in_range(p, (vec2_t){ halfHeight, 0 }, vec2_sub(size, (vec2_t){ halfHeight, 0 }));
    vec2_t cCenter1 = (vec2_t){ halfHeight, halfHeight }, cCenter2 = (vec2_t){ size.x - halfHeight, halfHeight };
    int inBlock = inRect || vec2_length(vec2_sub(p, cCenter1)) < halfHeight || vec2_length(vec2_sub(p, cCenter2)) < halfHeight;
    if (inBlock) {
        return GRABRES_CHILD;
    }
    return GRABRES_NEITHER;
}

static vec2_t draw(CodeBlock *block, vec2_t pos) {
    vec2_t size = getsize(block);
    noStroke();
    fillColor(COLOR_EMPTY);
    rectRounded(pos.x, pos.y, size.x, size.y, size.y / 2);
    textSize(TEXT_SIZE);
    fillColor(COLOR_TEXT);
    text(STR(block), pos.x + TEXT_ARG_RND_PADD, pos.y + TEXT_YOFFSET(size.y));
    return size;
}

static char *totext(CodeBlock *block) {
    char *str = STR(block);
    char *txt = malloc((strlen(str)+1) * sizeof(char));
    char *t = txt;
    for (char *c = str; *c; c++) {
        *(t++) = *c;
    }
    *t = 0;
    return txt;
}

void cb_num_new(CodeBlock *block) {
    block->typeData.isDir = 0;
    block->typeData.isArg = 1;
    block->typeData.ignData = 1;
    block->typeData.init = init;
    block->typeData.clone = clone;
    block->typeData.delete = delete;
    block->typeData.getsize = getsize;
    block->typeData.update = update;
    block->typeData.grab = grab;
    block->typeData.drop = NULL;
    block->typeData.draw = draw;
    block->typeData.totext = totext;
}

/**
 * @file cb_str.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/31/18
 * 
 */
#include "cb_str.h"

#include <stdlib.h>
#include <string.h>

#include <Engine/util.h>
#include <Engine/focus.h>

#include "cbtypedata.h"
#include "codeblockboard.h"

typedef struct CBStrFocus {
    CodeBlock *block;
    vec2_t pos;
} CBStrFocus;

#define STROFFSET(height) height * 0.60

#define FOCUS(block) (*(CBStrFocus**)block->data)
#define STR(block) ((char*)block->data+sizeof(void*))

#define STR_SIZE(block) block->dataSize - sizeof(void*)

static void addchar(CodeBlock *block, char c) {
    size_t len = strlen(STR(block));
    if ((len + 1) * sizeof(char) >= STR_SIZE(block)) {
        void *old = block->data;
        size_t oldSize = block->dataSize;
        block->dataSize *= 2;
        block->data = malloc(block->dataSize);
        memcpy(block->data, old, oldSize);
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

static char shiftnumchars[] ={')','!','@','#','$','%','^','&','*','('};

static vec2_t getsize(CodeBlock *block) {
    float height = max(INNER_HEIGHT, TEXT_H() + PADD * 2);
    return (vec2_t){ max(100, TEXT_W(strlen(STR(block))) + STROFFSET(height) * 2), height };
}

static int focusCheck(CBStrFocus *focus) {
    vec2_t mPos = CB_getMousePos();
    vec2_t size = getsize(focus->block);
    return vec2_in_range(vec2_sub(mPos, focus->pos), (vec2_t){ STROFFSET(size.y), PADD }, (vec2_t){ size.x - STROFFSET(size.y), size.y - PADD });
}

static void focusCallback(Key key, KeyState state, CBStrFocus *focus) {
    int shift = keyIsDown(KEY_LEFT_SHIFT) || keyIsDown(KEY_RIGHT_SHIFT);
    if ((state != KeyPressed && state != KeyRepeat) || keyIsDown(KEY_LEFT_CONTROL) || keyIsDown(KEY_RIGHT_CONTRO))
        return;
    if (KEY_A <= key && key <= KEY_Z)
        addchar(focus->block, key - KEY_A + (shift ? 'A' : 'a'));
    if (KEY_0 <= key && key <= KEY_9)
        addchar(focus->block, (shift ? shiftnumchars[key - KEY_0] : key - KEY_0 + '0'));
    if (key == KEY_GRAVE_ACCENT)
        addchar(focus->block, (shift ? '~' : '`'));
    if (key == KEY_MINUS)
        addchar(focus->block, (shift ? '_' : '-'));
    if (key == KEY_EQUAL)
        addchar(focus->block, (shift ? '+' : '='));
    if (key == KEY_LEFT_BRACKET)
        addchar(focus->block, (shift ? '{' : '['));
    if (key == KEY_RIGHT_BRACKE)
        addchar(focus->block, (shift ? '}' : ']'));
    if (key == KEY_BACKSLASH)
        addchar(focus->block, (shift ? '|' : '\\'));
    if (key == KEY_SEMICOLON)
        addchar(focus->block, (shift ? ':' : ';'));
    if (key == KEY_APOSTROPHE)
        addchar(focus->block, (shift ? '"' : '\''));
    if (key == KEY_COMMA)
        addchar(focus->block, (shift ? '<' : ','));
    if (key == KEY_PERIOD)
        addchar(focus->block, (shift ? '>' : '.'));
    if (key == KEY_SLASH)
        addchar(focus->block, (shift ? '?' : '/'));

    if (key == KEY_SPACE)
        addchar(focus->block, ' ');
    if (key == KEY_BACKSPACE)
        backspace(focus->block);
    if (key == KEY_ENTER) {
        addchar(focus->block, '\\');
        addchar(focus->block, 'n');
    }
}

static void init(CodeBlock *block) {
    block->dataSize = sizeof(void*) + 20;
    block->data = malloc(block->dataSize*sizeof(char));
    FOCUS(block) = malloc(sizeof(CBStrFocus));
    FOCUS(block)->block = block;
    FOCUS(block)->pos = (vec2_t){ 0, 0 };
    STR(block)[0] = 0;
    addFocusObject(focusCheck, focusCallback, FOCUS(block));
}

static void clone(CodeBlock *block) {
    FOCUS(block) = malloc(sizeof(CBStrFocus));
    FOCUS(block)->block = block;
    FOCUS(block)->pos = (vec2_t){ 0, 0 };
    addFocusObject(focusCheck, focusCallback, FOCUS(block));
}

static void delete(CodeBlock *block) {
    removeFocusObject(FOCUS(block));
    free(FOCUS(block));
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
    int inStr = vec2_in_range(p, (vec2_t){ STROFFSET(size.y), PADD }, (vec2_t){ size.x - STROFFSET(size.y), size.y - PADD });
    if (inStr)
        return GRABRES_CHILD;
    if (inBlock) {
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
    fillColor(COLOR_EMPTY);
    rectRounded(pos.x + STROFFSET(size.y), pos.y + PADD, size.x - STROFFSET(size.y) * 2, size.y - PADD * 2, RECT_RADIUS);
    textSize(TEXT_SIZE);
    fillColor(COLOR_TEXT);
    text("\"", pos.x + ARG_RND_W, pos.y + TEXT_YOFFSET(size.y));
    text(STR(block), pos.x + STROFFSET(size.y), pos.y + TEXT_YOFFSET(size.y));
    text("\"", pos.x + size.x - TEXT_W(1) - ARG_RND_W, pos.y + TEXT_YOFFSET(size.y));
    return size;
}

static char *totext(CodeBlock *block) {
    char *str = STR(block);
    size_t len = strlen(str);
    for (char *c = str; *c; c++) {
        if (*c == '\\' || *c == '"')
            len++;
    }
    char *txt = malloc((len+3) * sizeof(char));
    char *t = txt;
    *(t++) = '"';
    for (char *c = str; *c; c++) {
        if (*c == '\\' || *c == '"')
            *(t++) = '\\';
        *(t++) = *c;
    }
    *(t++) = '"';
    *t = 0;
    return txt;
}

void cb_str_new(CodeBlock *block) {
    block->typeData.isDir = 0;
    block->typeData.isArg = 1;
    block->typeData.numArgs = 0;
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

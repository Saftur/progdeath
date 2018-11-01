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

#include "cbtypedata.h"
#include "codeblockboard.h"

#define STROFFSET(height) height * 0.60

#define FOCUS(block) (*((int*)block->data))
#define STR(block) ((char*)block->data+sizeof(int))

#define STR_SIZE(block) block->dataSize - sizeof(int)

static void init(CodeBlock *block) {
    block->dataSize = sizeof(int) + 20;
    block->data = malloc(sizeof(int) + block->dataSize*sizeof(char));
    FOCUS(block) = 0;
    STR(block)[0] = 0;
}

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

static vec2_t getsize(CodeBlock *block) {
    float height = max(INNER_HEIGHT, TEXT_H() + PADD * 2);
    return (vec2_t){ max(100, TEXT_W(strlen(STR(block))) + STROFFSET(height) * 2), height };
}

static char shiftnumchars[] ={')','!','@','#','$','%','^','&','*','('};

static void update(CodeBlock *block, vec2_t pos) {
    if (mousePressed(MOUSE_BUTTON_1)) {
        vec2_t mPos = CB_getMousePos();
        vec2_t size = getsize(block);
        FOCUS(block) = vec2_in_range(vec2_sub(mPos, pos), (vec2_t){ STROFFSET(size.y), PADD }, (vec2_t){ size.x - STROFFSET(size.y), size.y - PADD });
    }
    if (!FOCUS(block) || keyPressed(KEY_LEFT_CONTROL) || keyPressed(KEY_RIGHT_CONTRO))
        return;
    int shift = keyIsDown(KEY_LEFT_SHIFT) || keyIsDown(KEY_RIGHT_SHIFT);
    for (int k = KEY_A; k <= KEY_Z; k++) {
        if (keyPressed(k))
            addchar(block, k - KEY_A + (shift ? 'A' : 'a'));
    }
    for (int k = KEY_0; k <= KEY_9; k++) {
        if (keyPressed(k))
            addchar(block, (shift ? shiftnumchars[k - KEY_0] : k - KEY_0 + '0'));
    }
    if (keyPressed(KEY_GRAVE_ACCENT))
        addchar(block, (shift ? '~' : '`'));
    if (keyPressed(KEY_MINUS))
        addchar(block, (shift ? '_' : '-'));
    if (keyPressed(KEY_EQUAL))
        addchar(block, (shift ? '+' : '='));
    if (keyPressed(KEY_LEFT_BRACKET))
        addchar(block, (shift ? '{' : '['));
    if (keyPressed(KEY_RIGHT_BRACKE))
        addchar(block, (shift ? '}' : ']'));
    if (keyPressed(KEY_BACKSLASH))
        addchar(block, (shift ? '|' : '\\'));
    if (keyPressed(KEY_SEMICOLON))
        addchar(block, (shift ? ':' : ';'));
    if (keyPressed(KEY_APOSTROPHE))
        addchar(block, (shift ? '"' : '\''));
    if (keyPressed(KEY_COMMA))
        addchar(block, (shift ? '<' : ','));
    if (keyPressed(KEY_PERIOD))
        addchar(block, (shift ? '>' : '.'));
    if (keyPressed(KEY_SLASH))
        addchar(block, (shift ? '?' : '/'));

    if (keyPressed(KEY_SPACE))
        addchar(block, ' ');
    if (keyPressed(KEY_BACKSPACE))
        backspace(block);
    if (keyPressed(KEY_ENTER)) {
        addchar(block, '\\');
        addchar(block, 'n');
    }
}

static CBGrabResult grab(CodeBlock *block, vec2_t p) {
    vec2_t size = getsize(block);
    float halfHeight = size.y / 2;
    int inRect = vec2_in_range(p, (vec2_t){ halfHeight, 0 }, vec2_sub(size, (vec2_t){ halfHeight, 0 }));
    vec2_t cCenter1 = (vec2_t){ halfHeight, halfHeight }, cCenter2 = (vec2_t){ size.x - halfHeight, halfHeight };
    int inBlock = inRect || vec2_length(vec2_sub(p, cCenter1)) < halfHeight || vec2_length(vec2_sub(p, cCenter2)) < halfHeight;
    int inStr = vec2_in_range(p, (vec2_t){ STROFFSET(size.y), PADD }, (vec2_t){ size.x - STROFFSET(size.y), size.y - PADD });
    if (inStr)
        return GRABRES_CHILD;
    if (inBlock) {
        setGrabbed(block, p);
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
    block->typeData.getsize = getsize;
    block->typeData.update = update;
    block->typeData.grab = grab;
    block->typeData.drop = NULL;
    block->typeData.draw = draw;
    block->typeData.totext = totext;
}

/**
 * @file codeblocktypes.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/31/18
 * @brief CodeBlock types implementation
 */
#include "codeblocktypes.h"

#include <string.h>
#include <Engine/transform.h>
#include <Engine/util.h>

#include "codeblock.h"
#include "editorscreen.h"

#define PADD 2

#define HEIGHT 30
#define INNER_HEIGHT (HEIGHT - PADD * 2)

#define COLOR_TEXT      (PColor){  0,   0,   0, 255}
#define COLOR_EMPTY     (PColor){255, 255, 255, 255}
#define COLOR_CONTROL   (PColor){255, 255,   0, 255}
#define COLOR_DIRECTIVE (PColor){ 47,  47, 255, 255}
#define COLOR_ARG       (PColor){255, 191, 127, 255}

#define TEXT_SIZE (HEIGHT - PADD * 2)
#define TEXT_W(num) ((TEXT_SIZE / 2) * num)
#define TEXT_H() TEXT_SIZE
#define TEXT_YOFFSET (TEXT_SIZE * 2 / 6)

#define IF_BORDER (HEIGHT / 5)
#define IF_XSPACING (HEIGHT / 15)
#define IF_YSPACING (HEIGHT / 5)
#define IF_BD_XSP (IF_BORDER + IF_XSPACING)
#define IF_BD_YSP (IF_BORDER + IF_YSPACING)

#define TABS_AS_SPACES 1

#if TABS_AS_SPACES
#define TABSIZE 4
#else
#define TABSIZE 1
#endif

int tablevel = 0;

void incTab() {
    tablevel++;
}

void decTab() {
    tablevel--;
}

#define CURR_TABSIZE (TABSIZE * tablevel)

static void addTabs(char *txt) {
    for (int i = 0; i < tablevel; i++) {
#if TABS_AS_SPACES
        strncpy(txt, "    ", 4);
        txt += 4;
#else
        strncpy(txt, "\t", 1);
        txt += 1;
#endif
    }
}


static vec2_t empty_getsize(CodeBlock *block) {
    return (vec2_t){ 100, INNER_HEIGHT };
}

static int empty_grab(CodeBlock *block, vec2_t p) {
    /*if (vec2_in_range(p, vec2_zero(), empty_getsize(block))) {

        return 1;
    }
    return 0;*/
    return 0;
}

static vec2_t empty_draw(CodeBlock *block, vec2_t pos) {
    vec2_t size = empty_getsize(block);
    noStroke();
    fillColor(COLOR_EMPTY);
    rect(pos.x, pos.y, size.x, size.y);
    return size;
}

static char *empty_text(CodeBlock *block) {
    char *txt = malloc(1 * sizeof(char));
    txt[0] = 0;
    return txt;
}


static vec2_t setvar_getsize(CodeBlock *block) {
    CodeBlock *var = (block->blocks->size >= 1) ? block->blocks->items[0] : NULL;
    CodeBlock *val = (block->blocks->size >= 2) ? block->blocks->items[1] : NULL;
    vec2_t varSize = var ? CodeBlock_getsize(var) : (vec2_t){ 0, 0 };
    vec2_t valSize = val ? CodeBlock_getsize(val) : (vec2_t){ 0, 0 };
    return (vec2_t){ varSize.x + valSize.x + TEXT_W(2) + PADD * 2, max(HEIGHT, max(varSize.y, valSize.y) + PADD * 2) };
}

static vec2_t setvar_varpos(CodeBlock *block, vec2_t size, vec2_t varSize) {
    return (vec2_t){ PADD, size.y / 2 - varSize.y / 2 };
}

static vec2_t setvar_valpos(CodeBlock *block, vec2_t size, vec2_t varSize, vec2_t valSize) {
    return (vec2_t){PADD + varSize.x + TEXT_W(2), size.y / 2 - valSize.y / 2};
}

static int setvar_grab(CodeBlock *block, vec2_t p) {
    vec2_t size = setvar_getsize(block);
    CodeBlock *var = (block->blocks->size >= 1) ? block->blocks->items[0] : NULL;
    CodeBlock *val = (block->blocks->size >= 2) ? block->blocks->items[1] : NULL;
    vec2_t varSize = var ? CodeBlock_getsize(var) : (vec2_t){ 0, 0 };
    vec2_t valSize = val ? CodeBlock_getsize(val) : (vec2_t){ 0, 0 };
    if (var) {
        vec2_t varPos = setvar_varpos(block, size, varSize);
        if (CodeBlock_grab(var, vec2_sub(p, varPos))) {
            block->blocks->items[0] = NULL;
            return 0;
        }
    }
    if (val) {
        vec2_t valPos = setvar_valpos(block, size, varSize, valSize);
        if (CodeBlock_grab(val, vec2_sub(p, valPos))) {
            block->blocks->items[1] = NULL;
            return 0;
        }
    }
    if (vec2_in_range(p, vec2_zero(), setvar_getsize(block))) {
        setGrabbed(block);
        return 1;
    }
    return 0;
}

static vec2_t setvar_draw(CodeBlock *block, vec2_t pos) {
    CodeBlock *var = (block->blocks->size >= 1) ? block->blocks->items[0] : NULL;
    CodeBlock *val = (block->blocks->size >= 2) ? block->blocks->items[1] : NULL;
    vec2_t varSize = var ? CodeBlock_getsize(var) : (vec2_t){ 0, 0 };
    vec2_t valSize = val ? CodeBlock_getsize(val) : (vec2_t){ 0, 0 };
    vec2_t size = setvar_getsize(block);
    noStroke();
    fillColor(COLOR_DIRECTIVE);
    rect(pos.x, pos.y, size.x, size.y);
    if (var)
        CodeBlock_draw(var, vec2_add(pos, setvar_varpos(block, size, varSize)));
    textSize(TEXT_SIZE);
    fillColor(COLOR_TEXT);
    text("=", pos.x + varSize.x + PADD + TEXT_W(1/2), pos.y + size.y / 2 + TEXT_YOFFSET);
    if (val)
        CodeBlock_draw(val, vec2_add(pos, setvar_valpos(block, size, varSize, valSize)));
    return size;
}

static char *setvar_text(CodeBlock *block) {
    if (block->blocks->size < 2)
        return NULL;

    char *var = CodeBlock_text(block->blocks->items[0]);
    char *val = CodeBlock_text(block->blocks->items[1]);
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


static vec2_t var_getsize(CodeBlock *block) {
    return (vec2_t){ TEXT_W(strlen(block->data)) + PADD * 2, max(INNER_HEIGHT, TEXT_H() + PADD * 2) };
}

static int var_grab(CodeBlock *block, vec2_t p) {
    if (vec2_in_range(p, vec2_zero(), var_getsize(block))) {
        setGrabbed(block);
        return 1;
    }
    return 0;
}

static vec2_t var_draw(CodeBlock *block, vec2_t pos) {
    vec2_t size = var_getsize(block);
    noStroke();
    fillColor(COLOR_ARG);
    rect(pos.x, pos.y, size.x, size.y);
    textSize(TEXT_SIZE);
    fillColor(COLOR_TEXT);
    text(block->data, pos.x + PADD, pos.y + size.y / 2 + TEXT_YOFFSET);
    return size;
}

static char *var_text(CodeBlock *block) {
    size_t len = strlen(block->data);
    char *txt = malloc((len+1) * sizeof(char));
    strcpy(txt, block->data);
    txt[len] = 0;
    return txt;
}


static vec2_t if_gettopsize(CodeBlock *block) {
    CodeBlock *cond = (block->blocks->size >= 1) ? block->blocks->items[0] : NULL;
    vec2_t condSize = cond ? CodeBlock_getsize(cond) : (vec2_t){ 0, 0 };
    vec2_t size = (vec2_t){ TEXT_W(3) + condSize.x + PADD * 2, max(HEIGHT, condSize.y + PADD * 2) };
    return size;
}

static vec2_t if_getsize(CodeBlock *block) {
    vec2_t size = if_gettopsize(block);
    vec2_t innSize;
    size.y += IF_YSPACING;
    for (unsigned i = 1; i < block->blocks->size; i++) {
        innSize = CodeBlock_getsize(block->blocks->items[i]);
        size.x = max(size.x, innSize.x + IF_BD_XSP);
        size.y += innSize.y;
    }
    size.y += IF_BD_YSP;
    return size;
}

static vec2_t if_condpos() {
    return (vec2_t){ TEXT_W(3) + PADD, PADD };
}

static int if_grab(CodeBlock *block, vec2_t p) {
    CodeBlock *cond = (block->blocks->size >= 1) ? block->blocks->items[0] : NULL;
    if (cond) {
        vec2_t condPos = if_condpos();
        if (CodeBlock_grab(cond, vec2_sub(p, condPos))) {
            block->blocks->items[0] = NULL;
            return 0;
        }
    }
    vec2_t topsize = if_gettopsize(block);
    float y = topsize.y;
    for (unsigned i = 1; i < block->blocks->size; i++) {
        CodeBlock *b = block->blocks->items[i];
        y += IF_YSPACING;
        vec2_t bPos = (vec2_t){ IF_BD_XSP, y };
        if (CodeBlock_grab(b, vec2_sub(p, bPos))) {
            List_remove_nodelete(block->blocks, i);
            return 0;
        }
        y += CodeBlock_getsize(b).y;
    }
    vec2_t size = if_getsize(block);
    if (vec2_in_range(p, vec2_zero(), (vec2_t){ size.x, topsize.y })) {
        setGrabbed(block);
        return 1;
    }
    return 0;
}

static vec2_t if_draw(CodeBlock *block, vec2_t pos) {
    vec2_t size = if_gettopsize(block);
    vec2_t topSize = size;
    vec2_t innPos = vec2_add(pos, (vec2_t){ IF_BD_XSP, topSize.y });
    vec2_t innSize;

    size.y += IF_YSPACING;
    noStroke();
    fillColor(COLOR_CONTROL);
    rect(pos.x, innPos.y, IF_BORDER, IF_YSPACING);
    innPos.y += IF_YSPACING;

    for (unsigned i = 1; i < block->blocks->size; i++) {
        innSize = CodeBlock_draw(block->blocks->items[i], innPos);
        innSize.y += IF_YSPACING;
        size.y += innSize.y;
        if (innSize.x + IF_BD_XSP > size.x)
            size.x = innSize.x + IF_BD_XSP;
        noStroke();
        fillColor(COLOR_CONTROL);
        rect(pos.x, innPos.y, IF_BORDER, innSize.y);
        innPos.y += innSize.y;
    }

    noStroke();
    fillColor(COLOR_CONTROL);
    rect(pos.x, pos.y, size.x, topSize.y);
    rect(pos.x, innPos.y, size.x, IF_BORDER);

    textSize(TEXT_SIZE);
    fillColor(COLOR_TEXT);
    text("if", pos.x + PADD + TEXT_W(1/2), pos.y + topSize.y / 2 + TEXT_YOFFSET);
    if (block->blocks->size >= 1)
        CodeBlock_draw(block->blocks->items[0], vec2_add(pos, if_condpos()));
    size.y += IF_BORDER;
    return size;
}

static char *if_text(CodeBlock *block) {
    if (block->blocks->size < 1)
        return NULL;

    size_t len = 0;
    char *line;
    size_t lineLen;
    List *lines = List_new(20, NULL, free);
    List *lineLens = List_new(20, NULL, NULL);
    char *subText;
    size_t subLen;

    for (unsigned i = 0; i < block->blocks->size; i++) {
        subLen = strlen(subText = CodeBlock_text(block->blocks->items[i]));
        line = malloc((lineLen = subLen + 1 + (i == 0 ? 8 : CURR_TABSIZE)) * sizeof(char));
        len += lineLen;
        if (i == 0) {
            strncpy(
                strncpy(
                    strncpy(
                        line, "if ", 3)
                    +3, subText, subLen)
                +subLen, " then", 5);
            incTab();
        } else {
            addTabs(line);
            strncpy(line+CURR_TABSIZE, subText, subLen);
        }
        strncpy(line+lineLen-1, "\n", 1);
        free(subText);
        List_push_back(lines, line);
        List_push_back(lineLens, lineLen);
    }
    decTab();
    line = malloc((lineLen = CURR_TABSIZE + 3) * sizeof(char));
    len += lineLen;
    addTabs(line);
    strncpy(line+CURR_TABSIZE, "end", 3);
    List_push_back(lines, line);
    List_push_back(lineLens, lineLen);

    char *txt = malloc((len+1) * sizeof(char));
    char *txtPos = txt;
    for (unsigned i = 0; i < lines->size; i++) {
        strncpy(txtPos, lines->items[i], (size_t)lineLens->items[i]);
        txtPos += (size_t)lineLens->items[i];
    }
    txt[len] = 0;

    List_delete(lines);
    List_delete(lineLens);

    return txt;
}

CodeBlockTypeData CodeBlock_types[] = {
    {0, empty_getsize, empty_grab, empty_draw, empty_text},
    {2, setvar_getsize, setvar_grab, setvar_draw, setvar_text},
    {0, var_getsize, var_grab, var_draw, var_text},
    {1, if_getsize, if_grab, if_draw, if_text},
};

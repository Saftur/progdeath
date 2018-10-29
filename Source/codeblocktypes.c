/**
 * @file codeblocktypes.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/28/18
 * CodeBlock types implementation
 */
#include "codeblocktypes.h"

#include <string.h>
#include <Engine/transform.h>

#include "codeblock.h"

#define HEIGHT 30
#define INNER_HEIGHT (HEIGHT - 4)

#define TEXT_YOFFSET 6

#define COLOR_TEXT      (PColor){  0,   0,   0, 255}
#define COLOR_CONTROL   (PColor){255, 255,   0, 255}
#define COLOR_DIRECTIVE (PColor){ 47,  47, 255, 255}
#define COLOR_ARG       (PColor){255, 191, 127, 255}

#define IF_BORDER 6
#define IF_SPACING 2
#define IF_BD_SP (IF_BORDER + IF_SPACING)

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

static vec2_t setvar_getsize(CodeBlock *block) {
    return (vec2_t){ 312, HEIGHT };
}

static vec2_t setvar_draw(CodeBlock *block, vec2_t pos) {
    CodeBlock *var = (block->blocks->size >= 1) ? block->blocks->items[0] : NULL;
    CodeBlock *val = (block->blocks->size >= 2) ? block->blocks->items[1] : NULL;
    vec2_t size = (vec2_t){ 312, HEIGHT };
    noStroke();
    fillColor(COLOR_DIRECTIVE);
    rect(pos.x, pos.y, size.x, size.y);
    if (var)
        CodeBlock_draw(var, vec2_add(pos, (vec2_t){2, 2}));
    textSize(size.y);
    fillColor(COLOR_TEXT);
    text("=", pos.x + 104, pos.y + size.y - TEXT_YOFFSET);
    if (val)
        CodeBlock_draw(val, vec2_add(pos, (vec2_t){206, 2}));
    return size;
}

static char *setvar_text(CodeBlock *block) {
    if (block->blocks->size < 2)
        return NULL;

    char *var = CodeBlock_text(block->blocks->items[0]);
    char *val = CodeBlock_text(block->blocks->items[1]);
    size_t varlen = strlen(var);
    size_t vallen = strlen(val);
    //           var      =   val
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
    return (vec2_t){ 100, INNER_HEIGHT };
}

static vec2_t var_draw(CodeBlock *block, vec2_t pos) {
    vec2_t size = (vec2_t){ 100, INNER_HEIGHT };
    noStroke();
    fillColor(COLOR_ARG);
    rect(pos.x, pos.y, size.x, size.y);
    textSize(size.y);
    fillColor(COLOR_TEXT);
    text(block->data, pos.x + 2, pos.y + size.y - TEXT_YOFFSET);
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
    vec2_t size = (vec2_t){ 100, HEIGHT };
    vec2_t condSize;
    if (block->blocks->size >= 1) {
        condSize = CodeBlock_getsize(block->blocks->items[0]);
        if (condSize.x + 26 > size.x)
            size.x = condSize.x + 26;
        if (condSize.y + 4 > size.y)
            size.y = condSize.y + 4;
    }
    return size;
}

static vec2_t if_getsize(CodeBlock *block) {
    vec2_t size = if_gettopsize(block);
    vec2_t innSize;
    for (unsigned i = 1; i < block->blocks->size; i++) {
        innSize = CodeBlock_getsize(block->blocks->items[i]);
        if (innSize.x + IF_BORDER > size.x)
            size.x = innSize.x + IF_BORDER;
        size.y += innSize.y;
    }
    size.y += IF_BORDER;
    return size;
}

static vec2_t if_draw(CodeBlock *block, vec2_t pos) {
    vec2_t size = if_gettopsize(block);
    vec2_t topSize = size;
    vec2_t innPos = vec2_add(pos, (vec2_t){ IF_BD_SP, topSize.y });
    vec2_t innSize;

    size.y += IF_SPACING;
    noStroke();
    fillColor(COLOR_CONTROL);
    rect(pos.x, innPos.y, IF_BORDER, IF_SPACING);
    innPos.y += IF_SPACING;

    for (unsigned i = 1; i < block->blocks->size; i++) {
        innSize = CodeBlock_draw(block->blocks->items[i], innPos);
        innSize.y += IF_SPACING;
        size.y += innSize.y;
        if (innSize.x + IF_BD_SP > size.x)
            size.x = innSize.x + IF_BD_SP;
        noStroke();
        fillColor(COLOR_CONTROL);
        rect(pos.x, innPos.y, IF_BORDER, innSize.y);
        innPos.y += innSize.y;
    }

    noStroke();
    fillColor(COLOR_CONTROL);
    rect(pos.x, pos.y, size.x, topSize.y);
    rect(pos.x, innPos.y, size.x, IF_BORDER);

    textSize(topSize.y);
    fillColor(COLOR_TEXT);
    text("if", pos.x + 2, pos.y + HEIGHT - TEXT_YOFFSET);
    if (block->blocks->size >= 1)
        CodeBlock_draw(block->blocks->items[0], vec2_add(pos, (vec2_t){ 24, 2 }));
    size.y += IF_BORDER;
    return size;
}

static char *if_text(CodeBlock *block) {
    if (block->blocks->size < 1)
        return NULL;

    // if (cond) then
    size_t len = 0;
    char *line;
    size_t lineLen;
    List *lines = List_new(20, free);
    List *lineLens = List_new(20, NULL);
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

CodeBlock_getsizefunc CodeBlock_getsizefuncs[] = {
    setvar_getsize,
    var_getsize,
    if_getsize,
};

CodeBlock_drawfunc CodeBlock_drawfuncs[] = {
    setvar_draw,
    var_draw,
    if_draw,
};

CodeBlock_textfunc CodeBlock_textfuncs[] = {
    setvar_text,
    var_text,
    if_text,
};

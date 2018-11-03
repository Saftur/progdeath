/**
 * @file cb_while.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/31/18
 * 
 */
#include "cb_while.h"

#include <stdio.h>

#include "cbtypedata.h"

#define BORDER (HEIGHT / 5)
#define XSPACING (HEIGHT / 15)
#define YSPACING (HEIGHT / 5)
#define BD_XSP (BORDER + XSPACING)
#define BD_YSP (BORDER + YSPACING)

static void init(CodeBlock *block) {
    List_push_back(block->blocks, CodeBlock_new(CB_EMPTY, NULL, 0));
}

static vec2_t gettopsize(CodeBlock *block) {
    CodeBlock *cond = block->blocks->items[0];
    vec2_t condSize = CodeBlock_getsize(cond);
    vec2_t size = (vec2_t){ TEXT_W(5.75) + condSize.x + PADD * 2, max(HEIGHT, condSize.y + PADD * 2) };
    return size;
}

static vec2_t getsize(CodeBlock *block) {
    vec2_t size = gettopsize(block);
    for (unsigned i = 1; i < block->blocks->size; i++) {
        vec2_t innSize = CodeBlock_getsize(block->blocks->items[i]);
        size.x = max(size.x, innSize.x + BD_XSP);
        size.y += innSize.y + YSPACING;
    }
    size.y += BD_YSP;
    return size;
}

static vec2_t condpos() {
    return (vec2_t){ TEXT_W(5.75) + PADD, PADD };
}

static void update(CodeBlock *block, vec2_t pos) {
    CodeBlock *cond = block->blocks->items[0];
    vec2_t condPos = condpos();
    CodeBlock_update(cond, vec2_add(pos, condPos));
    vec2_t topsize = gettopsize(block);
    float y = topsize.y;
    for (unsigned i = 1; i < block->blocks->size; i++) {
        CodeBlock *b = block->blocks->items[i];
        y += YSPACING;
        vec2_t bPos = (vec2_t){ BD_XSP, y };
        vec2_t bSize = CodeBlock_getsize(b);
        CodeBlock_update(b, vec2_add(pos, bPos));
        y += bSize.y;
    }
}

static CBGrabResult grab(CodeBlock *block, vec2_t p, int test) {
    CBGrabResult res;
    CodeBlock *cond = block->blocks->items[0];
    vec2_t condPos = condpos();
    res = CodeBlock_grab_test(cond, vec2_sub(p, condPos), test);
    if (res.either) {
        if (res.parent && !test)
            block->blocks->items[0] = empty_new();
        return GRABRES_CHILD;
    }
    vec2_t topsize = gettopsize(block);
    float y = topsize.y;
    for (unsigned i = 1; i < block->blocks->size; i++) {
        CodeBlock *b = block->blocks->items[i];
        y += YSPACING;
        vec2_t bPos = (vec2_t){ BD_XSP, y };
        vec2_t bSize = CodeBlock_getsize(b);
        res = CodeBlock_grab_test(b, vec2_sub(p, bPos), test);
        if (res.either) {
            if (res.parent && !test)
                List_remove_nodelete(block->blocks, i);
            return GRABRES_CHILD;
        }
        y += bSize.y;
    }
    vec2_t size = getsize(block);
    if (vec2_in_range(p, vec2_zero(), (vec2_t){ size.x, topsize.y })) {
        if (!test) setGrabbed(block, p);
        return GRABRES_PARENT;
    }
    return GRABRES_NEITHER;
}

static int drop(CodeBlock *block, CodeBlock *dropped, vec2_t p) {
    CodeBlock *cond = block->blocks->items[0];
    vec2_t condPos = condpos();
    vec2_t condSize = CodeBlock_getsize(cond);
    vec2_t subP = vec2_sub(p, condPos);
    if (CodeBlock_drop(cond, dropped, subP)) {
        return 1;
    } else if (is_arg(dropped) && vec2_in_range(subP, vec2_zero(), condSize)) {
        CodeBlock_delete(cond);
        block->blocks->items[0] = dropped;
        return 1;
    }
    vec2_t topsize = gettopsize(block);
    vec2_t size = getsize(block);
    float y = topsize.y;
    float insY = topsize.y - PADD;
    float prevInsAdd = PADD;
    float nextInsY = 0;
    for (unsigned i = 1; i < block->blocks->size; i++) {
        CodeBlock *b = block->blocks->items[i];
        y += YSPACING;
        vec2_t bPos = (vec2_t){ BD_XSP, y };
        float bHeight = CodeBlock_getsize(b).y;
        nextInsY = insY + YSPACING + prevInsAdd + (bHeight / 2);
        prevInsAdd = bHeight / 2;
        vec2_t subP = vec2_sub(p, bPos);
        if (CodeBlock_drop(b, dropped, subP)) {
            return 1;
        } else if (is_directive(dropped) && vec2_in_range(p, (vec2_t){ BD_XSP, insY }, (vec2_t){ size.x - BD_XSP, nextInsY })) {
            List_insert(block->blocks, i, dropped);
            return 1;
        }
        y += bHeight;
        insY = nextInsY;
    }
    nextInsY = insY + BD_YSP + prevInsAdd;
    if (is_directive(dropped) && vec2_in_range(p, (vec2_t){ BD_XSP, insY }, (vec2_t){ size.x - BD_XSP, nextInsY })) {
        List_push_back(block->blocks, dropped);
        return 1;
    }
    return 0;
}

static vec2_t draw(CodeBlock *block, vec2_t pos) {
    vec2_t size = gettopsize(block);
    vec2_t topSize = size;
    vec2_t innPos = vec2_add(pos, (vec2_t){ BD_XSP, topSize.y });
    vec2_t innSize;

    size.y += YSPACING;
    noStroke();
    fillColor(COLOR_CONTROL);
    rect(pos.x, innPos.y, BORDER, YSPACING);
    innPos.y += YSPACING;

    for (unsigned i = 1; i < block->blocks->size; i++) {
        innSize = CodeBlock_draw(block->blocks->items[i], innPos);
        innSize.y += YSPACING;
        size.y += innSize.y;
        if (innSize.x + BD_XSP > size.x)
            size.x = innSize.x + BD_XSP;
        noStroke();
        fillColor(COLOR_CONTROL);
        rect(pos.x, innPos.y, BORDER, innSize.y);
        innPos.y += innSize.y;
    }

    noStroke();
    fillColor(COLOR_CONTROL);
    rectRounded(pos.x, pos.y, size.x, topSize.y, RECT_RADIUS);
    rectRounded(pos.x, innPos.y, size.x, BORDER, RECT_RADIUS);
    rect(pos.x, pos.y + topSize.y - RECT_RADIUS, RECT_RADIUS, RECT_RADIUS);
    rect(pos.x, innPos.y, RECT_RADIUS, RECT_RADIUS);

    textSize(TEXT_SIZE);
    fillColor(COLOR_TEXT);
    text("while", pos.x + PADD + TEXT_W(1/4), pos.y + TEXT_YOFFSET(topSize.y));
    CodeBlock_draw(block->blocks->items[0], vec2_add(pos, condpos()));
    size.y += BORDER;
    return size;
}

static char *totext(CodeBlock *block) {
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
        subLen = strlen(subText = CodeBlock_totext(block->blocks->items[i]));
        line = malloc((lineLen = subLen + 1 + (i == 0 ? 9 : CURR_TABSIZE)) * sizeof(char));
        len += lineLen;
        if (i == 0) {
            /*strncpy(
                strncpy(
                    strncpy(
                        line, "while ", 3)
                    +3, subText, subLen)
                +subLen, " do", 5);*/
            sprintf(line, "while %s do", subText);
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

void cb_while_new(CodeBlock *block) {
    block->typeData.isDir = 1;
    block->typeData.isArg = 0;
    block->typeData.init = init;
    block->typeData.delete = NULL;
    block->typeData.getsize = getsize;
    block->typeData.update = update;
    block->typeData.grab = grab;
    block->typeData.drop = drop;
    block->typeData.draw = draw;
    block->typeData.totext = totext;
}

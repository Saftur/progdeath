/**
 * @file editorscreen.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/31/18
 * @brief Editor Screen implementation
 * @addtogroup Game-Screens
 * @{
 */
#include "editorscreen.h"

#include <C_Processing.h>

#include <Engine/gamelayer.h>
#include <Engine/transform.h>
#include <Engine/listener.h>
#include <Engine/util.h>

#include "gamebuttons.h"

#include "codeblock.h"
#include "codeblocklist.h"
#include "codeblockboard.h"
#include "cbgrabcomp.h"

static CBGrabComp *grabbed = NULL;
static CodeBlockBoard *board = NULL;
static CodeBlockList *list = NULL;

PFont monoFont;

void setGrabbed(CodeBlock *block) {
    CBGrabComp_setGrabbed(grabbed, block);
}

void addToBoard(CodeBlock *block, vec2_t pos) {
    CodeBlockBoard_addBlock(board, block, pos);
}

int isOnList(vec2_t pos) {
    pos = vec2_scale(pos, cb_scale);
    pos = vec2_add(pos, cb_offset);
    return vec2_in_range(pos, vec2_zero(), list->size);
}

//  MENU LISTENER

static bool menuEvent(Listener *listener) {
    return keyPressed(KEY_M);
}

static void menuEffect(Listener *listener) {
    ScreenMngr_setNextScreen(listener->comp.owner->objMngr->gLayer->scrMngr, "Menu");
}

//  LEVEL

/**
 * @brief Initialize Editor Screen
 * @param objMngr ObjectMngr to load screen on
 */
void editorScreenInit(ObjectMngr *objMngr) {
    Object *listenObj;
    Listener *listener;

    listenObj = Object_new("Menu Listener");
    listener = Listener_new(menuEvent, menuEffect);
    Object_addComp(listenObj, listener);
    ObjectMngr_addObj(objMngr, listenObj);

    Object *obj;
    Transform *trs;
    CodeBlock *cb, *cb2, *cb3;

    /*obj = Object_new("CodeBlock");
    trs = Transform_new();
    trs->pos = (vec2_t){ canvasWidth / 4, canvasHeight / 4 };

    cb = CodeBlock_new(CB_IF, NULL, 0);
    CodeBlock_setblock(cb, 0, CodeBlock_new(CB_VAR, "test", 5));
    CodeBlock_addblock(cb, cb2 = CodeBlock_new(CB_SETVAR, NULL, 0));
    CodeBlock_setblock(cb2, 0, CodeBlock_new(CB_VAR, "a", 2));
    CodeBlock_setblock(cb2, 1, CodeBlock_new(CB_VAR, "b", 2));
    CodeBlock_addblock(cb, cb2 = CodeBlock_new(CB_SETVAR, NULL, 0));
    CodeBlock_setblock(cb2, 0, CodeBlock_new(CB_VAR, "c", 2));
    CodeBlock_setblock(cb2, 1, CodeBlock_new(CB_VAR, "d", 2));
    CodeBlock_addblock(cb, cb2 = CodeBlock_new(CB_IF, NULL, 0));
    CodeBlock_setblock(cb2, 0, CodeBlock_new(CB_VAR, "test2", 6));
    CodeBlock_addblock(cb2, cb3 = CodeBlock_new(CB_SETVAR, NULL, 0));
    CodeBlock_setblock(cb3, 0, CodeBlock_new(CB_VAR, "e", 2));
    //CodeBlock_setblock(cb3, 1, CodeBlock_new(CB_VAR, "f", 2));

    char *txt = CodeBlock_text(cb);
    printf(txt);
    free(txt);

    Object_addComp(obj, trs);
    Object_addComp(obj, cb);
    ObjectMngr_addObj(objMngr, obj);*/

    obj = Object_new("CodeBlock Board");
    board = CodeBlockBoard_new();
    Object_addComp(obj, board);
    ObjectMngr_addObj(objMngr, obj);

    obj = Object_new("CodeBlock List");
    list = CodeBlockList_new();
    Object_addComp(obj, list);
    ObjectMngr_addObj(objMngr, obj);

    obj = Object_new("CodeBlock Grabbed");
    grabbed = CBGrabComp_new();
    Object_addComp(obj, grabbed);
    ObjectMngr_addObj(objMngr, obj);

    monoFont = loadFont("Assets\\UbuntuMono-Regular.ttf");
    textFont(monoFont, 12);

    cb_scale = 1.f;
    cb_offset = (vec2_t){ 0.f, 0.f };
}

/**
 * @brief Update Editor Screen
 * @param screen Screen to update
 */
void editorScreenUpdate(Screen *screen) {
    float dScale = 0.f;
    if (keyIsDown(KEY_EQUAL))
        dScale += 1.f * dt();
    if (keyIsDown(KEY_MINUS))
        dScale -= 1.f * dt();
    if (dScale != 0) {
        float oldScale = cb_scale;
        cb_scale += dScale;
        vec2_t center = (vec2_t){ canvasWidth / 2, canvasHeight / 2 };
        cb_offset = vec2_add(vec2_scale(vec2_scale(vec2_sub(cb_offset, center), 1 / oldScale), cb_scale), center);
    }
    if (keyIsDown(KEY_LEFT))
        cb_offset.x += 400.f * dt();
    if (keyIsDown(KEY_RIGHT))
        cb_offset.x -= 400.f * dt();
    if (keyIsDown(KEY_UP))
        cb_offset.y += 400.f * dt();
    if (keyIsDown(KEY_DOWN))
        cb_offset.y -= 400.f * dt();
}

/**
 * @brief End Editor Screen
 * @param objMngr ObjectMngr screen was loaded on
 */
void editorScreenEnd(ObjectMngr *objMngr) {
    textFont(getDefaultFont(), 12);
}

/// @}

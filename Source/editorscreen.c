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

void setGrabbed(CodeBlock *block, vec2_t offset) {
    CBGrabComp_setGrabbed(grabbed, block, offset);
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
    return (keyIsDown(KEY_LEFT_CONTROL) || keyIsDown(KEY_LEFT_CONTROL)) && keyPressed(KEY_M);
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
    cb_offset = vec2_zero();
}

/**
 * @brief Update Editor Screen
 * @param screen Screen to update
 */
void editorScreenUpdate(Screen *screen) {
    float oldScale = cb_scale;
    int ctrl = keyIsDown(KEY_LEFT_CONTROL) || keyIsDown(KEY_RIGHT_CONTRO);
    if (ctrl && keyIsDown(KEY_EQUAL))
        cb_scale += 1.f * dt();
    if (ctrl && keyIsDown(KEY_MINUS))
        cb_scale -= 1.f * dt();
    if (cb_scale != oldScale) {
        vec2_t center = (vec2_t){ canvasWidth / 2, canvasHeight / 2 };
        cb_offset = vec2_add(vec2_scale(vec2_scale(vec2_sub(cb_offset, center), 1 / oldScale), cb_scale), center);
    }

    if (ctrl && keyIsDown(KEY_LEFT))
        cb_offset.x += 400.f * dt();
    if (ctrl && keyIsDown(KEY_RIGHT))
        cb_offset.x -= 400.f * dt();
    if (ctrl && keyIsDown(KEY_UP))
        cb_offset.y += 400.f * dt();
    if (ctrl && keyIsDown(KEY_DOWN))
        cb_offset.y -= 400.f * dt();

    if (ctrl && keyPressed(KEY_0)) {
        cb_scale = 1.f;
        cb_offset = vec2_zero();
    }
}

/**
 * @brief End Editor Screen
 * @param objMngr ObjectMngr screen was loaded on
 */
void editorScreenEnd(ObjectMngr *objMngr) {
    textFont(getDefaultFont(), 12);

    CodeBlockBoard_save(board, "Scripts/player.lua");
}

/// @}

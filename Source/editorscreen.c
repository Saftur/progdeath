/**
 * @file editorscreen.c
 * @author Name (username)
 * @date 
 * @brief Sample Screen implementation
 * @addtogroup Game-Screens
 * @{
 */
#include "editorscreen.h"

#include <C_Processing.h>

#include <Engine/gamelayer.h>
#include <Engine/transform.h>
#include <Engine/listener.h>

#include "gamebuttons.h"

#include "codeblock.h"

//  MENU LISTENER

static bool menuEvent(Listener *listener) {
    return keyPressed(KEY_M);
}

static void menuEffect(Listener *listener) {
    ScreenMngr_setNextScreen(listener->comp.owner->objMngr->gLayer->scrMngr, "Menu");
}

//  LEVEL INIT

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

    obj = Object_new("CodeBlock");
    trs = Transform_new();
    trs->pos = (vec2_t){ canvasWidth / 2, canvasHeight / 2 };

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
    ObjectMngr_addObj(objMngr, obj);
}

/// @}

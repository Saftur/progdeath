/**
 * @file gamescreen.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/28/18
 * @brief Game screen implementation
 * @addtogroup Game-Screens
 * @{
 */
#include "gamescreen.h"

#include <Engine/gamelayer.h>
#include <Engine/transform.h>
#include <Engine/physics.h>
#include <Engine/listener.h>

#include "gamebuttons.h"

#include "entity.h"

#include "enemylua.h"
#include "Engine/emptycomp.h"
#include <stdio.h>
#include "../map.h"

//  MENU LISTENER

static bool menuEvent(Listener *listener) {
    return keyPressed(KEY_M);
}

static void menuEffect(Listener *listener) {
    ScreenMngr_setNextScreen(listener->comp.owner->objMngr->gLayer->scrMngr, "Menu");
}

//  LEVEL INIT

Map mainMap = {0};

void tempDrawFunction(Component* comp)
{
    renderMap(&mainMap, 50);
}

/**
 * @brief Initialize Game Screen
 * @param objMngr ObjectMngr to load screen on
 */
void gameScreenInit(ObjectMngr *objMngr) {
    Object *listenObj;
    Listener *listener;

    listenObj = Object_new("Menu Listener");
    listener = Listener_new(menuEvent, menuEffect);
    Object_addComp(listenObj, listener);
    ObjectMngr_addObj(objMngr, listenObj);

    Object *entObj;
    Transform *entTrs;
    Physics *entPhys;
    Entity *ent;

    Object *obj = Object_new("Temp Draw");
    EmptyComp *comp = EmptyComp_new();
    comp->comp.draw = tempDrawFunction;
    Object_addComp(obj, comp);
    ObjectMngr_addObj(objMngr, obj);

    createMap(&mainMap, 0);

    entObj = Object_new("Test Player");
    entTrs = Transform_new();
    entTrs->pos = (vec2_t){ canvasWidth / 2, canvasHeight / 2 };
    entPhys = Physics_new(300.f, 900.f);
    ent = Entity_new("Scripts/test_player.lua", ST_FILENAME, ENT_PLAYER, 100);
    Object_addComp(entObj, entTrs);
    Object_addComp(entObj, entPhys);
    Object_addComp(entObj, ent);
    ObjectMngr_addObj(objMngr, entObj);

    for (unsigned i = 0; i < 1; i++) {
        entObj = Object_new("Test Entity");
        entTrs = Transform_new();
        float angle = map((float)i, 0.f, 100.f, 0.f, TWO_PI);
        entTrs->pos = (vec2_t){ canvasWidth / 2 + 400 * cosf(angle), canvasHeight / 2 + 400 * sinf(angle) };
        entPhys = Physics_new(200.f, 900.f);
        ent = Entity_new(enemyScript, ST_CODE, ENT_ENEMY, 100);
        Object_addComp(entObj, entTrs);
        Object_addComp(entObj, entPhys);
        Object_addComp(entObj, ent);
        ObjectMngr_addObj(objMngr, entObj);
    }
}

/// @}

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
#include "map.h"

//  MENU LISTENER

static bool menuEvent(Listener *listener) {
    return keyPressed(KEY_M);
}

static void menuEffect(Listener *listener) {
    ScreenMngr_setNextScreen(listener->comp.owner->objMngr->gLayer->scrMngr, "Menu");
}

//  LEVEL INIT

Map game_map = {0};

void tempDrawFunction(Component* comp)
{
    renderMap(&game_map, MAP_TILE_SIZE);
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

    createMap(&game_map, 16, objMngr);

    entObj = Object_new("Test Player");
    entTrs = Transform_new();
    entTrs->pos = (vec2_t){ canvasWidth / 2 + 100, canvasHeight / 2 };
    entPhys = Physics_new(300.f, 900.f);
    ent = Entity_new("Scripts/test_player.lua", ST_FILENAME, ENT_PLAYER, 100, ENTITY_RADIUS);
    Object_addComp(entObj, entTrs);
    Object_addComp(entObj, entPhys);
    Object_addComp(entObj, ent);
    ObjectMngr_addObj(objMngr, entObj);

    Entity *playerEnt = ent;

    entObj = Object_new("Spear");
    entTrs = Transform_new();
    entTrs->pos = (vec2_t){ 0, 0 };
    entPhys = Physics_new(300.f, 900.f);
    ent = Entity_new(NULL, ST_NONE, ENT_EQUIPMENT, 0, ENTITY_RADIUS * 4);
    Entity_addType(ent, ENT_SPEAR);
    Object_addComp(entObj, entTrs);
    Object_addComp(entObj, entPhys);
    Object_addComp(entObj, ent);
    ObjectMngr_addObj(objMngr, entObj);

    playerEnt->equipment = ent;

    for (unsigned i = 0; i < 1; i++) {
        entObj = Object_new("Test Entity");
        entTrs = Transform_new();
        float angle = map((float)i, 0.f, 100.f, 0.f, TWO_PI);
        entTrs->pos = (vec2_t){ canvasWidth / 2 + 600 * cosf(angle), canvasHeight / 2 + 1200 };
        entPhys = Physics_new(200.f, 900.f);
        ent = Entity_new(enemyScript, ST_CODE, ENT_ENEMY, 100, ENTITY_RADIUS);
        Object_addComp(entObj, entTrs);
        Object_addComp(entObj, entPhys);
        Object_addComp(entObj, ent);
        ObjectMngr_addObj(objMngr, entObj);
    }
}

/// @}

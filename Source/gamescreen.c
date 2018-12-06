/**
 * @file gamescreen.c
 * @author Arthur Bouvier (a.bouvier)
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
#include "Engine/engine.h"
#include "mapsize.h"

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

    createMap(&game_map, 0, objMngr);

    entObj = Object_new("Test Player");
    entTrs = Transform_new();
    entPhys = Physics_new(300.f, 900.f);
    ent = Entity_new("Scripts/player.lua", ST_FILENAME, ENT_FIGHTER, 100, ENTITY_RADIUS);
    entTrs->pos = (vec2_t) { randomRangeInt(ent->radius, getMapWidth() * MAP_TILE_SIZE - ent->radius),
                             randomRangeInt(ent->radius, getMapHeight() * MAP_TILE_SIZE - ent->radius) };
    Entity_addType(ent, ENT_PLAYER);
    Object_addComp(entObj, entTrs);
    Object_addComp(entObj, entPhys);
    Object_addComp(entObj, ent);
    ObjectMngr_addObj(objMngr, entObj);
    CollCheckFunc check = Engine_getCollCheckFunc(ENTITY, ENTITY);
    for (int i = 0; i < objMngr->objs->size; i++)
    {
        Object *obj = objMngr->objs->items[i];
        Entity *ent2 = Object_getComp(obj, ENTITY);
        if (ent2 && ent2 != ent)
        {
            if (check(ent, ent2))
            {
                entTrs->pos = (vec2_t) { randomRangeInt(ent->radius, getMapWidth() * MAP_TILE_SIZE - ent->radius),
                                         randomRangeInt(ent->radius, getMapHeight() * MAP_TILE_SIZE - ent->radius) };
                i = -1;
            }
        }
    }

    Entity *playerEnt = ent;

    entObj = Object_new("Spear");
    entTrs = Transform_new();
    entTrs->pos = (vec2_t){ 0, 0 };
    entPhys = Physics_new(300.f, 900.f);
    ent = Entity_new(NULL, ST_NONE, ENT_EQUIPMENT, 0, ENTITY_RADIUS * 2);
    Entity_addType(ent, ENT_SPEAR);
    Object_addComp(entObj, entTrs);
    Object_addComp(entObj, entPhys);
    Object_addComp(entObj, ent);
    ObjectMngr_addObj(objMngr, entObj);

    playerEnt->equipment = ent;

    for (unsigned i = 0; i < 10; i++) {
        entObj = Object_new("Test Entity");
        entTrs = Transform_new();
        float angle = map((float)i, 0.f, 100.f, 0.f, TWO_PI);
        entTrs->pos = (vec2_t) { randomRangeInt(ent->radius, getMapWidth() * MAP_TILE_SIZE - ent->radius),
                                 randomRangeInt(ent->radius, getMapHeight() * MAP_TILE_SIZE - ent->radius)};
        //entTrs->pos = (vec2_t){ canvasWidth / 2 + 600 * cosf(angle), canvasHeight / 2 + 1200 };
        entPhys = Physics_new(200.f, 900.f);
        ent = Entity_new(enemyScript, ST_CODE, ENT_FIGHTER, 100, ENTITY_RADIUS);
        Entity_addType(ent, ENT_ENEMY);
        Object_addComp(entObj, entTrs);
        Object_addComp(entObj, entPhys);
        Object_addComp(entObj, ent);
        ObjectMngr_addObj(objMngr, entObj);
    }
}

/// @}

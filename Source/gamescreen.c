/**
 * @file gamescreen.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/27/18
 * @brief Game screen implementation
 * @addtogroup Game-Levels
 * @{
 */
#include "gamescreen.h"

#include <Engine/gamelayer.h>
#include <Engine/transform.h>
#include <Engine/listener.h>

#include "gamebuttons.h"

#include "entity.h"

//  MENU LISTENER

static bool menuEvent(Listener *listener) {
    return keyPressed(KEY_M);
}

static void menuEffect(Listener *listener) {
    LevelMngr_setNextLevel(listener->comp.owner->objMngr->gLayer->lvlMngr, "Menu");
}

//  LEVEL INIT

/**
 * @brief Initialize Game Screen
 * @param objMngr ObjectMngr to load level on
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
    Entity *ent;

    entObj = Object_new("Test Player");
    entTrs = Transform_new();
    entTrs->pos = (vec2_t){ canvasWidth / 2, canvasHeight / 2 };
    ent = Entity_new("Scripts/test_player.lua", ET_PLAYER);
    Object_addComp(entObj, entTrs);
    Object_addComp(entObj, ent);
    ObjectMngr_addObj(objMngr, entObj);

    entObj = Object_new("Test Entity");
    entTrs = Transform_new();
    entTrs->pos = (vec2_t){ canvasWidth / 4, canvasHeight / 4 };
    ent = Entity_new("Scripts/test_entity.lua", ET_ENEMY);
    Object_addComp(entObj, entTrs);
    Object_addComp(entObj, ent);
    ObjectMngr_addObj(objMngr, entObj);

    entObj = Object_new("Test Entity");
    entTrs = Transform_new();
    entTrs->pos = (vec2_t){ canvasWidth / 4, canvasHeight / 4 + 100 };
    ent = Entity_new("Scripts/test_entity.lua", ET_ENEMY);
    Object_addComp(entObj, entTrs);
    Object_addComp(entObj, ent);
    ObjectMngr_addObj(objMngr, entObj);

    /*for (unsigned i = 0; i < 100; i++) {
        entObj = Object_new("Test Entity");
        entTrs = Transform_new();
        entTrs->pos = (vec2_t){ canvasWidth / 4, canvasHeight / 4 };
        ent = Entity_new("Scripts/test_entity.lua");
        Object_addComp(entObj, entTrs);
        Object_addComp(entObj, ent);
        ObjectMngr_addObj(objMngr, entObj);
    }*/
}

/// @}

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
#include <Engine/button.h>
#include <Engine/emptycomp.h>

#include "gamebuttons.h"

#include "entity.h"

//  LEVEL INIT

/**
 * @brief Initialize Game Screen
 * @param objMngr ObjectMngr to load level on
 */
void gameScreenInit(ObjectMngr *objMngr) {
    Object *entObj;
    Transform *entTrs;
    Entity *ent;

    entObj = Object_new("Test Player");
    entTrs = Transform_new();
    entTrs->pos = (Vector2D){ canvasWidth / 2, canvasHeight / 2 };
    ent = Entity_new("Scripts/test_player.lua");
    Object_addComp(entObj, entTrs);
    Object_addComp(entObj, ent);
    ObjectMngr_addObj(objMngr, entObj);

    entObj = Object_new("Test Entity");
    entTrs = Transform_new();
    entTrs->pos = (Vector2D){ canvasWidth / 4, canvasHeight / 4 };
    ent = Entity_new("Scripts/test_entity.lua");
    Object_addComp(entObj, entTrs);
    Object_addComp(entObj, ent);
    ObjectMngr_addObj(objMngr, entObj);
}

/// @}

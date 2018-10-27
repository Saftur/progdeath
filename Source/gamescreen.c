/**
 * @file gamescreen.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/18/18
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
    Object *entObj = Object_new("Test Entity");
    Transform *entTrs = Transform_new();
    Entity *ent = Entity_new("Scripts/test_entity.lua");
    Object_addComp(entObj, entTrs);
    Object_addComp(entObj, ent);
    ObjectMngr_addObj(objMngr, entObj);
}

/// @}

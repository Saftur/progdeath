/**
 * @file apples.c
 * @author Connor Meyers(connor.meyers)
 * @date 10/30/18
 * @brief Deals with apple dropping, healing, and tree destruction
 * @addtogroup Components
 * @{
 */

#include "apples.h"
#include "Source/mapsearching.h"
#include "Engine/objectmngr.h"
#include "Engine/transform.h"
#include "Source/entactions.h"
#include "Engine/physics.h"
#include "Source/entity.h"

/**
  * @brief Spawns an apple on the map
  * @param worldPos world position of tree
  * @param objMngr  object manager
  */
void spawnApple(vec2_t worldPos, ObjectMngr *objMngr)
{
    Object *entObj;
    Transform *entTrs;
    Physics *entPhys;
    Entity *ent;
    entObj = Object_new("APPLE");
    entTrs = Transform_new();
    entTrs->pos = worldPos;
    entPhys = Physics_new(50.f, 900.f);
    ent = Entity_new(NULL, ST_NONE, ENT_ITEM, 100, ENTITY_RADIUS / 2);
    Entity_addType(ent, ENT_APPLE);
    Object_addComp(entObj, entTrs);
    Object_addComp(entObj, entPhys);
    Object_addComp(entObj, ent);
    ObjectMngr_addObj(objMngr, entObj);
}

/// @}

/**
 * @file objectmngr.h
 * @author Arthur Bouvier (a.bouvier)
 * @brief Object manager
 * @course GAM100F18
 * Copyright © 2018 DigiPen (USA) Corporation
 * @addtogroup Objects
 * @{
 */
#pragma once

#include "list.h"
#include "object.h"

typedef struct GameLayer GameLayer; ///< @brief GameLayer forward declaration

/**
 * @brief Starting max number of Object s in ObjectMngr
 */
extern size_t ObjectMngr_startMaxObjs;

/**
 * @brief Holds list of Object s in game
 */
typedef struct ObjectMngr {
    List *objs; ///< @brief List of Objects

    GameLayer *gLayer; ///< @brief Parent GameLayer
} ObjectMngr;

/**
 * @brief Create new ObjectMngr
 * @param gLayer Parent GameLayer
 * @return New ObjectMngr
 */
ObjectMngr *ObjectMngr_new(GameLayer *gLayer);
/**
 * @brief Delete ObjectMngr
 * @param objMngr ObjectMngr to delete
 */
void ObjectMngr_delete(ObjectMngr *objMngr);

/**
 * @brief Update ObjectMngr
 * @param objMngr ObjectMngr to update
 */
void ObjectMngr_update(ObjectMngr *objMngr);
/**
 * @brief Draw Object s in ObjectMngr
 * @param objMngr ObjectMngr to draw
 */
void ObjectMngr_draw(ObjectMngr *objMngr);

/**
 * @brief Add Object to ObjectMngr
 * @param objMngr ObjectMngr to add to
 * @param obj     Object to add
 */
void ObjectMngr_addObj(ObjectMngr *objMngr, Object *obj);
/**
 * @brief Get the number of Object s in ObjectMngr with given name
 * @param objMngr ObjectMngr to get from
 * @param name    Name to check for
 * @return Number of Object s with given name
 */
int ObjectMngr_countObjs(ObjectMngr *objMngr, const char *name);
/**
 * @brief Get an Object with given name from ObjectMngr
 * If there are more than one, choose which with foundNum.
 * @param objMngr  ObjectMngr to get from
 * @param name     Name to check for
 * @param foundNum Number of item in found list wanted
 * @return Object with given name at foundNum index in found list
 */
Object *ObjectMngr_getObj(ObjectMngr *objMngr, const char *name, int foundNum);
/**
 * @brief Delete all Object s in ObjectMngr
 * @param objMngr ObjectMngr to delete from
 */
void ObjectMngr_deleteAll(ObjectMngr *objMngr);

/// @}

/**
 * @file object.h
 * @author Arthur bouvier
 * @brief Game Object
 * @addtogroup Objects
 * @{
 */
#pragma once

#include <stdbool.h>

#include "list.h"

typedef struct ObjectMngr ObjectMngr; ///< @brief ObjectMngr forward declaration
typedef struct Component Component;   ///< @brief Component forward declaration

extern size_t Object_startMaxComps; ///< @brief Starting max number of Object s in ObjectMngr

/**
 * @brief Game Object
 */
typedef struct Object {
    const char *name; ///< @brief Object name
    List *comps;      ///< @brief List of Component s
    List *collComps;  ///< @brief List of collidable Component s

    int toDestroy; ///< @brief Used to tell ObjectMngr when to delete this Object

    ObjectMngr *objMngr; ///< @brief Parent ObjectMngr
} Object;

/**
 * @brief Create new Object
 * @param name Object name
 * @return New Object
 */
Object *Object_new(const char *name);
/**
 * @brief Clone Object
 * @param obj Object to clone
 * @return Cloned Object
 */
Object *Object_clone(Object *obj);
/**
 * @brief Delete Object
 * @param obj Object to delete
 */
void Object_delete(Object *obj);
/**
 * @brief Mark an Object for destruction
 * @param obj Object to be marked
 */
void Object_destroy(Object *obj);
/**
 * @brief Check if an Object is destroyed
 * @param obj Object to check
 * @return If obj is destroyed
 */
bool Object_isDestroyed(Object *obj);

/**
 * @brief Update Object
 * @param obj Object to update
 */
void Object_update(Object *obj);
/**
 * @brief Draw Object
 * @param obj Object to draw
 */
void Object_draw(Object *obj);

/**
 * @brief Add Component to Object
 * @param obj  Object to add to
 * @param comp Component to add
 */
void Object_addComp(Object *obj, Component *comp);
/**
 * @brief Get Component from Object
 * @param obj    Object to get Component from
 * @param typeId Type id of Component
 * @return Found Component
 */
Component *Object_getComp(Object *obj, unsigned typeId);

/// @}

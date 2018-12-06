/**
 * @file object.c
 * @author Arthur Bouvier (a.bouvier)
 * @brief Object implementation
 * @addtogroup Objects
 * @{
 */
#include "object.h"

#include "component.h"

/**
 * @brief Starting max number of Object s in ObjectMngr
 */
size_t Object_startMaxComps = 10;

/**
 * @brief Create new Object
 * @param name Object name
 * @return New Object
 */
Object *Object_new(const char *name) {
    Object *obj = malloc(sizeof(Object));
    obj->name = name;
    obj->comps = List_new(Object_startMaxComps, Component_clone, Component_delete);
    obj->collComps = List_new(Object_startMaxComps, NULL, NULL);
    obj->toDestroy = -1;
    return obj;
}

/**
 * @brief Clone Object
 * @param obj Object to clone
 * @return Cloned Object
 */
Object *Object_clone(Object *obj) {
    Object *new = malloc(sizeof(Object));
    new->name = obj->name;
    new->comps = List_copy(obj->comps);
    for (unsigned i = 0; i < new->comps->size; i++)
        ((Component*)new->comps->items[i])->owner = new;
    new->collComps = List_copy(obj->comps);
    obj->toDestroy = -1;
    return new;
}

/**
 * @brief Delete Object
 * @param obj Object to delete
 */
void Object_delete(Object *obj) {
    List_delete(obj->comps);
    List_delete(obj->collComps);
    free(obj);
}

/**
 * @brief Mark an Object for destruction
 * @param obj Object to be marked
 */
void Object_destroy(Object *obj) {
    obj->toDestroy = 1;
}

/**
 * @brief Check if an Object is destroyed
 * @param obj Object to check
 * @return If obj is destroyed
 */
bool Object_isDestroyed(Object *obj) {
    return obj->toDestroy >= 0;
}

/**
 * @brief Update Object
 * @param obj Object to update
 */
void Object_update(Object *obj) {
    for (unsigned i = 0; i < obj->comps->size; i++)
        Component_update(obj->comps->items[i]);
}

/**
 * @brief Draw Object
 * @param obj Object to draw
 */
void Object_draw(Object *obj) {
    for (unsigned i = 0; i < obj->comps->size; i++)
        Component_draw(obj->comps->items[i]);
}

/**
 * @brief Add Component to Object
 * @param obj  Object to add to
 * @param comp Component to add
 */
void Object_addComp(Object *obj, Component *comp) {
    comp->owner = obj;
    List_push_back(obj->comps, comp);
    if (comp->collides)
        List_push_back(obj->collComps, comp);
}

/**
 * @brief Get Component from Object
 * @param obj    Object to get Component from
 * @param typeId Type id of Component
 * @return Found Component
 */
Component *Object_getComp(Object *obj, unsigned typeId) {
    for (unsigned i = 0; i < obj->comps->size; i++) {
        Component *comp = obj->comps->items[i];
        if (comp->typeId == typeId)
            return comp;
    }
    return NULL;
}

/// @}

/**
 * @file objectmngr.c
 * @author Arthur Bouvier (a.bouvier)
 * @brief ObjectMngr implementation
 * @addtogroup Objects
 * @{
 */
#include "objectmngr.h"

#include <stdlib.h> // malloc
#include <string.h> // strcmp

#include "engine.h"
#include "gamelayer.h"
#include "component.h"

/**
 * @brief Starting max number of Object s in ObjectMngr
 */
size_t ObjectMngr_startMaxObjs = 10;

/**
 * @brief Create new ObjectMngr
 * @param gLayer Parent GameLayer
 * @return New ObjectMngr
 */
ObjectMngr *ObjectMngr_new(GameLayer *gLayer) {
    ObjectMngr *objMngr = malloc(sizeof(ObjectMngr));
    objMngr->objs = List_new(ObjectMngr_startMaxObjs, Object_clone, Object_delete);
    objMngr->gLayer = gLayer;
    return objMngr;
}

/**
 * @brief Delete ObjectMngr
 * @param objMngr ObjectMngr to delete
 */
void ObjectMngr_delete(ObjectMngr *objMngr) {
    List_delete(objMngr->objs);
    free(objMngr);
}

/**
 * @brief Update ObjectMngr
 * @param objMngr ObjectMngr to update
 */
void ObjectMngr_update(ObjectMngr *objMngr) {
    for (unsigned i = 0; i < objMngr->objs->size; i++) {
        Object *obj = objMngr->objs->items[i];
        if (Object_isDestroyed(obj)) {
            if (obj->toDestroy-- == 0)
                List_remove(objMngr->objs, i);
        } else Object_update(obj);
    }
    // Check collisions
    size_t numObjs = objMngr->objs->size;
    for (unsigned o1 = 0; o1 < numObjs; o1++) {
        Object *obj1 = objMngr->objs->items[o1];
        if (obj1->collComps->size == 0 || Object_isDestroyed(obj1)) continue;
        for (unsigned o2 = o1+1; o2 < numObjs; o2++) {
            Object *obj2 = objMngr->objs->items[o2];
            if (obj2->collComps->size == 0 || Object_isDestroyed(obj2)) continue;
            for (unsigned c1 = 0; c1 < obj1->collComps->size; c1++) {
                Component *comp1 = obj1->collComps->items[c1];
                for (unsigned c2 = 0; c2 < obj2->collComps->size; c2++) {
                    Component *comp2 = obj2->collComps->items[c2];
                    CollCheckFunc check = Engine_getCollCheckFunc(comp1->typeId, comp2->typeId);
                    if (check && check(comp1, comp2)) {
                        CollResolveFunc resolve = Engine_getCollResolveFunc(comp1->typeId, comp2->typeId);
                        if (resolve)
                            resolve(comp1, comp2);
                        Component_coll_resolve(comp1, comp2);
                    }
                }
            }
        }
    }
}

/**
 * @brief Draw Object s in ObjectMngr
 * @param objMngr ObjectMngr to draw
 */
void ObjectMngr_draw(ObjectMngr *objMngr) {
    for (unsigned i = 0; i < objMngr->objs->size; i++) {
        Object *obj = objMngr->objs->items[i];
        if (!Object_isDestroyed(obj))
            Object_draw(obj);
    }
}

/**
 * @brief Add Object to ObjectMngr
 * @param objMngr ObjectMngr to add to
 * @param obj     Object to add
 */
void ObjectMngr_addObj(ObjectMngr *objMngr, Object *obj) {
    obj->objMngr = objMngr;
    List_push_back(objMngr->objs, obj);
}

/**
 * @brief Get the number of Object s in ObjectMngr with given name
 * @param objMngr ObjectMngr to get from
 * @param name    Name to check for
 * @return Number of Object s with given name
 */
int ObjectMngr_countObjs(ObjectMngr *objMngr, const char *name) {
    int count = 0;
    for (unsigned i = 0; i < objMngr->objs->size; i++) {
        if (strcmp(((Object*)objMngr->objs->items[i])->name, name) == 0)
            count++;
    }
    return count;
}

/**
 * @brief Get an Object with given name from ObjectMngr
 * If there are more than one, choose which with foundNum.
 * @param objMngr  ObjectMngr to get from
 * @param name     Name to check for
 * @param foundNum Number of item in found list wanted
 * @return Object with given name at foundNum index in found list
 */
Object *ObjectMngr_getObj(ObjectMngr *objMngr, const char *name, int foundNum) {
    int num = 0;
    for (unsigned i = 0; i < objMngr->objs->size; i++) {
        if (strcmp(((Object*)objMngr->objs->items[i])->name, name) == 0 && num++ == foundNum)
            return objMngr->objs->items[i];
    }
    return NULL;
}

/**
 * @brief Delete all Object s in ObjectMngr
 * @param objMngr ObjectMngr to delete from
 */
void ObjectMngr_deleteAll(ObjectMngr *objMngr) {
    List_clear(objMngr->objs);
}

/// @}

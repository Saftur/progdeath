/**
 * @file entity.c
 * @author Name (username)
 * @date 10/28/18
 * @brief Entity implementation
 * @addtogroup Components
 * @{
 */
#include "entity.h"

#include <lauxlib.h>
#include <lualib.h>

#include <Engine/transform.h>

#include "luautil.h"
#include "luafuncs.h"
#include "entluafuncs.h"

/**
 * @brief Create new Entity
 * @param scriptName Lua script name
 * @param baseType   Entity base type
 * @return New Entity
 */
Entity *Entity_new(const char *scriptName, EntityType baseType) {
    Entity *this = malloc(sizeof(Entity));
    this->comp.typeName = "Entity";
    this->comp.typeId = ENTITY;
    this->comp.delete = _Entity_delete;
    this->comp.update = _Entity_update;
    this->comp.draw = _Entity_draw;
    this->comp.collides = false;
    this->comp.coll_resolve = _Entity_coll_resolve;

    initEntityLuaState(this, scriptName);
    if (!this->script) {
        _Entity_delete(this);
        return NULL;
    }
    register_luafuncs(this->script);

    this->types = List_new(10, NULL);
    List_push_back(this->types, (void*)baseType);

    this->detectRange = 300.f;

    return this;
}

/**
 * @brief Delete Entity
 * @param this Entity to delete
 */
void _Entity_delete(Entity *this) {
    lua_close(this->script);
    List_delete(this->types);
    free(this);
}

/**
 * @brief Update Entity
 * @param this Entity to update
 */
void _Entity_update(Entity *this) {
    resetTimeoutHook(this->script);
    lua_getglobal(this->script, "update");
    lua_call(this->script, 0, 0);

    Transform *trs;
    if (List_find(this->types, ENT_PLAYER, NULL) && (trs = Object_getComp(this->comp.owner, TRANSFORM))) {
        translate(canvasWidth / 2 - trs->pos.x, canvasHeight / 2 - trs->pos.y);
    }
}

/**
 * @brief Draw Entity
 * @param this Entity to draw
 */
void _Entity_draw(Entity *this) {
    Transform *trs = Object_getComp(this->comp.owner, TRANSFORM);
    if (!trs) return;
    fill(0, 0, 0, 255);
    noStroke();
    circle(trs->pos.x, trs->pos.y, 20);
}

/**
 * @brief Resolve collisions with other Component s
 * @param this  This Component
 * @param other Other Component
 */
void _Entity_coll_resolve(Entity *this, Component *other) {

}

/**
 * @brief Add type to this Entity 's type list
 * @param this Entity to add to
 * @param type EntityType to add
 */
void Entity_addType(Entity *this, EntityType type) {
    if (Entity_isType(this, type))
        List_push_back(this->types, (void*)type);
}

/**
 * @brief Check if Entity is of given EntityType
 * @param this Entity to check
 * @param type EntityType to check
 * @return If this is of given type
 */
int Entity_isType(Entity *this, EntityType type) {
    return List_find(this->types, (void*)type, NULL);
}

/// @}


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
 * @param type       Entity type
 * @return New Entity
 */
Entity *Entity_new(const char *scriptName, EntityType type) {
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

    this->type = type;

    this->detectRange = 300.f;

    return this;
}

/**
 * @brief Delete Entity
 * @param this Entity to delete
 */
void _Entity_delete(Entity *this) {
    lua_close(this->script);
    free(this);
}

/**
 * @brief Update Entity
 * @param this Entity to update
 */
void _Entity_update(Entity *this) {
    resetTimeoutHook(this->script);
    //luaU_call(this->script, "update", ">");
    lua_getglobal(this->script, "update");
    lua_call(this->script, 0, 0);

    Transform *trs;
    if ((this->type == ET_PLAYER) && (trs = Object_getComp(this->comp.owner, TRANSFORM))) {
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
    fill(255, 255, 255, 255);
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

/// @}


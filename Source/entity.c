/**
 * @file entity.c
 * @author Name (username)
 * @date 10/26/18
 * @brief Entity implementation
 * @addtogroup Components
 * @{
 */
#include "entity.h"

#include <lauxlib.h>
#include <lualib.h>

#include <Engine/transform.h>

#include "luautil.h"
#include "entluafuncs.h"

/**
 * @brief Create new Entity
 * @param scriptName Lua script name
 * @return New Entity
 */
Entity *Entity_new(const char *scriptName) {
    Entity *this = malloc(sizeof(Entity));
    this->comp.typeName = "Entity";
    this->comp.typeId = ILLEGAL_COMP;
    this->comp.delete = _Entity_delete;
    this->comp.update = _Entity_update;
    this->comp.draw = _Entity_draw;
    this->comp.collides = false;
    this->comp.coll_resolve = _Entity_coll_resolve;

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    if (luaL_loadfile(L, scriptName) || lua_pcall(L, 0, 0, 0)) {
        fprintf(stderr, "Failed to load script \"%s\"", scriptName);
        _Entity_delete(this);
        return NULL;
    }

    lua_pushstring(L, "Entity");
    lua_pushlightuserdata(L, this);
    lua_settable(L, LUA_REGISTRYINDEX);

    register_entluafuncs(L);

    this->script = L;

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
    //lua_getglobal(this->script, "update");
    //lua_call(this->script, 0, 0);
    luaU_call(this->script, "update", ">");
}

/**
 * @brief Draw Entity
 * @param this Entity to draw
 */
void _Entity_draw(Entity *this) {
    Transform *trs = Object_getComp(this->comp.owner, TRANSFORM);
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


/**
 * @file entluafuncs.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/27/18
 * Entity Lua Functions implementation
 */
#include "entluafuncs.h"

#include <lauxlib.h>
#include <lualib.h>

#include <Engine/objectmngr.h>
#include <Engine/transform.h>

#include "luautil.h"
#include "luavector2d.h"
#include "entity.h"

#define SCRIPT_TIMEOUT 1000

/**
 * @brief Get Entity from Lua state
 * @param L Lua state
 * @return Lua state's Entity
 */
static Entity *getEntity(lua_State *L) {
    lua_pushstring(L, "Entity");
    lua_gettable(L, LUA_REGISTRYINDEX);
    return lua_touserdata(L, -1);
}

/**
 * @brief Entity panic function
 * @param 1 Error object
 */
static int l_panic(lua_State *L) {
    fprintf(stderr, "%s\n", luaL_checkstring(L, 1));
    Entity *ent = getEntity(L);
    Object_destroy(ent->comp.owner);
}

/**
 * @brief Timeout hook
 * @param 1 Hook reason
 */
static int l_counthook(lua_State *L) {
    lua_pushstring(L, "Entity script ran too long");
    lua_error(L);
}


/**
 * @brief Move Entity by Vector2D v
 * @param ent Entity to move
 * @param v   Vector2D to move by
 */
static void move(Entity *ent, Vector2D v) {
    Transform *trs = Object_getComp(ent->comp.owner, TRANSFORM);
    trs->pos = Vector2D_add(trs->pos, v);
}

/**
 * @brief Move by Vector2D
 * @param 1 Vector2D to move by
 */
static int l_move(lua_State *L) {
    Entity *ent = getEntity(L);
    Vector2D v;
    switch (lua_type(L, 1)) {
    case LUA_TNUMBER:
        luaL_argcheck(L, lua_isnumber(L, 2), 2, "'number' expected");
        v.y = luaL_checknumber(L, 2);
        v.x = lua_tonumber(L, 1);
        break;

    case LUA_TTABLE:
        lua_rawgeti(L, 1, 1);
        v.x = luaL_checknumber(L, -1);
        lua_pop(L, 1);
        lua_rawgeti(L, 1, 2);
        v.y = luaL_checknumber(L, -1);
        lua_pop(L, 1);
        break;

    default:
        luaL_argcheck(L, false, 1, "'table' or 'number' expected");
    }
    move(ent, v);
}

/**
 * @brief Get nearby Entity s
 * @return Table of Entity s
 */
static int l_getnearby(lua_State *L) {
    Entity *ent = getEntity(L);
    Transform *trs = Object_getComp(ent->comp.owner, TRANSFORM);
    if (!trs)
        return 0;
    lua_newtable(L);
    List *objs = ent->comp.owner->objMngr->objs;
    int count = 0;
    for (unsigned i = 0; i < objs->size; i++) {
        Object *obj = objs->items[i];
        Entity *objEnt;
        Transform *objTrs;
        if (!(objEnt = Object_getComp(obj, ENTITY)) || objEnt == ent || !(objTrs = Object_getComp(obj, TRANSFORM)))
            continue;
        if (Vector2D_dist(trs->pos, objTrs->pos) < ent->detectRange) {
            lua_pushlightuserdata(L, objEnt);
            lua_rawseti(L, -2, ++count);
        }
    }
    return 1;
}

/**
 * @brief Get position of Entity
 * @param 1 Entity to get position of (default: self)
 * @param Vector2D position of Entity
 */
static int l_getpos(lua_State *L) {
    Entity *ent = lua_touserdata(L, 1);
    if (!ent)
        ent = getEntity(L);
    Transform *trs;
    if (!ent || !(trs = Object_getComp(ent->comp.owner, TRANSFORM)))
        return 0;
    lua_newtable(L);
    lua_pushnumber(L, trs->pos.x);
    lua_rawseti(L, -2, 1);
    lua_pushnumber(L, trs->pos.y);
    lua_rawseti(L, -2, 2);
    return 1;
}

/**
 * @brief Initialize Entity Lua script state
 * @param ent        Entity whose script to initialize
 * @param scriptName Name of Lua script to use
 */
void initEntityLuaState(Entity *ent, const char *scriptName) {
    lua_State *L = luaL_newstate();
    luaL_requiref(L, "_G", luaopen_base, 1);
    luaL_requiref(L, "string", luaopen_string, 1);
    luaL_requiref(L, "table", luaopen_table, 1);
    luaL_requiref(L, "math", luaopen_math, 1);
    luaL_requiref(L, "Vector2D", luaopen_Vector2D, 1);
    lua_pop(L, 5);

    lua_pushstring(L, "debug");
    luaL_requiref(L, "debug", luaopen_debug, 1);
    lua_settable(L, LUA_REGISTRYINDEX);
    lua_pushnil(L);
    lua_setglobal(L, "debug");

    if (luaL_loadfile(L, scriptName) || lua_pcall(L, 0, 0, 0)) {
        fprintf(stderr, "Failed to load script \"%s\"", scriptName);
        return NULL;
    }

    lua_pushstring(L, "Entity");
    lua_pushlightuserdata(L, ent);
    lua_settable(L, LUA_REGISTRYINDEX);

    luaU_setpanic(L, l_panic);

    lua_pushstring(L, "COUNTHOOK");
    lua_pushcfunction(L, l_counthook);
    lua_settable(L, LUA_REGISTRYINDEX);

    lua_pushcfunction(L, l_move);
    lua_setglobal(L, "Move");

    lua_pushcfunction(L, l_getnearby);
    lua_setglobal(L, "GetNearby");

    lua_pushcfunction(L, l_getpos);
    lua_setglobal(L, "GetPos");

    ent->script = L;
}

/**
 * @brief Reset the timeout Lua hook
 * @param L Lua state
 */
void resetTimeoutHook(lua_State *L) {
    lua_pushstring(L, "debug");
    lua_gettable(L, LUA_REGISTRYINDEX);
    lua_pushstring(L, "sethook");
    lua_gettable(L, -2);
    lua_pushstring(L, "COUNTHOOK");
    lua_gettable(L, LUA_REGISTRYINDEX);
    lua_pushstring(L, "");
    lua_pushnumber(L, SCRIPT_TIMEOUT);
    lua_pcall(L, 3, 0, 0);

}

/**
 * @file entluafuncs.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/27/18
 * Entity Lua Functions implementation
 */
#include "entluafuncs.h"

#include <lauxlib.h>
#include <lualib.h>

#include <Engine/transform.h>

#include "luautil.h"
#include "entity.h"

#define SCRIPT_TIMEOUT 1000

static Entity *getEntity(lua_State *L) {
    lua_pushstring(L, "Entity");
    lua_gettable(L, LUA_REGISTRYINDEX);
    return lua_touserdata(L, -1);
}

static int l_panic(lua_State *L) {
    fprintf(stderr, "%s\n", luaL_checkstring(L, 1));
    Entity *ent = getEntity(L);
    Object_destroy(ent->comp.owner);
}

static int l_counthook(lua_State *L) {
    lua_pushstring(L, "Entity script ran too long");
    lua_error(L);
}

static void move(Entity *ent, Vector2D v) {
    Transform *trs = Object_getComp(ent->comp.owner, TRANSFORM);
    trs->pos = Vector2D_add(trs->pos, v);
}

static int l_move(lua_State *L) {
    Entity *ent = getEntity(L);
    Vector2D v;
    switch (lua_type(L, 1)) {
    case LUA_TNUMBER:
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
        luaL_error(L, "error: wrong type passed into Move");
    }
    move(ent, v);
}

lua_State *initEntityLuaState(Entity *ent, const char *scriptName) {
    lua_State *L = luaL_newstate();
    luaL_requiref(L, "_G", luaopen_base, 1);
    luaL_requiref(L, "string", luaopen_string, 1);
    luaL_requiref(L, "table", luaopen_table, 1);
    luaL_requiref(L, "math", luaopen_math, 1);
    lua_pop(L, 4);

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

    return L;
}

void setCountHook(lua_State *L) {
    lua_pushstring(L, "debug");
    lua_gettable(L, LUA_REGISTRYINDEX);
    lua_pushstring(L, "sethook");
    const char *t = lua_typename(L, lua_type(L, -2));
    lua_gettable(L, -2);
    lua_pushstring(L, "COUNTHOOK");
    lua_gettable(L, LUA_REGISTRYINDEX);
    lua_pushstring(L, "");
    lua_pushnumber(L, SCRIPT_TIMEOUT);
    lua_pcall(L, 3, 0, 0);

}

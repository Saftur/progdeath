/**
 * @file entluafuncs.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/27/18
 * Entity Lua Functions implementation
 */
#include "entluafuncs.h"

#include <lauxlib.h>

#include <Engine/transform.h>

#include "entity.h"

static Entity *getEntity(lua_State *L) {
    lua_pushstring(L, "Entity");
    lua_gettable(L, LUA_REGISTRYINDEX);
    return lua_touserdata(L, -1);
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

void register_entluafuncs(lua_State *L) {
    lua_pushcfunction(L, l_move);
    lua_setglobal(L, "Move");
}

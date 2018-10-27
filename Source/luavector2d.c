/**
 * @file luavector2d.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/27/18
 * Lua Vector2D implementation
 */
#include "luavector2d.h"

#include <lauxlib.h>

#include <Engine/vector2d.h>

static int l_new(lua_State *L) {
    // Check argument
    luaL_argcheck(L, lua_istable(L, 1), 1, "'table' expected");
    // Get X
    lua_pushnumber(L, 1);
    lua_gettable(L, 1);
    if (!lua_isnumber(L, -1)) {
        lua_pop(L, 1);
        lua_pushstring(L, "x");
        lua_gettable(L, 1);
    }
    if (!lua_isnumber(L, -1)) {
        lua_pop(L, 1);
        lua_pushstring(L, "X");
        lua_gettable(L, 1);
    }
    // Get Y
    lua_pushnumber(L, 2);
    lua_gettable(L, 1);
    if (!lua_isnumber(L, -1)) {
        lua_pop(L, 1);
        lua_pushstring(L, "y");
        lua_gettable(L, 1);
    }
    if (!lua_isnumber(L, -1)) {
        lua_pop(L, 1);
        lua_pushstring(L, "Y");
        lua_gettable(L, 1);
    }
    // Check X & Y
    luaL_argcheck(L, lua_isnumber(L, -2) && lua_isnumber(L, -1), 1, "incorrect table");

    Vector2D *v = lua_newuserdata(L, sizeof(Vector2D));
    luaL_getmetatable(L, "Vector2D");
    lua_setmetatable(L, -2);
    v->x = lua_tonumber(L, -3);
    v->y = lua_tonumber(L, -2);
    return 1;
}

static float *getXY(lua_State *L) {
    Vector2D *v = lua_touserdata(L, 1);
    luaL_argcheck(L, lua_isstring(L, 2), 2, "'number' or 'string' expected");
    const char *i = luaL_checkstring(L, 2);
    switch (*i) {
    case 'x': case 'X': case '1':
        return &(v->x);
        break;
    case 'y': case 'Y': case'2':
        return &(v->y);
        break;
    default:
        luaL_argcheck(L, false, 2, "'x', 'X', 1, 'y', 'Y', or 2 expected");
    }
}

static int l_index(lua_State *L) {
    lua_pushnumber(L, *getXY(L));
    return 1;
}

static int l_newindex(lua_State *L) {
    float *f = getXY(L);
    luaL_argcheck(L, lua_isnumber(L, 3), 3, "'number' expected");
    *f = luaL_checknumber(L, 3);
    return 0;
}

static const struct luaL_Reg Vector2Dlib_f[] = {
    {"New", l_new},
    {NULL, NULL}
};

static const struct luaL_Reg Vector2Dlib_m[] = {
    {"__index", l_index},
    {"__newindex", l_newindex},
    {NULL, NULL}
};

/**
 * @brief Initialize Lua Vector2D library
 * @param L Lua state
 */
int luaopen_Vector2D(lua_State *L) {
    luaL_newmetatable(L, "Vector2D");

    luaL_setfuncs(L, Vector2Dlib_m, 0);

    /*lua_newtable(L);
    luaL_setfuncs(L, Vector2Dlib_f, 0);
    lua_setglobal(L, "Vector2D");*/
    luaL_newlib(L, Vector2Dlib_f);
    return 1;
}

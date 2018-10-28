/**
 * @file luavec2.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/27/18
 * Lua vec2_t implementation
 */
#include "luavec2.h"

#include <lauxlib.h>
#include <CP_Math.h>

#include "luautil.h"

static int l_new(lua_State *L) {
    vec2_t *v = lua_newuserdata(L, sizeof(vec2_t));
    luaL_getmetatable(L, "vec2");
    lua_setmetatable(L, -2);
    switch (lua_type(L, 1)) {
    case LUA_TTABLE:
        // Get X
        lua_geti(L, 1, 1);
        if (!lua_isnumber(L, -1)) {
            lua_pop(L, 1);
            lua_getfield(L, 1, "x");
        }
        if (!lua_isnumber(L, -1)) {
            lua_pop(L, 1);
            lua_getfield(L, 1, "X");
        }
        if (!lua_isnumber(L, -1))
            luaL_argcheck(L, 0, 1, "'number' expected");
        v->x = lua_tonumber(L, -1);
        // Get Y
        lua_geti(L, 1, 2);
        if (!lua_isnumber(L, -1)) {
            lua_pop(L, 1);
            lua_getfield(L, 1, "y");
        }
        if (!lua_isnumber(L, -1)) {
            lua_pop(L, 1);
            lua_getfield(L, 1, "Y");
        }
        if (!lua_isnumber(L, -1))
            luaL_argcheck(L, 0, 1, "'number' expected");
        v->y = lua_tonumber(L, -1);
        lua_pop(L, 2);
        break;
    case LUA_TNUMBER:
        luaL_argcheck(L, lua_isnumber(L, 2), 2, "'number' expected");
        v->y = luaL_checknumber(L, 2);
        v->x = lua_tonumber(L, 1);
        break;
    case LUA_TNONE:
        v->x = 0.f;
        v->y = 0.f;
        break;
    default:
        luaL_argcheck(L, 0, 1, "'table' or 'number' expected");
    }
    return 1;
}

static float *getXY(lua_State *L) {
    vec2_t *v = lua_touserdata(L, 1);
    if (!lua_isstring(L, 2))
        return NULL;
    //luaL_argcheck(L, lua_isstring(L, 2), 2, "'number' or 'string' expected");
    const char *i = luaL_checkstring(L, 2);
    switch (*i) {
    case 'x': case 'X': case '1':
        return &(v->x);
        break;
    case 'y': case 'Y': case'2':
        return &(v->y);
        break;
    default:
        return NULL;
        //luaL_argcheck(L, 0, 2, "'x', 'X', 1, 'y', 'Y', or 2 expected");
    }
}

static int l_index(lua_State *L) {
    float *f = getXY(L);
    if (f) {
        lua_pushnumber(L, *getXY(L));
    } else if (lua_isstring(L, 2)) {
        lua_getmetatable(L, 1);
        lua_pushvalue(L, 2);
        lua_rawget(L, -2);
    } else {
        luaL_argcheck(L, lua_isstring(L, 2), 2, "'number' or 'string' expected");
    }
    return 1;
}

static int l_newindex(lua_State *L) {
    float *f = getXY(L);
    if (!f)
        luaL_argcheck(L, 0, 2, "'x', 'X', 1, 'y', 'Y', or 2 expected");
    luaL_argcheck(L, lua_isnumber(L, 3), 3, "'number' expected");
    *f = luaL_checknumber(L, 3);
    return 0;
}

static int l_call(lua_State *L) {
    lua_getglobal(L, "vec2");
    lua_getfield(L, -1, "new");
    lua_pushvalue(L, 2);
    if (lua_istable(L, -1)) {
        if (!lua_pcall(L, 1, 1, 0))
            return 1;
    } else {
        lua_pushvalue(L, 3);
        if (!lua_pcall(L, 2, 1, 0))
            return 1;
    }
}

static int l_unm(lua_State *L) {
    vec2_t *v = luaL_checkudata(L, 1, "vec2");
    vec2_t nv = vec2_neg(*v);
    lua_getglobal(L, "vec2");
    lua_getfield(L, -1, "new");
    lua_pushnumber(L, nv.x);
    lua_pushnumber(L, nv.y);
    if (!lua_pcall(L, 2, 1, 0))
        return 1;
}

static int l_add(lua_State *L) {
    vec2_t a = *(vec2_t*)luaL_checkudata(L, 1, "vec2");
    luaL_argcheck(L, lua_isuserdata(L, 2), 2, "'vec2' expected");
    vec2_t b = *(vec2_t*)luaL_checkudata(L, 2, "vec2");
    vec2_t nv = vec2_add(a, b);
    lua_getglobal(L, "vec2");
    lua_getfield(L, -1, "new");
    lua_pushnumber(L, nv.x);
    lua_pushnumber(L, nv.y);
    if (!lua_pcall(L, 2, 1, 0))
        return 1;
}

static int l_sub(lua_State *L) {
    vec2_t a = *(vec2_t*)luaL_checkudata(L, 1, "vec2");
    luaL_argcheck(L, lua_isuserdata(L, 2), 2, "'vec2' expected");
    vec2_t b = *(vec2_t*)luaL_checkudata(L, 2, "vec2");
    vec2_t nv = vec2_sub(a, b);
    lua_getglobal(L, "vec2");
    lua_getfield(L, -1, "new");
    lua_pushnumber(L, nv.x);
    lua_pushnumber(L, nv.y);
    if (!lua_pcall(L, 2, 1, 0))
        return 1;
}

static int l_mul(lua_State *L) {
    if (lua_isnumber(L, 1)) {
        lua_pushvalue(L, 1);
        lua_remove(L, 1);
    }
    vec2_t a = *(vec2_t*)luaL_checkudata(L, 1, "vec2");
    luaL_argcheck(L, lua_isnumber(L, 2), 2, "'number' expected");
    float scale = lua_tonumber(L, 2);
    vec2_t nv = vec2_scale(a, scale);
    lua_getglobal(L, "vec2");
    lua_getfield(L, -1, "new");
    lua_pushnumber(L, nv.x);
    lua_pushnumber(L, nv.y);
    if (!lua_pcall(L, 2, 1, 0))
        return 1;
}

static int l_div(lua_State *L) {
    int swap = 0;
    if (lua_isnumber(L, 1)) {
        lua_pushvalue(L, 1);
        lua_remove(L, 1);
        swap = 1;
    }
    vec2_t a = *(vec2_t*)luaL_checkudata(L, 1, "vec2");
    luaL_argcheck(L, lua_isnumber(L, 2), 2, "'number' expected");
    float scale = 1.f / lua_tonumber(L, 2);
    vec2_t nv = vec2_scale(a, scale);
    if (swap) {
        nv.x = 1.f / nv.x;
        nv.y = 1.f / nv.y;
    }
    lua_getglobal(L, "vec2");
    lua_getfield(L, -1, "new");
    lua_pushnumber(L, nv.x);
    lua_pushnumber(L, nv.y);
    if (!lua_pcall(L, 2, 1, 0))
        return 1;
}

static int l_norm(lua_State *L) {
    vec2_t v = *(vec2_t*)luaL_checkudata(L, 1, "vec2");
    vec2_t nv = vec2_norm(v);
    lua_getglobal(L, "vec2");
    lua_getfield(L, -1, "new");
    lua_pushnumber(L, nv.x);
    lua_pushnumber(L, nv.y);
    if (!lua_pcall(L, 2, 1, 0))
        return 1;
}

static int l_length(lua_State *L) {
    vec2_t *v = luaL_checkudata(L, 1, "vec2");
    lua_pushnumber(L, vec2_length(*v));
    return 1;
}

static int l_distance(lua_State *L) {
    vec2_t a = *(vec2_t*)luaL_checkudata(L, 1, "vec2");
    luaL_argcheck(L, lua_isuserdata(L, 2), 2, "'vec2' expected");
    vec2_t b = *(vec2_t*)luaL_checkudata(L, 2, "vec2");
    lua_pushnumber(L, vec2_distance(a, b));
    return 1;
}

static int l_dot(lua_State *L) {
    vec2_t a = *(vec2_t*)luaL_checkudata(L, 1, "vec2");
    luaL_argcheck(L, lua_isuserdata(L, 2), 2, "'vec2' expected");
    vec2_t b = *(vec2_t*)luaL_checkudata(L, 2, "vec2");
    lua_pushnumber(L, vec2_dot(a, b));
    return 1;
}

static int l_cross(lua_State *L) {
    vec2_t a = *(vec2_t*)luaL_checkudata(L, 1, "vec2");
    luaL_argcheck(L, lua_isuserdata(L, 2), 2, "'vec2' expected");
    vec2_t b = *(vec2_t*)luaL_checkudata(L, 2, "vec2");
    lua_pushnumber(L, vec2_cross(a, b));
    return 1;
}

static int l_angle(lua_State *L) {
    vec2_t a = *(vec2_t*)luaL_checkudata(L, 1, "vec2");
    luaL_argcheck(L, lua_isuserdata(L, 2), 2, "'vec2' expected");
    vec2_t b = *(vec2_t*)luaL_checkudata(L, 2, "vec2");
    lua_pushnumber(L, vec2_angle(a, b));
    return 1;
}

static const struct luaL_Reg shared_funcs[] ={
    {"length", l_length},
    {"norm", l_norm},
    {"distance", l_distance},
    {"dot", l_dot},
    {"cross", l_cross},
    {"angle", l_angle},
    {NULL, NULL}
};

static const struct luaL_Reg lib_funcs[] = {
    {"new", l_new},
    {NULL, NULL}
};

static const struct luaL_Reg lib_metafuncs[] = {
    {"__call", l_call},
    {NULL, NULL}
};

static const struct luaL_Reg ud_metafuncs[] = {
    {"__index", l_index},
    {"__newindex", l_newindex},
    {"__len", l_length},

    {"__unm", l_unm},
    {"__add", l_add},
    {"__sub", l_sub},
    {"__mul", l_mul},
    {"__div", l_div},
    {NULL, NULL}
};

/**
 * @brief Initialize Lua vec2 library
 * @param L Lua state
 */
int luaopen_vec2(lua_State *L) {
    luaL_newmetatable(L, "vec2");

    luaL_setfuncs(L, ud_metafuncs, 0);
    luaL_setfuncs(L, shared_funcs, 0);

    luaL_newlib(L, lib_funcs);
    luaL_setfuncs(L, shared_funcs, 0);

    lua_newtable(L);
    luaL_setfuncs(L, lib_metafuncs, 0);
    lua_setmetatable(L, -2);

    return 1;
}

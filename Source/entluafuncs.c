/**
 * @file entluafuncs.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/28/18
 * Entity Lua Functions implementation
 */
#include "entluafuncs.h"

#include <lauxlib.h>
#include <lualib.h>

#include <Engine/objectmngr.h>
#include <Engine/transform.h>
#include <Engine/physics.h>

#include "luautil.h"
#include "luavec2.h"
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
 * @brief Move by vec2_t
 * @param 1 vec2_t to move by
 */
static int l_move(lua_State *L) {
    luaL_argcheck(L, lua_isuserdata(L, 1), 1, "'vec2' expected");
    Entity *ent = getEntity(L);
    vec2_t v = *(vec2_t*)luaL_checkudata(L, 1, "vec2");
    Transform *trs = Object_getComp(ent->comp.owner, TRANSFORM);
    if (trs) trs->pos = vec2_add(trs->pos, v);
    return 0;
}

/**
 * @brief Set acceleration
 * @param 1 vec2_t acceleration
 */
static int l_setaccel(lua_State *L) {
    luaL_argcheck(L, lua_isuserdata(L, 1), 1, "'vec2' expected");
    Entity *ent = getEntity(L);
    vec2_t a = *(vec2_t*)luaL_checkudata(L, 1, "vec2");
    Physics *phys = Object_getComp(ent->comp.owner, PHYSICS);
    if (phys) phys->accel = a;
    return 0;
}

/**
 * @brief Set velocity
 * @param 1 vec2_t velocity
 */
static int l_setvel(lua_State *L) {
    luaL_argcheck(L, lua_isuserdata(L, 1), 1, "'vec2' expected");
    Entity *ent = getEntity(L);
    vec2_t a = *(vec2_t*)luaL_checkudata(L, 1, "vec2");
    Physics *phys = Object_getComp(ent->comp.owner, PHYSICS);
    if (phys) phys->vel = a;
    return 0;
}

/**
 * @brief Get max velocity
 * @return Max velocity
 */
static int l_getmaxvel(lua_State *L) {
    Entity *ent = getEntity(L);
    Physics *phys = Object_getComp(ent->comp.owner, PHYSICS);
    if (phys) {
        lua_pushnumber(L, phys->maxVel);
        return 1;
    }
    return 0;
}

/**
 * @brief Get nearby Entity s
 * @return Table of nearby Entity s
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
        if (vec2_distance(trs->pos, objTrs->pos) < ent->detectRange) {
            lua_pushlightuserdata(L, objEnt);
            lua_rawseti(L, -2, ++count);
        }
    }
    return 1;
}

/**
 * @brief Get nearest Entity
 * @param 1 EntityType OR table of EntityType s (default: any)
 * @return Nearest Entity
 */
static int l_getnearest(lua_State *L) {
    Entity *ent = getEntity(L);
    Transform *trs = Object_getComp(ent->comp.owner, TRANSFORM);
    if (!trs)
        return 0;
    List *types = List_new(4, NULL);
    switch (lua_type(L, 1)) {
    case LUA_TNUMBER:
        List_push_back(types, (int)lua_tonumber(L, 1));
        break;
    case LUA_TTABLE:
        lua_len(L, 1);
        int len = lua_tonumber(L, -1);
        lua_pop(L, 1);
        for (int i = 1; i <= len; i++) {
            lua_geti(L, 1, i);
            List_push_back(types, (int)lua_tonumber(L, 1));
        }
        break;
    case LUA_TNONE:
        break;
    }
    List *objs = ent->comp.owner->objMngr->objs;
    float nearestDist = -1.f;
    Entity *nearest = NULL;
    for (unsigned i = 0; i < objs->size; i++) {
        Object *obj = objs->items[i];
        Entity *objEnt;
        Transform *objTrs;
        if (!(objEnt = Object_getComp(obj, ENTITY)) || objEnt == ent || !(objTrs = Object_getComp(obj, TRANSFORM)))
            continue;
        int found = (types->size == 0) ? 1 : 0;
        for (unsigned i = 0; !found && i < types->size; i++) {
            if (Entity_isType(objEnt, (EntityType)types->items[i])) {
                found = 1;
            }
        }
        if (!found)
            continue;
        float dist = vec2_distance(trs->pos, objTrs->pos);
        if (dist < ent->detectRange && (!nearest || dist < nearestDist)) {
            nearest = objEnt;
        }
    }
    if (nearest)
        lua_pushlightuserdata(L, nearest);
    else lua_pushnil(L);
    List_delete(types);
    return 1;
}

/**
 * @brief Get position of Entity
 * @param 1 Entity to get position of (default: self)
 * @return vec2_t position of Entity
 */
static int l_getpos(lua_State *L) {
    Entity *ent;
    if (lua_isnone(L, 1))
        ent = getEntity(L);
    else if (!(ent = lua_touserdata(L, 1)))
        luaL_argcheck(L, false, 1, "'Entity' or 'none' expected");
    Transform *trs;
    if (!ent || !(trs = Object_getComp(ent->comp.owner, TRANSFORM)))
        return 0;
    lua_getglobal(L, "vec2");
    lua_getfield(L, -1, "new");
    lua_pushnumber(L, trs->pos.x);
    lua_pushnumber(L, trs->pos.y);
    lua_call(L, 2, 1);
    return 1;
}

/**
 * @brief Get types of Entity
 * @param 1 Entity to get type of (default: self)
 * @return table of EntityType s of Entity
 */
static int l_gettypes(lua_State *L) {
    Entity *ent;
    if (lua_isnone(L, 1))
        ent = getEntity(L);
    else if (!(ent = lua_touserdata(L, 1)))
        luaL_argcheck(L, false, 1, "'Entity' or 'none' expected");
    lua_newtable(L);
    for (unsigned i = 0; i < ent->types->size; i++)
        lua_rawseti(L, -1, ent->types->items[i]);
    return 1;
}

/**
 * @brief Check if Entity is of a certain EntityType
 * @param 1 Entity to get type of (default: self)
 * @param 2 Type to check
 * @return If Entity has given EntityType
 */
static int l_istype(lua_State *L) {
    Entity *ent = NULL;
    int typeArg = 1;
    switch (lua_type(L, 1)) {
    case LUA_TNUMBER:
        ent = getEntity(L);
        break;
    case LUA_TUSERDATA:
        ent = lua_touserdata(L, 1);
        typeArg = 2;
        break;
    default:
        luaL_argcheck(L, false, 1, "'number' or 'Entity' expected");
    }
    luaL_argcheck(L, lua_isnumber(L, typeArg), typeArg, "'number' expected");
    int type = lua_tonumber(L, typeArg);
    lua_pushboolean(L, Entity_isType(ent, type));
    return 1;
}

/**
 * @brief List of EntityType names
 */
static const char *typenames[] = {
    "Player",
    "Enemy",
};

/**
 * @brief Get type name of type
 * @param 1 EntityType
 * @return Type name
 */
static int l_gettypename(lua_State *L) {
    luaL_argcheck(L, lua_tonumber(L, 1), 1, "'Entity', 'number', or 'none' expected");
    lua_pushfstring(L, typenames[(int)lua_tonumber(L, 1)]);
    return 1;
}

/**
 * @brief List of Entity Lua functions
 */
static const luaL_Reg funcs[] = {
    {"Move", l_move},
    {"SetAccel", l_setaccel},
    {"SetVel", l_setvel},

    {"GetMaxVel", l_getmaxvel},

    {"GetNearby", l_getnearby},
    {"GetNearest", l_getnearest},

    {"GetTypes", l_gettypes},
    {"IsType", l_istype},
    {"GetTypeName", l_gettypename},

    {"GetPos", l_getpos},
    {NULL, NULL}
};

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
    luaL_requiref(L, "vec2", luaopen_vec2, 1);
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

    lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
    luaL_setfuncs(L, funcs, 0);

    lua_pushnumber(L, ET_PLAYER);
    lua_setglobal(L, "ET_PLAYER");
    lua_pushnumber(L, ET_ENEMY);
    lua_setglobal(L, "ET_ENEMY");

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

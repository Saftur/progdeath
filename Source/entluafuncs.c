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
 * @brief Move Entity by vec2_t v
 * @param ent Entity to move
 * @param v   vec2_t to move by
 */
static void move(Entity *ent, vec2_t v) {
    Transform *trs = Object_getComp(ent->comp.owner, TRANSFORM);
    trs->pos = vec2_add(trs->pos, v);
}

/**
 * @brief Move by vec2_t
 * @param 1 vec2_t to move by
 */
static int l_move(lua_State *L) {
    Entity *ent = getEntity(L);
    luaL_argcheck(L, lua_isuserdata(L, 1), 1, "'vec2' expected");
    vec2_t v = *(vec2_t*)luaL_checkudata(L, 1, "vec2");
    move(ent, v);
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
    //int type = (lua_isnumber(L, 1)) ? (lua_tonumber(L, 1)) : -1;
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
            if (objEnt->type == types->items[i]) {
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
 * @param vec2_t position of Entity
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
    //lua_newtable(L);
    lua_pushnumber(L, trs->pos.x);
    //lua_rawseti(L, -2, 1);
    lua_pushnumber(L, trs->pos.y);
    //lua_rawseti(L, -2, 2);
    lua_call(L, 2, 1);
    return 1;
}

/**
 * @brief Get type of Entity
 * @param 1 Entity to get type of (default: self)
 * @param EntityType of Entity
 */
static int l_gettype(lua_State *L) {
    Entity *ent;
    if (lua_isnone(L, 1))
        ent = getEntity(L);
    else if (!(ent = lua_touserdata(L, 1)))
        luaL_argcheck(L, false, 1, "'Entity' or 'none' expected");
    lua_pushnumber(L, ent->type);
    return 1;
}

static const char *typenames[] ={
    "Player",
    "Enemy",
};

/**
 * @brief Get type name of type OR Entity
 * @param 1 Type OR Entity to get type name of (default: self)
 * @return Type name
 */
static int l_gettypename(lua_State *L) {
    Entity *ent;
    int type;
    if (lua_isnone(L, 1))
        type = getEntity(L)->type;
    else if (lua_isnumber(L, 1))
        type = lua_tonumber(L, 1);
    else if (ent = lua_touserdata(L, 1))
        type = ent->type;
    else
        luaL_argcheck(L, false, 1, "'Entity', 'number', or 'none' expected");
    lua_pushfstring(L, typenames[type]);
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

    lua_pushcfunction(L, l_move);
    lua_setglobal(L, "Move");

    lua_pushcfunction(L, l_getnearby);
    lua_setglobal(L, "GetNearby");

    lua_pushcfunction(L, l_getnearest);
    lua_setglobal(L, "GetNearest");

    lua_pushcfunction(L, l_gettype);
    lua_setglobal(L, "GetType");

    lua_pushcfunction(L, l_gettypename);
    lua_setglobal(L, "GetTypeName");

    lua_pushcfunction(L, l_getpos);
    lua_setglobal(L, "GetPos");

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

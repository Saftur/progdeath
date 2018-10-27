/**
 * @file luafuncs.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/27/18
 * Lua Functions implementation
 */
#include "luafuncs.h"

#include <string.h>

#include <lauxlib.h>
#include <C_Processing.h>

/**
 * @brief Register keys
 * @param L Lua state
 */
static void register_keys(lua_State *L) {
    char keyName[20];

    strcpy(keyName, "KEY_A");
    for (unsigned k = KEY_A; k <= KEY_Z; k++, keyName[4]++) {
        lua_pushnumber(L, k);
        lua_setglobal(L, keyName);
    }

    strcpy(keyName, "KEY_0");
    for (unsigned k = KEY_0; k <= KEY_9; k++, keyName[4]++) {
        lua_pushnumber(L, k);
        lua_setglobal(L, keyName);
    }
}

/**
 * @brief Check if key down
 * @param 1 Key to check
 * @return If key is down
 */
static int l_keydown(lua_State *L) {
    int k = luaL_checkinteger(L, 1);
    lua_pushboolean(L, keyIsDown(k));
    return 1;
}

/**
 * @brief Register Lua functions
 * @param L Lua state
 */
void register_luafuncs(lua_State *L) {
    register_keys(L);

    lua_pushcfunction(L, l_keydown);
    lua_setglobal(L, "KeyDown");
}

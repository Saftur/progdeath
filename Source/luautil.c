/**
 * @file luautil.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/27/18
 * Lua Utilities implementation
 */
#include "luautil.h"

#include <lauxlib.h>

/**
 * @brief Default Lua panic function
 * @param 1 Error object
 */
static int l_defaultpanic(lua_State *L) {
    fprintf(stderr, "%s\n", luaL_checkstring(L, 1));
    lua_close(L);
    exit(EXIT_FAILURE);
}

/**
 * @brief Set Lua panic function to default
 * @param L Lua state
 */
void luaU_setdefaultpanic(lua_State *L) {
    luaU_setpanic(L, l_defaultpanic);
}

/**
 * @brief Set Lua panic function
 * @param L Lua state
 * @param f Panic function
 */
void luaU_setpanic(lua_State *L, lua_CFunction f) {
    lua_atpanic(L, f);
    lua_pushstring(L, "PANIC");
    lua_pushcfunction(L, f);
    lua_settable(L, LUA_REGISTRYINDEX);
}

/**
 * @brief Print a Lua error
 * @param L   Lua state
 * @param fmt Error format string
 * @param ... Things to print
 * @return EXIT_FAILURE
 */
int luaU_error(lua_State *L, const char *fmt, ...) {
    va_list argp;
    va_start(argp, fmt);
    lua_pushstring(L, "PANIC");
    lua_gettable(L, LUA_REGISTRYINDEX);
    lua_pushvfstring(L, fmt, argp);
    lua_pcall(L, 1, 0, 0);
    va_end(argp);
    return EXIT_FAILURE;
}

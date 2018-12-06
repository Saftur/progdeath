/**
 * @file luafuncs.h
 * @author Arthur Bouvier (a.bouvier)
 * Lua Functions
 */
#pragma once

#include <lua.h>

/**
 * @brief Register Lua functions
 * @param L Lua state
 */
void register_luafuncs(lua_State *L);

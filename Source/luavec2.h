/**
 * @file luavec2.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/27/18
 * Lua userdata for vec2_t
 */
#pragma once

#include <lua.h>

/**
 * @brief Initialize Lua vec2 library
 * @param L Lua state
 */
int luaopen_vec2(lua_State *L);

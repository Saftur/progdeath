/**
 * @file luavector2d.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/27/18
 * Lua userdata for Vector2D
 */
#pragma once

#include <lua.h>

/**
 * @brief Initialize Lua Vector2D library
 * @param L Lua state
 */
int luaopen_Vector2D(lua_State *L);

/**
 * @file luavec2.h
 * @author Arthur Bouvier (a.bouvier)
 * @brief Lua userdata for vec2_t
 * @course GAM100F18
 * Copyright © 2018 DigiPen (USA) Corporation
 */
#pragma once

#include <lua.h>

/**
 * @brief Initialize Lua vec2 library
 * @param L Lua state
 */
int luaopen_vec2(lua_State *L);

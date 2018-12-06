/**
 * @file luafuncs.h
 * @author Arthur Bouvier (a.bouvier)
 * @brief Lua Functions
 * @course GAM100F18
 * Copyright © 2018 DigiPen (USA) Corporation
 */
#pragma once

#include <lua.h>

/**
 * @brief Register Lua functions
 * @param L Lua state
 */
void register_luafuncs(lua_State *L);

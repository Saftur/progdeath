/**
 * @file luautil.h
 * @author Arthur Bouvier (a.bouvier)
 * @brief Lua Utilities
 * @course GAM100F18
 * Copyright � 2018 DigiPen (USA) Corporation
 */
#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <lua.h>

/**
 * @brief Set Lua panic function to default
 * @param L Lua state
 */
void luaU_setdefaultpanic(lua_State *L);
/**
 * @brief Set Lua panic function
 * @param L Lua state
 * @param f Panic function
 */
void luaU_setpanic(lua_State *L, lua_CFunction f);

/**
 * @brief Print a Lua error
 * @param L   Lua state
 * @param fmt Error format string
 * @param ... Things to print
 */
int luaU_error(lua_State *L, const char *fmt, ...);

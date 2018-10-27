/**
 * @file luautil.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/26/18
 * Lua Utilities
 */
#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <lua.h>

/**
 * @brief Print a Lua error
 * @param L   Lua state
 * @param fmt Error format string
 * @param ... Things to print
 */
void luaU_error(lua_State *L, const char *fmt, ...);
/**
 * @brief Call a Lua function
 * @param L    Lua state
 * @param func Lua function name
 * @param sig  Function signature.
 *             ie. "dd>d" means takes two doubles, returns a double.
 *             Specifiers are d->double, i->int, and s->string
 * @param ...  Arguments to pass in, and pointers to store results in
 */
void luaU_call(lua_State *L, const char *func, const char *sig, ...);

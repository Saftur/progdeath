/**
 * @file entluafuncs.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/27/18
 * Entity Lua Functions
 */
#pragma once

#include <lua.h>

typedef struct Entity Entity;

lua_State *initEntityLuaState(Entity *ent, const char *scriptName);

void setCountHook(lua_State *L);

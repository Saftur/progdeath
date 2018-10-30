/**
 * @file entluafuncs.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/27/18
 * Entity Lua Functions
 */
#pragma once

#include <lua.h>

#include "entity.h"

/**
 * @brief Initialize Entity Lua script state
 * @param ent        Entity whose script to initialize
 * @param script     Lua script code or filename
 * @param scriptType Lua script type (code or filename)
 */
void initEntityLuaState(Entity *ent, const char *script, ScriptType scriptType);

/**
 * @brief Reset the timeout Lua hook
 * @param L Lua state
 */
void resetTimeoutHook(lua_State *L);

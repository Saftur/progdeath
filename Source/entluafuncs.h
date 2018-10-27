/**
 * @file entluafuncs.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/27/18
 * Entity Lua Functions
 */
#pragma once

#include <lua.h>

typedef struct Entity Entity; ///< @brief Entity forward declaration

/**
 * @brief Initialize Entity Lua script state
 * @param ent        Entity whose script to initialize
 * @param scriptName Name of Lua script to use
 */
void initEntityLuaState(Entity *ent, const char *scriptName);

/**
 * @brief Reset the timeout Lua hook
 * @param L Lua state
 */
void resetTimeoutHook(lua_State *L);

/**
 * @file entity.h
 * @author Name (username)
 * @date 10/26/18
 * @brief Entity Component
 * @addtogroup Components
 * @{
 */
#pragma once

#include <Engine/component.h>
#include "gamecomps.h"

#include <lua.h>

typedef enum EntityType {
    ET_PLAYER,
    ET_ENEMY,
} EntityType;

/**
 * @brief Entity Component
 */
typedef struct Entity {
    Component comp; ///< @brief Component data

    lua_State *script; ///< @brief Entity Lua script
    EntityType type;   ///< @brief Entity type

    float detectRange; ///< @brief Entity detection range
} Entity;

/**
 * @brief Create new Entity
 * @param scriptName Lua script name
 * @param type       Entity type
 * @return New Entity
 */
Entity *Entity_new(const char *scriptName, EntityType type);

/**
 * @brief Delete Entity
 * @param this Entity to delete
 */
void _Entity_delete(Entity *this);

/**
 * @brief Update Entity
 * @param this Entity to update
 */
void _Entity_update(Entity *this);
/**
 * @brief Draw Entity
 * @param this Entity to draw
 */
void _Entity_draw(Entity *this);

/**
 * @brief Resolve collisions with other Component s
 * @param this  This Component
 * @param other Other Component
 */
void _Entity_coll_resolve(Entity *this, Component *other);

/// @}

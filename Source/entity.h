/**
 * @file entity.h
 * @author Name (username)
 * @date 10/28/18
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
    List *types;       ///< @brief List of Entity types

    float detectRange; ///< @brief Entity detection range
} Entity;

/**
 * @brief Create new Entity
 * @param scriptName Lua script name
 * @param baseType   Entity base type
 * @return New Entity
 */
Entity *Entity_new(const char *scriptName, EntityType baseType);

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

/**
 * @brief Add type to this Entity 's type list
 * @param this Entity to add to
 * @param type EntityType to add
 */
void Entity_addType(Entity *this, EntityType type);
/**
 * @brief Check if Entity is of given EntityType
 * @param this Entity to check
 * @param type EntityType to check
 * @return If this is of given type
 */
int Entity_isType(Entity *this, EntityType type);

/// @}

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

#include "entactions.h"

#define INVENTORY_SIZE 5 //Maximum amount of items the players can hold (not including main hand and offhand)

#define ENTITY_RADIUS 20

typedef struct Entity Entity;

typedef enum EntityType {
    ENT_PLAYER,
    ENT_ENEMY,

    ENT_ENV,
    ENT_GRASS,
    ENT_MOUNTAIN,
    ENT_FIRE,
    ENT_WATER,
    ENT_TREE,

    ENT_ITEM,
    ENT_APPLE,
} EntityType;

typedef enum ScriptType {
    ST_NONE,
    ST_FILENAME,
    ST_CODE,
} ScriptType;

typedef struct Entity Entity; ///< @brief Entity forward declaration



typedef struct Inventory {
    //TODO change entity to item
    Entity *mainHand, *offHand, *stored[INVENTORY_SIZE];
} Inventory;

/**
 * @brief Entity Component
 */
typedef struct Entity {
    Component comp; ///< @brief Component data

    const char *scriptStr; ///< @brief Lua script code or filename
    ScriptType scriptType; ///< @brief Lua script type (code or filename)
    lua_State *script;     ///< @brief Entity Lua script
    List *types;           ///< @brief List of Entity types

    float detectRange; ///< @brief Entity detection range
    Entity *actualEnt; ///< @brief Pointer to actual Entity on Entity lua clones

    float hp;	 ///< @brief Current Health 
    float maxHp; ///< @brief Maximum Health 

    Inventory inventory; ///< @brief Entity's stored items

    EntAction currAction; ///< @brief the current action the entity is in
    List *actionData;     ///< @brief holds date for the current action

    double incapacitated; ///< @brief if second() < incapacitated the entity script will not be called
    double actionDelay;   ///< @brief time before switching back to normal state after using an action
    double actionStartup; ///< @brief time before the action takes place after calling it

    float radius; ///< @brief radius of the entity (for drawing)
} Entity;

/**
 * @brief Create new Entity
 * @param script     Lua script code or filename
 * @param scriptType Lua script type (code or filename)
 * @param baseType   Entity base type
 * @param maxHp	     Maximum Health
 * @param radius     radius of the entity
 * @return New Entity
 */
Entity *Entity_new(const char *script, ScriptType scriptType, EntityType baseType, float maxHp, float radius);

/**
 * @brief Clone Entity
 * @param this Entity to clone
 * @return Cloned Entity
 */
Entity *_Entity_clone(Entity *this);

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

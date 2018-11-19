/**
 * @file entity.c
 * @author Name (username)
 * @date 10/28/18
 * @brief Entity implementation
 * @addtogroup Components
 * @{
 */
#include "entity.h"

#include <lauxlib.h>
#include <lualib.h>

#include <Engine/transform.h>

#include "luautil.h"
#include "luafuncs.h"
#include "entluafuncs.h"
#include "map.h"


 /**
  * @brief Create new Entity
  * @param script     Lua script code or filename
  * @param scriptType Lua script type (code or filename)
  * @param baseType   Entity base type
  * @param maxHp	     Maximum Health
  * @param radius     radius of the entity
  * @return New Entity
  */
Entity *Entity_new(const char *script, ScriptType scriptType, EntityType baseType, float maxHp, float radius){
    Entity *this = malloc(sizeof(Entity));
    this->comp.typeName = "Entity";
    this->comp.typeId = ENTITY;
    this->comp.clone = _Entity_clone;
    this->comp.delete = _Entity_delete;
    this->comp.update = _Entity_update;
    this->comp.draw = _Entity_draw;
    this->comp.collides = true;
    this->comp.coll_resolve = _Entity_coll_resolve;
    this->comp.owner = NULL;

    this->script = NULL;
    this->scriptStr = script;
    this->scriptType = scriptType;
    initEntityLuaState(this, script, scriptType);
    if (this->scriptType != ST_NONE) {
        if (!this->script) {
            _Entity_delete(this);
            return NULL;
        }
        register_luafuncs(this->script);
    }

    this->types = List_new(10, NULL, NULL);
    List_push_back(this->types, (void*)baseType);

    this->detectRange = 3000.f;
    this->actualEnt = this;

    this->hp = maxHp;
    this->maxHp = maxHp;

    this->currAction = EA_NONE;
    this->actionData = List_new(3, NULL, free);

    this->incapacitated = 0.0;
    this->actionDelay = 0.0;
    this->actionStartup = 0.0;

    this->radius = radius;
    if (maxHp == 0)
        this->invincible = 1;
    else
        this->invincible = 0;

    return this;
}

/**
 * @brief Clone Entity
 * @param this Entity to clone
 * @return Cloned Entity
 */
Entity *_Entity_clone(Entity *this) {
    Entity *new = malloc(sizeof(Entity));

    initEntityLuaState(new, this->script, this->scriptType);
    if (!new->script) {
        _Entity_delete(new);
        return NULL;
    }
    register_luafuncs(new->script);

    new->types = List_copy(this->types);

    new->detectRange = this->detectRange;

    return new;
}

/**
 * @brief Delete Entity
 * @param this Entity to delete
 */
void _Entity_delete(Entity *this) {
    if (this->script)
        lua_close(this->script);
    List_delete(this->types);
    free(this);
}

/**
 * @brief Update Entity
 * @param this Entity to update
 */
void _Entity_update(Entity *this) {
    if (this->script) {
        resetTimeoutHook(this->script);

        if (this->incapacitated > 0)
            this->incapacitated -= dt();
        if (this->incapacitated <= 0) {
            lua_getglobal(this->script, "update");
            lua_call(this->script, 0, 0);
        }
    }

    Transform *trs;
    if (List_find(this->types, ENT_PLAYER, NULL) && (trs = Object_getComp(this->comp.owner, TRANSFORM))) {
        translate(canvasWidth / 2 - trs->pos.x, canvasHeight / 2 - trs->pos.y);
    }

    if (this->actionDelay > 0)
    {
        this->actionDelay -= dt();
        if (this->actionDelay <= 0)
            this->currAction = EA_NONE;
    }

    if (this->actionStartup > 0)
        this->actionStartup -= dt();
    if (this->actionStartup <= 0 && this->actionDelay <= 0)
        entActionFuncs[this->currAction](this);

    if (this->hp <= 0.0)
        Object_destroy(this->comp.owner);
}

/**
 * @brief Draw Entity
 * @param this Entity to draw
 */
void _Entity_draw(Entity *this) {

    Transform *trs = Object_getComp(this->comp.owner, TRANSFORM);
    if (!trs) return;
    noStroke();
    for (unsigned i = 0; i < this->types->size; i++)
    {
        switch ((EntityType)(this->types->items[i]))
        {
        case ENT_APPLE:
            fill(215,30,0,255);
            circle(trs->pos.x, trs->pos.y, this->radius / 2);
            break;
        case ENT_TREE:
            fill(envObjs[ENV_TREE].color.r, envObjs[ENV_TREE].color.g, envObjs[ENV_TREE].color.b, envObjs[ENV_TREE].color.a);
            circle(trs->pos.x, trs->pos.y, this->radius);
            break;
        case ENT_ITEM:
            break;
        default:
            fill(0, 0, 0, 255);
            circle(trs->pos.x, trs->pos.y, this->radius);
        }
    }
}

/**
 * @brief Resolve collisions with other Component s
 * @param this  This Component
 * @param other Other Component
 */
void _Entity_coll_resolve(Entity *this, Component *other) {

}

/**
 * @brief Add type to this Entity 's type list
 * @param this Entity to add to
 * @param type EntityType to add
 */
void Entity_addType(Entity *this, EntityType type) {
    if (!Entity_isType(this, type))
        List_push_back(this->types, (void*)type);
}

/**
 * @brief Check if Entity is of given EntityType
 * @param this Entity to check
 * @param type EntityType to check
 * @return If this is of given type
 */
int Entity_isType(Entity *this, EntityType type) {
    return List_find(this->types, (void*)type, NULL);
}

/// @}


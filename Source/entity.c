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
#include <Engine/gamelayer.h>

#include "luautil.h"
#include "luafuncs.h"
#include "entluafuncs.h"
#include "map.h"
#include "mapsize.h"
#include "Engine/screenmngr.h"
#include "deathscreen.h"

static vec2_t camPos = { 0,0 };

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

    this->direction = 0;

    this->radius = radius;
    if (maxHp == 0)
        this->invincible = 1;
    else
        this->invincible = 0;

    this->equipment = NULL;
    this->equipOffset = (vec2_t){ this->radius, 0 };
    this->equipRotate = 0;


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
    Transform *trs = Object_getComp(this->comp.owner, TRANSFORM);

    if (this->equipment) {
        this->equipment->direction = this->direction;
        Transform *eqTrs = Object_getComp(this->equipment->comp.owner, TRANSFORM);
        eqTrs->pos = vec2_add(trs->pos, mat3_mult_vec2(mat3_rotate(-(this->direction + this->equipRotate)), this->equipOffset)/*vec2_scale((vec2_t){ cos(radians(this->direction)), -sin(radians(this->direction)) }, this->radius)*/);
    }

    if (this->script) {
        resetTimeoutHook(this->script);

        if (this->incapacitated > 0)
            this->incapacitated -= dt();
        if (this->incapacitated <= 0) {
            lua_getglobal(this->script, "update");
            lua_call(this->script, 0, 0);
        }
    }

    if (trs->pos.x - this->radius  < 0)
        trs->pos.x = this->radius;

    if (trs->pos.y - this->radius < 0)
        trs->pos.y = this->radius;

    if (trs->pos.x + this->radius > MAP_TILE_SIZE * getMapWidth())
        trs->pos.x = MAP_TILE_SIZE * getMapWidth() - this->radius;

    if (trs->pos.y + this->radius > MAP_TILE_SIZE * getMapHeight())
        trs->pos.y = MAP_TILE_SIZE * getMapHeight() - this->radius;

    if (Entity_isType(this, ENT_PLAYER)) 
    {   
        vec2_t target = trs->pos;

        if (target.x - canvasWidth / 2 < 0)
            target.x = canvasWidth / 2;
        else if (target.x + canvasWidth / 2 > MAP_TILE_SIZE * getMapWidth())
            target.x = MAP_TILE_SIZE * getMapWidth() - canvasWidth / 2;
        if (target.y - canvasHeight / 2 < 0)
            target.y = canvasHeight / 2;
        else if (target.y + canvasHeight / 2 > MAP_TILE_SIZE * getMapHeight())
            target.y = MAP_TILE_SIZE * getMapHeight() - canvasHeight / 2;


        camPos.x = lerp_i(camPos.x, target.x, 0.1);
        camPos.y = lerp_i(camPos.y, target.y, 0.1);
        translate(canvasWidth / 2 - camPos.x, canvasHeight / 2 - camPos.y);
    }

    if (this->actionDelay > 0)
    {
        this->actionDelay -= dt();
        if (this->actionDelay <= 0)
        {
            List_clear(this->actionData);
            this->currAction = EA_NONE;
            this->invincible = 0;
        }
    }

    if (this->actionStartup > 0)
        this->actionStartup -= dt();
    if (this->actionStartup <= 0 && this->actionDelay <= 0)
        entActionFuncs[this->currAction](this);

    if (!this->invincible && this->hp <= 0.0)
    {
        if (Entity_isType(this, ENT_PLAYER))
        {
            has_won = 0;
            ScreenMngr_setNextScreen(this->comp.owner->objMngr->gLayer->scrMngr, "Death Screen");
        }
        else
        {
            Object_destroy(this->comp.owner);
            List *objs = this->comp.owner->objMngr->objs;
            int count = 0;
            for (unsigned i = 0; i < objs->size; i++) {
                Object *obj = objs->items[i];
                Entity *objEnt;
                if (!(objEnt = Object_getComp(obj, ENTITY)) || objEnt == this)
                    continue;
                if (Entity_isType(objEnt, ENT_ENEMY))
                    count++;
            }
            if (!count)
            {
                has_won = 1;
                ScreenMngr_setNextScreen(this->comp.owner->objMngr->gLayer->scrMngr, "Death Screen");
            }
        }
    }
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
        case ENT_SPEAR:
            fill(0, 0, 0, 255);
            float w = this->radius, h = this->radius / 16;
            rectRotated(trs->pos.x - w / 2, trs->pos.y - h / 2, w, h, -this->direction);
            break;
        case ENT_APPLE:
            fill(215,30,0,255);
            circle(trs->pos.x, trs->pos.y, this->radius);
            break;
        case ENT_TREE:
            fill(envObjs[ENV_TREE].color.r, envObjs[ENV_TREE].color.g, envObjs[ENV_TREE].color.b, envObjs[ENV_TREE].color.a);
            circle(trs->pos.x, trs->pos.y, this->radius);
            break;
        case ENT_FIRE:
            fill(226, 88, 34, 255);
            circle(trs->pos.x, trs->pos.y, this->radius);
            break;
        case ENT_MOUNTAIN:
            fill(139, 141, 122, 255);
            circle(trs->pos.x, trs->pos.y, this->radius);
            break;
        case ENT_WATER:
            fill(79, 66, 181, 255);
            circle(trs->pos.x, trs->pos.y, this->radius);
            break;
        case ENT_ITEM: case ENT_EQUIPMENT: case ENT_ENV:
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


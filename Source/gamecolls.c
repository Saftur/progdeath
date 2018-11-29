/**
 * @file gamecolls.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/18/18
 * @brief Game colliders implementation
 * @addtogroup Game-Components
 * @{
 */
#include "gamecolls.h"

#include <Engine/engine.h>
#include <Engine/transform.h>
#include <Engine/physics.h>
#include <Engine/initcollcomps.h>
#include <Engine/util.h>

#include "entity.h"
#include <stdio.h>
#include "../apples.h"

/* Collision Component sorting example:
static bool CollCheck_Entity_Wall(Component *comp1, Component *comp2) {
    Entity *ent;
    Wall *wall;
    INITCOLLCOMPS(ENTITY, entity, wall, comp1, comp2);
    //Check here
}
*/

static bool CollCheck_Entity_Entity(Component *comp1, Component *comp2) {
    Entity *ent1 = comp1;
    Entity *ent2 = comp2;

    if (ent1->equipment == ent2 || ent2->equipment == ent1)
        return 0;

    Transform *trs1 = Object_getComp(ent1->comp.owner, TRANSFORM);
    Transform *trs2 = Object_getComp(ent2->comp.owner, TRANSFORM);

    return vec2_distance(trs1->pos, trs2->pos) < ENTITY_RADIUS * 2;
}

static void CollResolve_Entity_Entity(Component *comp1, Component *comp2) {
    Entity *ent1 = comp1;
    Entity *ent2 = comp2;

    if (Entity_isType(ent1, ENT_EQUIPMENT) || Entity_isType(ent2, ENT_EQUIPMENT))
        return;


    if(Entity_isType(ent1, ENT_ENV) || Entity_isType(ent2, ENT_ENV))
    {
        Entity *character = ent2;
        Entity *env = ent1;
        if (Entity_isType(ent1, ENT_PLAYER) || Entity_isType(ent1, ENT_ENEMY))
        {
            character = ent1;
            env = ent2;
        }
        
        if(Entity_isType(env, ENT_FIRE))
            character->hp -= 100;
        else if(Entity_isType(env, ENT_TREE))
        {
            env->hp -= 100;
            Transform *envTrs = Object_getComp(env->comp.owner, TRANSFORM);
            spawnApple(envTrs->pos, env->comp.owner->objMngr);
        }

        return;
    }

    Transform *trs1 = Object_getComp(ent1->comp.owner, TRANSFORM);
    Transform *trs2 = Object_getComp(ent2->comp.owner, TRANSFORM);

    vec2_t *p1 = &trs1->pos;
    vec2_t *p2 = &trs2->pos;

    vec2_t half_vec_1to2 = vec2_scale(vec2_sub(*p2, *p1), 0.5f);
    float half_vec_len = vec2_length(half_vec_1to2);
    float dist = half_vec_len * 2;
    vec2_t mid = vec2_add(*p1, half_vec_1to2);
    vec2_t radius_vec = vec2_scale(half_vec_1to2, (float)ENTITY_RADIUS / half_vec_len);

    *p1 = vec2_sub(mid, radius_vec);
    *p2 = vec2_add(mid, radius_vec);
}

/**
 * @brief List game colliders
 */
void listColls() {
    Engine_addCollCheckFunc(ENTITY, ENTITY, CollCheck_Entity_Entity);
    Engine_addCollResolveFunc(ENTITY, ENTITY, CollResolve_Entity_Entity);
}

/// @}

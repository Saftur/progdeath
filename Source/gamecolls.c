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
#include "apples.h"

static bool CollCheck_Entity_Entity(Component *comp1, Component *comp2) {
    Entity *ent1 = comp1;
    Entity *ent2 = comp2;

    if (Entity_isType(ent1, ENT_EQUIPMENT)) {
        Entity *tmp = ent1;
        ent1 = ent2;
        ent2 = tmp;
    }

    if (ent1->equipment == ent2 || ent2->equipment == ent1)
        return 0;

    Transform *trs1 = Object_getComp(ent1->comp.owner, TRANSFORM);
    Transform *trs2 = Object_getComp(ent2->comp.owner, TRANSFORM);
    vec2_t p1 = trs1->pos;
    vec2_t p2 = trs2->pos;

    if (Entity_isType(ent2, ENT_EQUIPMENT)) {
        vec2_t d = vec2_scale(vec2_norm(vec2_sub(p2, p1)), ent1->radius);
        vec2_t p = vec2_add(p1, d);
        p = vec2_sub(p, p2);
        p = mat3_mult_vec2(mat3_rotate(-ent2->direction), p);
        vec2_t eqSize = (vec2_t){ ent2->radius, ent2->radius / 16 };
        return vec2_in_range(vec2_add(p, vec2_scale(eqSize, 0.5)), vec2_zero(), eqSize);
    }

    return vec2_distance(p1, p2) < ent1->radius + ent2->radius;
}

static void CollResolve_Entity_Entity(Component *comp1, Component *comp2) {
    Entity *ent1 = comp1;
    Entity *ent2 = comp2;

    /*if (Entity_isType(ent1, ENT_EQUIPMENT) || Entity_isType(ent2, ENT_EQUIPMENT))
        return;*/

    if (ent1->types->items[0] > ent2->types->items[0]) {
        Entity *tmp = ent1;
        ent1 = ent2;
        ent2 = tmp;
    }

    enum {
        PUSH_ENT1,
        PUSH_ENT2,
        PUSH_BOTH
    } pushType = PUSH_BOTH;

    if (Entity_isType(ent1, ENT_FIGHTER)) {
        if (Entity_isType(ent2, ENT_EQUIPMENT)) {
            if (ent2->currAction == EA_WPN_ATTACK && !ent1->invincible)
                ent1->hp -= 40;
            return;
        }
        else if (Entity_isType(ent2, ENT_FIRE) && !ent1->invincible) {
            ent1->hp -= 5;
            return;
        }
        else if (Entity_isType(ent2, ENT_WATER) || Entity_isType(ent2, ENT_TREE)){
            return;
        } else if (Entity_isType(ent2, ENT_ENV))
            pushType = PUSH_ENT1;
        else if (Entity_isType(ent2, ENT_APPLE)) {
            ent1->hp = min(ent1->maxHp, ent1->hp + 40);
            ent2->hp -= 200;
            return;
        }
    } else if (Entity_isType(ent1, ENT_EQUIPMENT)) {
        if (ent1->currAction == EA_WPN_ATTACK) {
            if (Entity_isType(ent2, ENT_TREE)) {
                ent2->hp -= 100;
                Transform *envTrs = Object_getComp(ent2->comp.owner, TRANSFORM);
                spawnApple(envTrs->pos, ent2->comp.owner->objMngr);
            }
        }
        return;
    }


    Transform *trs1 = Object_getComp(ent1->comp.owner, TRANSFORM);
    Transform *trs2 = Object_getComp(ent2->comp.owner, TRANSFORM);

    vec2_t *p1 = &trs1->pos;
    vec2_t *p2 = &trs2->pos;

    vec2_t p1_to_p2 = vec2_sub(*p2, *p1);
    float p1_to_p2_len = vec2_length(p1_to_p2);
    p1_to_p2 = vec2_norm(p1_to_p2);
    float dist_inside = ent1->radius + ent2->radius - p1_to_p2_len;
    vec2_t p1_push = vec2_scale(p1_to_p2, -dist_inside / 2);
    vec2_t p2_push = vec2_scale(p1_to_p2, dist_inside / 2);
    switch (pushType) {
    case PUSH_ENT1:
        *p1 = vec2_add(*p1, vec2_sub(p1_push, p2_push));
        break;
    case PUSH_ENT2:
        *p2 = vec2_add(*p2, vec2_sub(p2_push, p1_push));
        break;
    case PUSH_BOTH:
        *p1 = vec2_add(*p1, p1_push);
        *p2 = vec2_add(*p2, p2_push);
        break;
    }
}

/**
 * @brief List game colliders
 */
void listColls() {
    Engine_addCollCheckFunc(ENTITY, ENTITY, CollCheck_Entity_Entity);
    Engine_addCollResolveFunc(ENTITY, ENTITY, CollResolve_Entity_Entity);
}

/// @}

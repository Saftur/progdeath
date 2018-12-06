/**
 * @file entactions.c
 * @author Connor Meyers (connor.meyers)
 * @course GAM100F18
 * Copyright © 2018 DigiPen (USA) Corporation
 */
#include "entactions.h"

#include <Engine/physics.h>
#include <Engine/transform.h>

#include "entity.h"

static void none(Entity* ent){}

#define TIME (*(float*)ent->actionData->items[0])
static void attack(Entity* ent)
{
    //TODO
    if (ent->actionData->size == 0) {
        float *time = malloc(sizeof(float));
        *time = 0.25f;
        List_push_back(ent->actionData, time);
        ent->equipment->currAction = EA_WPN_ATTACK;
        ent->equipOffset.x += 20;
    }
    TIME -= dt();
    if (TIME <= 0) {
        ent->equipment->currAction = EA_NONE;
        ent->actionDelay = 1.f;
        ent->equipOffset.x -= 20;
    }
}
#undef TIME

#define DELAY 0.5
static void block(Entity* ent)
{
    ent->invincible = 1;
    ent->incapacitated = DELAY;
    ent->actionDelay = DELAY;
}
#undef DELAY

#define RANGE (ent->types->items[0] == ENT_PLAYER ? 60 : 50)
#define VELOCITY (ent->types->items[0] == ENT_PLAYER ? 800 : 600)
#define INCAPACITATE (ent->types->items[0] == ENT_PLAYER ? 1.125 : 0.5)
#define DELAY (ent->types->items[0] == ENT_PLAYER ? 0.5 : 1.5)
static void throw(Entity* ent) 
{
    Entity *target = *(Entity**)ent->actionData->items[0];

    Transform *tarTrs = Object_getComp(target->comp.owner, TRANSFORM);
    Transform *entTrs = Object_getComp(ent->comp.owner, TRANSFORM);

    if (!tarTrs || !entTrs) return;

    if (vec2_distance(tarTrs->pos, entTrs->pos) > RANGE) {
        ent->actionDelay = DELAY;
        return;
    }

    float direction = *(float*)ent->actionData->items[1];

    Physics *physics = Object_getComp(target->comp.owner, PHYSICS);

    if (!physics) return;

    physics->vel = vec2_scale((vec2_t) { cosf(radians(direction)), -sinf(radians(direction)) }, VELOCITY);

    target->incapacitated = INCAPACITATE;
    ent->actionDelay = DELAY;
}
#undef RANGE
#undef VELOCITY
#undef INCAPACITATE
#undef COOLDOWN


void(*entActionFuncs[])(Entity*) = 
{
    none,
    attack,
    block,
    throw,

    none,
};
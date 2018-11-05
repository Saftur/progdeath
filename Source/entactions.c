#include "entactions.h"

#include <Engine/physics.h>
#include <Engine/transform.h>

#include "entity.h"

static void none(Entity* ent){}

static void attack(Entity* ent)
{

}

static void block(Entity* ent) {}

static void throw(Entity* ent) 
{
#define RANGE 100
#define VELOCITY 1000
    Entity *target = *(Entity**)ent->actionData->items[0];

    Transform *tarTrs = Object_getComp(target->comp.owner, TRANSFORM);
    Transform *entTrs = Object_getComp(ent->comp.owner, TRANSFORM);

    if (!tarTrs || !entTrs) return;

    if (vec2_distance(tarTrs->pos, entTrs->pos) > RANGE) return;

    float direction = *(float*)ent->actionData->items[1];

    Physics *physics = Object_getComp(target->comp.owner, PHYSICS);

    if (!physics) return;

    physics->vel = vec2_scale((vec2_t) {cos(direction), sin(direction)}, VELOCITY);

#undef RANGE
#undef VELOCITY
}

static void push(Entity* ent) {}

static void pickup(Entity* ent) {}

static void turn(Entity* ent) {}

static void useitem(Entity* ent) {}



void(*entActionFuncs[])(Entity*) = 
{
    none,
    attack,
    block,
    throw,
    push,
    pickup,
    turn,
    useitem,
};
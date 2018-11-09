#include "entactions.h"

#include <Engine/physics.h>
#include <Engine/transform.h>

#include "entity.h"

static void none(Entity* ent){}

static void attack(Entity* ent)
{

}

static void block(Entity* ent) {}

#define RANGE 45
#define VELOCITY 500
#define INCAPACITATE 1.125
#define DELAY 0.5
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

    physics->vel = vec2_scale((vec2_t) { cos(direction), sin(direction) }, VELOCITY);

    target->incapacitated = INCAPACITATE;
    ent->actionDelay = DELAY;
}
#undef RANGE
#undef VELOCITY
#undef INCAPACITATE
#undef COOLDOWN

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
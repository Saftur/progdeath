/**
 * @file splashscreen.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/18/18
 * @brief Splash Screen implementation
 * @addtogroup Game-Levels
 * @{
 */
#include "splashscreen.h"

#include <Engine/gamelayer.h>
#include <Engine/transform.h>
#include <Engine/sprite.h>

static float countdown = 2;

/**
 * @brief Countdown to pass screen
 * @param comp Logo Component
 */
static void logoUpdate(Component *comp) {
    countdown -= dt();
    if (countdown <= 0.f)
        LevelMngr_setNextLevel(comp->owner->objMngr->gLayer->lvlMngr, "Menu Screen");
}

/**
 * @brief Initialize Splash Screen
 * @param objMngr ObjectMngr to load level on
 */
void splashScreenInit(ObjectMngr *objMngr) {
    Object *logoObj = Object_new("Logo");
    Transform *logoTrs = Transform_new();
    logoTrs->pos = (Point){ canvasWidth / 2, canvasHeight / 2 };
    Sprite *logoSprite = Sprite_new("Assets\\DigiPen_RED_1024px.png", (Vector2D){ 1024, 247 });
    logoSprite->comp.update = logoUpdate;
    Object_addComp(logoObj, logoTrs);
    Object_addComp(logoObj, logoSprite);
    ObjectMngr_addObj(objMngr, logoObj);
}

/// @}

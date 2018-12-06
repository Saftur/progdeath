/**
 * @file splashscreen.c
 * @author Arthur Bouvier (a.bouvier)
 * @brief Splash Screen implementation
 * @course GAM100F18
 * Copyright © 2018 DigiPen (USA) Corporation
 * @addtogroup Game-Screens
 * @{
 */
#include "splashscreen.h"

#include <Engine/gamelayer.h>
#include <Engine/transform.h>
#include <Engine/sprite.h>
#include "Engine/text.h"

static float countdown = 3.f;

/**
 * @brief Countdown to pass screen
 * @param comp Logo Component
 */
static void logoUpdate(Component *comp) {
    Sprite *spr = (Sprite*)comp;
    countdown -= dt();
    if (countdown < 0.f || keyPressed(KEY_SPACE))
        ScreenMngr_setNextScreen(comp->owner->objMngr->gLayer->scrMngr, "Menu");
    else if (countdown > 2.f)
        spr->alpha = 1.f - (countdown - 2.f);
    else if (countdown > 0.25f)
        spr->alpha = 1.f;
    else if (countdown > 0.f)
        spr->alpha = countdown*4.f;
}

/**
 * @brief Initialize Splash Screen
 * @param objMngr ObjectMngr to load screen on
 */
void splashScreenInit(ObjectMngr *objMngr) {

    Object *obj = Object_new("Text");
    Transform *trs = Transform_new();
    trs->pos = (vec2_t) { 12, canvasHeight - 75 };
    Text *text = Text_new("Copyright 2018 DigiPen (USA) Corporation", 50);
    Object_addComp(obj, trs);
    Object_addComp(obj, text);
    ObjectMngr_addObj(objMngr, obj);

    Object *logoObj = Object_new("Logo");
    Transform *logoTrs = Transform_new();
    logoTrs->pos = (vec2_t){ canvasWidth / 2, canvasHeight / 2 };
    Sprite *logoSprite = Sprite_new("Assets\\DigiPen_RED_1024px.png", (vec2_t){ 1024, 247 });
    logoSprite->comp.update = logoUpdate;
    Object_addComp(logoObj, logoTrs);
    Object_addComp(logoObj, logoSprite);
    ObjectMngr_addObj(objMngr, logoObj);
}

/// @}

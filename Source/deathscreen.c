/**
 * @file deathscreen.c
 * @author Arthur Bouvier (a.bouvier)
 * @brief Death screen implementation
 * @course GAM100F18
 * Copyright © 2018 DigiPen (USA) Corporation
 * @addtogroup Game-Screens
 * @{
 */
#include "deathscreen.h"

#include <Engine/gamelayer.h>
#include <Engine/transform.h>
#include <Engine/button.h>
#include <Engine/emptycomp.h>

#include "gamebuttons.h"
#include <stdio.h>

int has_won = 0;

//  FROWNY FACE

/**
 * @brief Draw frowny face
 * @param comp Component to draw on
 */
static void drawFrownyFace(Component *comp) {
    Transform *trs = Object_getComp(comp->owner, TRANSFORM);
    if (!trs) return;

    vec2_t pos = trs->pos;

    stroke(127, 127, 127, 255);
    fill(255, 255, 0, 255);
    circle(pos.x, pos.y, 150);

    noStroke();
    fill(0, 0, 0, 255);
    circle(pos.x - 50, pos.y - 35, 20);
    circle(pos.x + 50, pos.y - 35, 20);

    beginShape();
    for (int i = 1; i <= 19; i++) {
        float rad = (float)i * PI / 20.f;
        vertex(pos.x + cos(rad) * 80, pos.y + 80 - sin(rad) * 50);
    }
    for (int i = 19; i >= 1; i--) {
        float rad = (float)i * PI / 20.f;
        vertex(pos.x + cos(rad) * 70, pos.y + 80 - sin(rad) * 40);
    }
    endShape();
    float rad = PI / 20.f;
    circle(pos.x + cos(rad) * 75, pos.y + 80 - sin(rad) * 45, 5.f);
    circle(pos.x - cos(rad) * 75, pos.y + 80 - sin(rad) * 45, 5.f);
}


PImage backgroundImage;
PImage gameLogo;

static void logoDraw(Transform *trs)
{
    image(gameLogo, canvasWidth / 2, 250, 915, 500);
}


static void backgroundPosUpdate(Transform *trs)
{
    trs->pos.y += 100 * dt();
    if (trs->pos.y > canvasWidth * 2)
        trs->pos.y -= canvasWidth * 3;
}

static void backgroundDraw(Transform *trs)
{
    image(backgroundImage, canvasWidth / 2, trs->pos.y, canvasWidth + 100, canvasWidth + 100);
}

static void drawFirstRect()
{
    stroke(0, 0, 0, 255);
    fill(128, 128, 128, 255);
    rect(10, 650, 760, 370);
}

static void drawSecondRect()
{
    stroke(0, 0, 0, 255);
    fill(128, 128, 128, 255);
    rect(750, 125, 400, 100);
}
/**
 * @brief Draw frowny face
 * @param comp Component to draw on
 */
static void drawSmileyFace(Component *comp) {
    Transform *trs = Object_getComp(comp->owner, TRANSFORM);
    if (!trs) return;

    vec2_t pos = trs->pos;

    stroke(127, 127, 127, 255);
    fill(255, 255, 0, 255);
    circle(pos.x, pos.y, 150);

    noStroke();
    fill(0, 0, 0, 255);
    circle(pos.x - 50, pos.y - 35, 20);
    circle(pos.x + 50, pos.y - 35, 20);

    beginShape();
    for (int i = 1; i <= 19; i++) {
        float rad = (float)i * PI / 20.f;
        vertex(pos.x + cos(rad) * 80, pos.y + 40 + sin(rad) * 50);
    }
    for (int i = 19; i >= 1; i--) {
        float rad = (float)i * PI / 20.f;
        vertex(pos.x + cos(rad) * 70, pos.y + 40 + sin(rad) * 40);
    }
    endShape();
    float rad = PI / 20.f;
    circle(pos.x + cos(rad) * 75, pos.y + 40 + sin(rad) * 45, 5.f);
    circle(pos.x - cos(rad) * 75, pos.y + 40 + sin(rad) * 45, 5.f);
}

//  LEVEL INIT

/**
 * @brief Initialize Death Screen
 * @param objMngr ObjectMngr to load screen on
 */
void deathScreenInit(ObjectMngr *objMngr) {
    backgroundImage = loadImage("Assets/Background.png");
    gameLogo = loadImage("Assets/GameLogo.png");


    for (int i = 0; i < 3; i++) {
        Object *obj = Object_new("Object");
        Transform *trs = Transform_new();
        trs->posUpdate = backgroundPosUpdate;
        trs->comp.draw = backgroundDraw;
        trs->pos.y = i * canvasWidth;
        Object_addComp(obj, trs);
        ObjectMngr_addObj(objMngr, obj);
    }

    Transform *trs;
    Text *text;
    Object *obj;
    EmptyComp *ec;

    obj = Object_new("Rect");
    ec = EmptyComp_new();
    ec->comp.draw = drawFirstRect;
    Object_addComp(obj, ec);
    ObjectMngr_addObj(objMngr, obj);

    obj = Object_new("Rect");
    ec = EmptyComp_new();
    ec->comp.draw = drawSecondRect;
    Object_addComp(obj, ec);
    ObjectMngr_addObj(objMngr, obj);

    const char *tutorialText[] = {
       "President: Claude Comair",
       "Instructors: Justin Chambers",
        "Developers:", 
        "                    Arthur Bouvier (Programmer)",
        "           and Connor Meyers (Programmer)",
        "Third-Party Libraries:",
        "Copyright 2018 Lua.org, PUC-Rio.",
    };

    for (unsigned i = 0; i < sizeof(tutorialText) / sizeof(const char*); i++) {
        Object *obj = Object_new("Text");
        Transform *trs = Transform_new();
        trs->pos = (vec2_t) { 12.f, 650 + (float)(i + 1) * 50.f };
        Text *text = Text_new(tutorialText[i], 50);
        Object_addComp(obj, trs);
        Object_addComp(obj, text);
        ObjectMngr_addObj(objMngr, obj);
    }

    obj = Object_new("Result Text");
    trs = Transform_new();
    trs->pos = (vec2_t){ canvasWidth / 2 - 175, 200.f };
    text = Text_new(has_won ? "You Won" : "You Lost", 100);
    Object_addComp(obj, trs);
    Object_addComp(obj, text);
    ObjectMngr_addObj(objMngr, obj);


    Object *buttonObj;
    Transform *buttonTrs;
    Button *button;

    buttonObj = Object_new("Menu Button");
    buttonTrs = Transform_new();
    buttonTrs->pos = (vec2_t){ canvasWidth/2 - 500, canvasHeight/2 };
    button = Button_new((vec2_t){ 200, 60 }, menuButtonEffect, "Menu");
    Object_addComp(buttonObj, buttonTrs);
    Object_addComp(buttonObj, button);
    ObjectMngr_addObj(objMngr, buttonObj);

    buttonObj = Object_new("Restart Button");
    buttonTrs = Transform_new();
    buttonTrs->pos = (vec2_t){ canvasWidth/2, canvasHeight/2 };
    button = Button_new((vec2_t){ 200, 60 }, playButtonEffect, "Restart");
    Object_addComp(buttonObj, buttonTrs);
    Object_addComp(buttonObj, button);
    ObjectMngr_addObj(objMngr, buttonObj);

    buttonObj = Object_new("Quit Button");
    buttonTrs = Transform_new();
    buttonTrs->pos = (vec2_t){ canvasWidth/2 + 500, canvasHeight/2 };
    button = Button_new((vec2_t){ 200, 60 }, quitButtonEffect, "Quit");
    Object_addComp(buttonObj, buttonTrs);
    Object_addComp(buttonObj, button);
    ObjectMngr_addObj(objMngr, buttonObj);

    obj = Object_new("Frowny Face");
    trs = Transform_new();
    trs->pos = (vec2_t){ canvasWidth / 2, 800 };
    EmptyComp *comp = EmptyComp_new();
    comp->comp.draw = has_won ? drawSmileyFace : drawFrownyFace;
    Object_addComp(obj, trs);
    Object_addComp(obj, comp);
    ObjectMngr_addObj(objMngr, obj);

}

/// @}

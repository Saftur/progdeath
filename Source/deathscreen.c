/**
 * @file deathscreen.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/18/18
 * @brief Death screen implementation
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
    Object *obj;
    Transform *trs;
    Text *text;

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

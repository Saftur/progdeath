/**
 * @file menuscreen.c
 * @author Arthur Bouvier (a.bouvier)
 * @brief Menu Screen implementation
 * @addtogroup Game-Screens
 * @{
 */
#include "menuscreen.h"

#include <C_Processing.h>

#include <Engine/gamelayer.h>
#include <Engine/emptycomp.h>
#include <Engine/transform.h>
#include <Engine/physics.h>
#include <Engine/text.h>
#include <Engine/button.h>

#include "gamebuttons.h"


PImage backgroundImage;
PImage gameLogo;

static void logoDraw(Transform *trs)
{
    image(gameLogo, canvasWidth / 2, 250, 915, 500 );
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
    fill(128, 128, 128, 255);
    rect(10, 500, 1150, 500);
}

/**
 * @brief Initialize Menu Screen
 * @param objMngr ObjectMngr to load screen on
 */
void menuScreenInit(ObjectMngr *objMngr) {
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

    Object *obj = Object_new("Object");
    EmptyComp *ec = EmptyComp_new();
    ec->comp.draw = logoDraw;
    Object_addComp(obj, ec);
    ObjectMngr_addObj(objMngr, obj);

    const char *tutorialText[] ={
        "                               Welcome to the Programmable Death Match.",
        "Enter the editor, we have preloaded an example script",
        "for you to mess around with and understand the basics.",
        "It is your responsibility to learn the ropes around here.",
        "There are 20 enemies waiting for you in the arena, try",
        "and defeat them all without crashing the game. You are",
        "in control now. Don't let us down programmer."
    };

    obj = Object_new("Rect");
    ec = EmptyComp_new();
    ec->comp.draw = drawFirstRect;
    Object_addComp(obj, ec);
    ObjectMngr_addObj(objMngr, obj);

    for (unsigned i = 0; i < sizeof(tutorialText) / sizeof(const char*); i++) {
        Object *obj = Object_new("Text");
        Transform *trs = Transform_new();
        trs->pos = (vec2_t){ 12.f, 500 + (float)(i+1) * 50.f };
        Text *text = Text_new(tutorialText[i], 50);
        Object_addComp(obj, trs);
        Object_addComp(obj, text);
        ObjectMngr_addObj(objMngr, obj);
    }

    Object *buttonObj;
    Transform *buttonTrs;
    Button *button;

    buttonObj = Object_new("Play Button");
    buttonTrs = Transform_new();
    buttonTrs->pos = (vec2_t){ 550 + canvasWidth/2, 150 + canvasHeight/2 - 80 };
    button = Button_new((vec2_t){ 300, 120 }, playButtonEffect, "Play");
    Object_addComp(buttonObj, buttonTrs);
    Object_addComp(buttonObj, button);
    ObjectMngr_addObj(objMngr, buttonObj);

    buttonObj = Object_new("Editor Button");
    buttonTrs = Transform_new();
    buttonTrs->pos = (vec2_t){ 550 + canvasWidth/2, 150 + canvasHeight/2 + 60 };
    button = Button_new((vec2_t){ 300, 120 }, editorButtonEffect, "Editor");
    Object_addComp(buttonObj, buttonTrs);
    Object_addComp(buttonObj, button);
    ObjectMngr_addObj(objMngr, buttonObj);

    buttonObj = Object_new("Quit Button");
    buttonTrs = Transform_new();
    buttonTrs->pos = (vec2_t){ 550 + canvasWidth/2, 150 + canvasHeight/2 + 200 };
    button = Button_new((vec2_t){ 300, 120 }, quitButtonEffect, "Quit");
    Object_addComp(buttonObj, buttonTrs);
    Object_addComp(buttonObj, button);
    ObjectMngr_addObj(objMngr, buttonObj);

    Object *textObj;
    Transform *textTrs;
    Text *text;

    textObj = Object_new("Author Text");
    textTrs = Transform_new();
    textTrs->pos = (vec2_t){ 12, canvasHeight - 120 };
    text = Text_new("Programmers: Arthur Bouvier and Connor Meyers", 50);
    Object_addComp(textObj, textTrs);
    Object_addComp(textObj, text);
    ObjectMngr_addObj(objMngr, textObj);

    textObj = Object_new("Instructor Text");
    textTrs = Transform_new();
    textTrs->pos = (vec2_t){ 12, canvasHeight - 70 };
    text = Text_new("Instructor: Justin Chambers", 50);
    Object_addComp(textObj, textTrs);
    Object_addComp(textObj, text);
    ObjectMngr_addObj(objMngr, textObj);

   /* textObj = Object_new("Copyright Text");
    textTrs = Transform_new();
    textTrs->pos = (vec2_t){ 0, canvasHeight - 20 };
    text = Text_new("Copyright 2018 DigiPen Institute of Technology and DigiPen (USA) Corporation", 50);
    Object_addComp(textObj, textTrs);
    Object_addComp(textObj, text);
    ObjectMngr_addObj(objMngr, textObj);*/
}

/// @}

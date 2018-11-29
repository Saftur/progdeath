/**
 * @file menuscreen.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/28/18
 * @brief Menu Screen implementation
 * @addtogroup Game-Screens
 * @{
 */
#include "menuscreen.h"

#include <C_Processing.h>

#include <Engine/gamelayer.h>
#include <Engine/transform.h>
#include <Engine/text.h>
#include <Engine/button.h>

#include "gamebuttons.h"

/**
 * @brief Initialize Menu Screen
 * @param objMngr ObjectMngr to load screen on
 */
void menuScreenInit(ObjectMngr *objMngr) {
    const char *tutorialText[] ={
        "                               Welcome to the Programmable Death Match.",
        "Enter the editor, we have preloaded an example script",
        "for you to mess around with and understand the basics.",
        "It is your responsibility to learn the ropes around here.",
        "There are 20 enemies waiting for you in the arena, try",
        "and defeat them all without crashing the game. You are",
        "in control now. Don't let us down programmer."
    };

    for (unsigned i = 0; i < sizeof(tutorialText) / sizeof(const char*); i++) {
        Object *obj = Object_new("Text");
        Transform *trs = Transform_new();
        trs->pos = (vec2_t){ 0.f, (float)(i+1) * 50.f };
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
    buttonTrs->pos = (vec2_t){ canvasWidth/2, canvasHeight/2 - 60 };
    button = Button_new((vec2_t){ 150, 60 }, playButtonEffect, "Play");
    Object_addComp(buttonObj, buttonTrs);
    Object_addComp(buttonObj, button);
    ObjectMngr_addObj(objMngr, buttonObj);

    buttonObj = Object_new("Editor Button");
    buttonTrs = Transform_new();
    buttonTrs->pos = (vec2_t){ canvasWidth/2, canvasHeight/2 + 60 };
    button = Button_new((vec2_t){ 150, 60 }, editorButtonEffect, "Editor");
    Object_addComp(buttonObj, buttonTrs);
    Object_addComp(buttonObj, button);
    ObjectMngr_addObj(objMngr, buttonObj);

    buttonObj = Object_new("Quit Button");
    buttonTrs = Transform_new();
    buttonTrs->pos = (vec2_t){ canvasWidth/2, canvasHeight/2 + 180 };
    button = Button_new((vec2_t){ 150, 60 }, quitButtonEffect, "Quit");
    Object_addComp(buttonObj, buttonTrs);
    Object_addComp(buttonObj, button);
    ObjectMngr_addObj(objMngr, buttonObj);

    Object *textObj;
    Transform *textTrs;
    Text *text;

    textObj = Object_new("Author Text");
    textTrs = Transform_new();
    textTrs->pos = (vec2_t){ 0, canvasHeight - 120 };
    text = Text_new("Programmers: Arthur Bouvier and Connor Meyers", 50);
    Object_addComp(textObj, textTrs);
    Object_addComp(textObj, text);
    ObjectMngr_addObj(objMngr, textObj);

    textObj = Object_new("Instructor Text");
    textTrs = Transform_new();
    textTrs->pos = (vec2_t){ 0, canvasHeight - 70 };
    text = Text_new("Instructor: Justin Chambers", 50);
    Object_addComp(textObj, textTrs);
    Object_addComp(textObj, text);
    ObjectMngr_addObj(objMngr, textObj);

    textObj = Object_new("Copyright Text");
    textTrs = Transform_new();
    textTrs->pos = (vec2_t){ 0, canvasHeight - 20 };
    text = Text_new("Copyright 2018 DigiPen Institute of Technology and DigiPen (USA) Corporation", 50);
    Object_addComp(textObj, textTrs);
    Object_addComp(textObj, text);
    ObjectMngr_addObj(objMngr, textObj);
}

/// @}

/**
 * @file button.c
 * @author Arthur Bouvier (a.bouvier)
 * @brief Button implementation
 * @course GAM100F18
 * Copyright � 2018 DigiPen (USA) Corporation
 * @addtogroup Components
 * @{
 */
#include "button.h"

#include "transform.h"
#include "util.h"

#define BUTTON_TEXT_OFFSET_X 12 ///< @brief Offset from edges of button to text
#define BUTTON_TEXT_OFFSET_Y 24 ///< @brief Offset from edges of button to text

/**
 * @brief Create new Button
 * @param buttonSize Size of Button
 * @param effect     Effect on Button press
 * @param text       Button text
 * @return New Button
 */
Button *Button_new(vec2_t buttonSize, ButtonEffect effect, const char *text) {
    Button *this = malloc(sizeof(Button));
    this->comp.typeName = "Button";
    this->comp.typeId = BUTTON;
    this->comp.clone = _Button_clone;
    this->comp.delete = _Button_delete;
    this->comp.update = _Button_update;
    this->comp.draw = _Button_draw;
    this->comp.collides = false;
    this->comp.coll_resolve = NULL;
    this->comp.owner = NULL;

    this->buttonSize = buttonSize;
    this->effect = effect;
    this->text = Text_new(text, buttonSize.y - 5);
    this->text->comp.owner = NULL;

    return this;
}

/**
 * @brief Clone Button
 * @param this Button to clone
 * @return Cloned Button
 */
Button *_Button_clone(Button *this) {
    Button *new = malloc(sizeof(Button));

    new->buttonSize = this->buttonSize;
    new->effect = this->effect;
    new->text = Text_new(this->text, new->buttonSize.y - 5);
    
    return new;
}

/**
 * @brief Delete Button
 * @param this Button to delete
 */
void _Button_delete(Button *this) {
    _Text_delete(this->text);
    free(this);
}

/**
 * @brief Check if mouse is on Button
 * @param bPos  Button position
 * @param bSize Button size
 * @return If mouse is on Button
 */
static bool mouseOnButton(vec2_t bPos, vec2_t bSize) {
    vec2_t bHalfSize = vec2_scale(bSize, 0.5f);
    return vec2_in_range((vec2_t){ mouseX, mouseY }, vec2_sub(bPos, bHalfSize), vec2_add(bPos, bHalfSize));
}

/**
 * @brief Update Button
 * @param this Button to update
 */
void _Button_update(Button *this) {
    if (!this->text->comp.owner)
        this->text->comp.owner = this->comp.owner;

    if (!this->effect)
        return;

    Transform *trs = Object_getComp(this->comp.owner, TRANSFORM);
    if (!trs) return;
    if (mouseClicked() && mouseOnButton(trs->pos, this->buttonSize))
        this->effect(this);
}

/**
 * @brief Draw Button
 * @param this Button to draw
 */
void _Button_draw(Button *this) {
    Transform *trs = Object_getComp(this->comp.owner, TRANSFORM);
    if (!trs) return;

    stroke(0, 0, 0, 255);
    if (mouseOnButton(trs->pos, this->buttonSize))
        fill(200, 200, 200, 255);
    else fill(100, 100, 100, 255);

    vec2_t *pos = &trs->pos;
    vec2_t halfSize = vec2_scale(this->buttonSize, 0.5f);
    rect(pos->x - halfSize.x, pos->y - halfSize.y, this->buttonSize.x, this->buttonSize.y);

    *pos = (vec2_t){ pos->x - halfSize.x + BUTTON_TEXT_OFFSET_X, pos->y + halfSize.y - BUTTON_TEXT_OFFSET_Y };
    _Text_draw(this->text);
    *pos = (vec2_t){ pos->x + halfSize.x - BUTTON_TEXT_OFFSET_X, pos->y - halfSize.y + BUTTON_TEXT_OFFSET_Y };
}

/// @}

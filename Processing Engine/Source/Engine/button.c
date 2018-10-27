/**
 * @file button.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/18/18
 * @brief Button implementation
 * @addtogroup Components
 * @{
 */
#include "button.h"

#include "transform.h"

#define BUTTON_TEXT_OFFSET 12 ///< @brief Offset from edges of button to text

/**
 * @brief Create new Button
 * @param buttonSize Size of Button
 * @param effect     Effect on Button press
 * @param text       Button text
 * @return New Button
 */
Button *Button_new(Vector2D buttonSize, ButtonEffect effect, const char *text) {
    Button *this = malloc(sizeof(Button));
    this->comp.typeName = "Button";
    this->comp.typeId = BUTTON;
    this->comp.delete = _Button_delete;
    this->comp.update = _Button_update;
    this->comp.draw = _Button_draw;
    this->comp.collides = false;
    this->comp.coll_resolve = NULL;

    this->buttonSize = buttonSize;
    this->effect = effect;
    this->text = Text_new(text, buttonSize.y - 5);
    this->text->comp.owner = NULL;

    return this;
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
static bool mouseOnButton(Point bPos, Vector2D bSize) {
    Vector2D bHalfSize = Vector2D_scale(0.5f, bSize);
    return Vector2D_inRange((Point){ mouseX, mouseY }, Vector2D_sub(bPos, bHalfSize), Vector2D_add(bPos, bHalfSize));
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

    Point *pos = &trs->pos;
    Vector2D halfSize = Vector2D_scale(0.5f, this->buttonSize);
    rect(pos->x - halfSize.x, pos->y - halfSize.y, this->buttonSize.x, this->buttonSize.y);

    *pos = (Point){ pos->x - halfSize.x + BUTTON_TEXT_OFFSET, pos->y + halfSize.y - BUTTON_TEXT_OFFSET };
    _Text_draw(this->text);
    *pos = (Point){ pos->x + halfSize.x - BUTTON_TEXT_OFFSET, pos->y - halfSize.y + BUTTON_TEXT_OFFSET };
}

/// @}

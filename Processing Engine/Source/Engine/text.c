/**
 * @file text.c
 * @author Arthur Bouvier (a.bouvier)
 * @brief Text implementation
 * @addtogroup Components
 * @{
 */
#include "text.h"

#include <string.h> // strcpy

#include "transform.h"

/**
 * @brief Create new Text
 * @return New Text
 */
Text *Text_new(const char *text, int textSize) {
    Text *this = malloc(sizeof(Text));
    this->comp.typeName = "Text";
    this->comp.typeId = TEXT;
    this->comp.clone = _Text_clone;
    this->comp.delete = _Text_delete;
    this->comp.update = NULL;
    this->comp.draw = _Text_draw;
    this->comp.collides = false;
    this->comp.coll_resolve = NULL;
    this->comp.owner = NULL;

    this->text = NULL;
    Text_setText(this, text);
    this->textSize = textSize;
    this->textColor = color(0, 0, 0, 255);

    return this;
}

/**
 * @brief Clone Text
 * @param this Text to clone
 * @return Cloned Text
 */
Text *_Text_clone(Text *this) {
    Text *new = malloc(sizeof(Text));

    new->text = NULL;
    Text_setText(new, this->text);
    new->textSize = this->textSize;
    new->textColor = this->textColor;

    return new;
}

/**
 * @brief Delete Text
 * @param this Text to delete
 */
void _Text_delete(Text *this) {
    free(this->text);
    free(this);
}

/**
 * @brief Draw Text
 * @param this Text to draw
 */
void _Text_draw(Text *this) {
    Transform *trs = Object_getComp(this->comp.owner, TRANSFORM);
    if (!trs) return; 
    fillColor(this->textColor);
    textSize((float)this->textSize);
    text(this->text, trs->pos.x, trs->pos.y);
}

/**
 * @brief Set Text text to given text
 * @param this Text to change text of
 * @param text New text
 */
void Text_setText(Text *this, const char *text) {
    free(this->text);

    this->text = malloc((strlen(text)+1) * sizeof(char));
    strcpy(this->text, text);
}

/// @}

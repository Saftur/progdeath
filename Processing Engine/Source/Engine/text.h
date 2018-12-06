/**
 * @file text.h
 * @author Arthur Bouvier (a.bouvier)
 * @brief Text Component
 * @addtogroup Components
 * @{
 */
#pragma once

#include "component.h"

/**
 * @brief Text Component
 */
typedef struct Text {
    Component comp; ///< @brief Component data

    char *text;       ///< @brief Text to display
    int textSize;     ///< @brief Size of text to display
    PColor textColor; ///< @brief PColor of text to display
} Text;

/**
 * @brief Create new Text
 * @return New Text
 */
Text *Text_new(const char *text, int textSize);

/**
 * @brief Clone Text
 * @param this Text to clone
 * @return Cloned Text
 */
Text *_Text_clone(Text *this);

/**
 * @brief Delete Text
 * @param this Text to delete
 */
void _Text_delete(Text *this);

/**
 * @brief Draw Text
 * @param this Text to draw
 */
void _Text_draw(Text *this);

/**
 * @brief Set Text text to given text
 * @param this Text to change text of
 * @param text New text
 */
void Text_setText(Text *this, const char *text);

/// @}

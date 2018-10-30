/**
 * @file button.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/18/18
 * @brief Button Component
 * @addtogroup Components
 * @{
 */
#pragma once

#include "component.h"

#include "text.h"

typedef struct Button Button; ///< @brief Button forward declaration

typedef void (*ButtonEffect)(Button*); ///< @brief Button effect function signature

/**
 * @brief Button Component
 */
typedef struct Button {
    Component comp; ///< @brief Component data

    vec2_t buttonSize;   ///< @brief Width & height of Button
    ButtonEffect effect; ///< @brief What happens when the Button is pressed
    Text *text;          ///< @brief Button text
} Button;

/**
 * @brief Create new Button
 * @param buttonSize Size of Button
 * @param effect     Effect on Button press
 * @param text       Button text
 * @return New Button
 */
Button *Button_new(vec2_t buttonSize, ButtonEffect effect, const char *text);

/**
 * @brief Clone Button
 * @param this Button to clone
 * @return Cloned Button
 */
Button *_Button_clone(Button *this);

/**
 * @brief Delete Button
 * @param this Button to delete
 */
void _Button_delete(Button *this);

/**
 * @brief Update Button
 * @param this Button to update
 */
void _Button_update(Button *this);
/**
 * @brief Draw Button
 * @param this Button to draw
 */
void _Button_draw(Button *this);

/// @}

/**
 * @file focus.h
 * @author Arthur Bouvier (a.bouvier)
 * @brief Keyboard focus
 * @course GAM100F18
 * Copyright © 2018 DigiPen (USA) Corporation
 */
#pragma once

#include <C_Processing.h>

typedef int(*FocusCheckFunc)(void *upvalue);
typedef void(*FocusCallback)(Key key,KeyState state,void *upvale);

/**
 * @brief Initialize focus
 */
void initFocus();
/**
 * @brief Clean up focus data
 */
void endFocus();

void addFocusObject(FocusCheckFunc check, FocusCallback callback, void *upvalue);
void removeFocusObject(void *upvalue);

/**
 * @brief Set the focus
 * @param callback Focus callback
 * @param upvalue  Focus upvalue
 */
//void setFocusCallback(FocusCallback callback, void *upvalue);
/**
 * @brief Unset the focus only if current upvalue is equal to given upvalue
 * @param upvalue Upvalue to check
 */
//void unsetFocusCallback(void *upvalue);
/**
 * @brief Check if current upvalue is equal to given upvalue
 * @param upvalue Upvalue to check
 * @return If upvalues are equal
 */
//int checkFocusUpvalue(void *upvalue);

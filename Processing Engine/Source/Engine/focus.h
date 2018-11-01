/**
 * @file focus.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 11/1/18
 * @brief Keyboard focus
 */
#pragma once

#include <C_Processing.h>

typedef void(*FocusCallback)(Key key,KeyState state,void *upvale);

/**
 * @brief Calls current focus callback with given upvalue
 * @param key   Key changed
 * @param state State of key
 */
void keyCallback(Key key, KeyState state);

/**
 * @brief Set the focus
 * @param callback Focus callback
 * @param upvalue  Focus upvalue
 */
void setFocusCallback(FocusCallback callback, void *upvalue);
/**
 * @brief Unset the focus only if current upvalue is equal to given upvalue
 * @param upvalue Upvalue to check
 */
void unsetFocusCallback(void *upvalue);
/**
 * @brief Check if current upvalue is equal to given upvalue
 * @param upvalue Upvalue to check
 * @return If upvalues are equal
 */
int checkFocusUpvalue(void *upvalue);

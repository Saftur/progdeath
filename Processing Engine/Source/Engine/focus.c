/**
 * @file focus.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 11/1/18
 * @brief Keyboard focus implementation
 */
#include "focus.h"

/**
 * @brief Contains current focus callback and upvalue
 */
static struct Focus {
    FocusCallback callback;
    void *upvalue;
} focus;

/**
 * @brief Calls current focus callback with given upvalue
 * @param key   Key changed
 * @param state State of key
 */
void keyCallback(Key key, KeyState state) {
    if (focus.callback)
        focus.callback(key, state, focus.upvalue);
}

/**
 * @brief Set the focus
 * @param callback Focus callback
 * @param upvalue  Focus upvalue
 */
void setFocusCallback(FocusCallback callback, void *upvalue) {
    focus.callback = callback;
    focus.upvalue = upvalue;
}

/**
 * @brief Unset the focus only if current upvalue is equal to given upvalue
 * @param upvalue Upvalue to check
 */
void unsetFocusCallback(void *upvalue) {
    if (checkFocusUpvalue(upvalue))
        setFocusCallback(NULL, NULL);
}

/**
 * @brief Check if current upvalue is equal to given upvalue
 * @param upvalue Upvalue to check
 * @return If upvalues are equal
 */
int checkFocusUpvalue(void *upvalue) {
    return upvalue == focus.upvalue;
}

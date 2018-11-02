/**
 * @file focus.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 11/1/18
 * @brief Keyboard focus implementation
 */
#include "focus.h"

#include "engine.h"
#include "list.h"

/**
 * @brief Contains focus callback and upvalue
 */
//typedef struct Focus {
//} Focus;

typedef struct FocusObject {
    FocusCheckFunc check;
    FocusCallback callback;
    void *upvalue;
} FocusObject;

static List *focusObjs;
static FocusObject *focus;

/**
 * @brief Calls current focus callback with given upvalue
 * @param key   Key changed
 * @param state State of key
 */
void keyCallback(Key key, KeyState state) {
    if (key == KEY_ESCAPE && state == KeyPressed)
        focus = NULL;
    if (focus)
        focus->callback(key, state, focus->upvalue);
}

/**
 * @brief Checks if any focus object should be focused on
 * @param button Button changed
 * @param state  State of button
 */
void mouseCallback(MouseButton button, KeyState state) {
    if (state != KeyPressed) return;
    for (unsigned i = 0; i < focusObjs->size; i++) {
        FocusObject *obj = focusObjs->items[i];
        if (obj->check(obj->upvalue)) {
            focus = obj;
            return;
        }
    }
    focus = NULL;
}

/**
 * @brief Initialize focus
 */
void initFocus() {
    Engine_addKeyCallback(keyCallback, NULL);
    Engine_addMouseCallback(mouseCallback, NULL);
    focusObjs = List_new(20, NULL, free);
    focus = NULL;
}

/**
 * @brief Clean up focus data
 */
void endFocus() {
    List_delete(focusObjs);
    focus = NULL;
}

void addFocusObject(FocusCheckFunc check, FocusCallback callback, void *upvalue) {
    FocusObject *obj = malloc(sizeof(FocusObject));
    obj->check = check;
    obj->callback = callback;
    obj->upvalue = upvalue;
    List_push_back(focusObjs, obj);
}

void removeFocusObject(void *upvalue) {
    for (unsigned i = 0; i < focusObjs->size; i++) {
        FocusObject *obj = focusObjs->items[i];
        if (obj->upvalue == upvalue) {
            if (focus == obj)
                focus = NULL;
            List_remove(focusObjs, i);
            return;
        }
    }
}

/**
 * @brief Set the focus
 * @param callback Focus callback
 * @param upvalue  Focus upvalue
 */
/*void setFocusCallback(FocusCallback callback, void *upvalue) {
    focus.callback = callback;
    focus.upvalue = upvalue;
}*/

/**
 * @brief Unset the focus only if current upvalue is equal to given upvalue
 * @param upvalue Upvalue to check
 */
/*void unsetFocusCallback(void *upvalue) {
    if (checkFocusUpvalue(upvalue))
        setFocusCallback(NULL, NULL);
}*/

/**
 * @brief Check if current upvalue is equal to given upvalue
 * @param upvalue Upvalue to check
 * @return If upvalues are equal
 */
/*int checkFocusUpvalue(void *upvalue) {
    return upvalue == focus.upvalue;
}*/

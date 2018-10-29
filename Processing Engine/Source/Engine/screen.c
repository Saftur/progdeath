/**
 * @file screen.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/26/18
 * @brief Screen implementation
 * @addtogroup Engine
 * @{
 */
#include "screen.h"

/**
 * @brief Create new Screen
 * @param name   Screen name
 * @param init   Screen init function
 * @param delete Screen init function
 * @return New Screen
 */
Screen *Screen_new(const char *name, void (*init)(ObjectMngr*), void (*end)(Screen*)) {
    Screen *screen = malloc(sizeof(Screen));
    screen->name = name;
    screen->init = init;
    screen->end = end;
    return screen;
}

/**
 * @brief Initialize Screen
 * @param screen   Screen to initialize
 * @param objMngr ObjectMngr to initialize with Screen
 */
void Screen_init(Screen *screen, ObjectMngr *objMngr) {
    if (screen->init)
        screen->init(objMngr);
}

/**
 * @brief End Screen
 * @param screen Screen to end
 */
void Screen_end(Screen *screen) {
    if (screen->end)
        screen->end(screen);
}

/**
 * @brief Delete Screen
 * @param screen Screen to delete
 */
void Screen_delete(Screen *screen) {
    free(screen);
}

/// @}

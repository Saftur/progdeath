/**
 * @file screen.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/26/18
 * @brief Screen descriptor
 * @addtogroup Engine
 * @{
 */
#pragma once

#include "objectmngr.h"

typedef struct Screen Screen; ///< Screen forward declaration

/**
 * @brief Contains data for a game Screen
 */
typedef struct Screen {
    const char *name;          ///< Screen name
    void (*init)(ObjectMngr*); ///< Screen init function
    void (*end)(Screen*);      ///< Screen end function
} Screen;

/**
 * @brief Create new Screen
 * @param name   Screen name
 * @param init   Screen init function
 * @param delete Screen init function
 * @return New Screen
 */
Screen *Screen_new(const char *name, void (*init)(ObjectMngr*), void (*end)(Screen*));

/**
 * @brief Initialize Screen
 * @param screen   Screen to initialize
 * @param objMngr ObjectMngr to initialize with Screen
 */
void Screen_init(Screen *screen, ObjectMngr *objMngr);
/**
 * @brief End Screen
 * @param screen Screen to end
 */
void Screen_end(Screen *screen);
/**
 * @brief Delete Screen
 * @param screen Screen to delete
 */
void Screen_delete(Screen *screen);

/// @}

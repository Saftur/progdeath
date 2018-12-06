/**
 * @file screen.h
 * @author Arthur Bouvier (a.bouvier)
 * @brief Screen descriptor
 * @addtogroup Engine
 * @{
 */
#pragma once

#include "objectmngr.h"

typedef struct Screen Screen; ///< @brief Screen forward declaration

typedef void (*Screen_InitFunc)(ObjectMngr*); ///< @brief Screen init function signature
typedef void (*Screen_UpdateFunc)(Screen*);   ///< @brief Screen update function signature
typedef void (*Screen_EndFunc)(ObjectMngr*);  ///< @brief Screen end function signature

/**
 * @brief Contains data for a game Screen
 */
typedef struct Screen {
    const char *name;         ///< @brief Screen name
    ObjectMngr *objMngr;      ///< @brief ObjectMngr associated to Screen
    Screen_InitFunc init;     ///< @brief Screen init function
    Screen_UpdateFunc update; ///< @brief Screen update function
    Screen_EndFunc end;       ///< @brief Screen end function
} Screen;

/**
 * @brief Create new Screen
 * @param name   Screen name
 * @param init   Screen init function
 * @param update Screen update function
 * @param end    Screen end function
 * @return New Screen
 */
Screen *Screen_new(const char *name, Screen_InitFunc init, Screen_UpdateFunc update, Screen_EndFunc end);

/**
 * @brief Initialize Screen
 * @param screen   Screen to initialize
 * @param objMngr ObjectMngr to initialize with Screen
 */
void Screen_init(Screen *screen, ObjectMngr *objMngr);

/**
 * @brief Update Screen
 * @param screen   Screen to update
 */
void Screen_update(Screen *screen);
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

/**
 * @file level.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/26/18
 * @brief Level descriptor
 * @addtogroup Engine
 * @{
 */
#pragma once

#include "objectmngr.h"

typedef struct Level Level; ///< Level forward declaration

/**
 * @brief Contains data for a game Level
 */
typedef struct Level {
    const char *name;          ///< Level name
    void (*init)(ObjectMngr*); ///< Level init function
    void (*end)(Level*);       ///< Level end function
} Level;

/**
 * @brief Create new Level
 * @param name   Level name
 * @param init   Level init function
 * @param delete Level init function
 * @return New Level
 */
Level *Level_new(const char *name, void (*init)(ObjectMngr*), void (*end)(Level*));

/**
 * @brief Initialize Level
 * @param level   Level to initialize
 * @param objMngr ObjectMngr to initialize with Level
 */
void Level_init(Level *level, ObjectMngr *objMngr);
/**
 * @brief End Level
 * @param level Level to end
 */
void Level_end(Level *level);
/**
 * @brief Delete Level
 * @param level Level to delete
 */
void Level_delete(Level *level);

/// @}

/**
 * @file level.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/26/18
 * @brief Level implementation
 * @addtogroup Engine
 * @{
 */
#include "level.h"

/**
 * @brief Create new Level
 * @param name   Level name
 * @param init   Level init function
 * @param delete Level init function
 * @return New Level
 */
Level *Level_new(const char *name, void (*init)(ObjectMngr*), void (*end)(Level*)) {
    Level *level = malloc(sizeof(Level));
    level->name = name;
    level->init = init;
    level->end = end;
    return level;
}

/**
 * @brief Initialize Level
 * @param level   Level to initialize
 * @param objMngr ObjectMngr to initialize with Level
 */
void Level_init(Level *level, ObjectMngr *objMngr) {
    if (level->init)
        level->init(objMngr);
}

/**
 * @brief End Level
 * @param level Level to end
 */
void Level_end(Level *level) {
    if (level->end)
        level->end(level);
}

/**
 * @brief Delete Level
 * @param level Level to delete
 */
void Level_delete(Level *level) {
    free(level);
}

/// @}

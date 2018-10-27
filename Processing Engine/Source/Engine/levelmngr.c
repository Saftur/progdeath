/**
 * @file levelmngr.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/26/18
 * @brief LevelMngr implementation
 * @addtogroup Engine
 * @{
 */
#include "levelmngr.h"

#include <stdlib.h> // malloc
#include <string.h> // strcmp

#include "gamelayer.h"
#include "objectmngr.h"

/**
 * @brief Create new LevelMngr
 * @param gLayer Parent GameLayer
 * @return New LevelMngr
 */
LevelMngr *LevelMngr_new(GameLayer *gLayer) {
    LevelMngr *lvlMngr = malloc(sizeof(LevelMngr));
    lvlMngr->levels = List_new(10, Level_delete);
    lvlMngr->currLevel = NULL;
    lvlMngr->nextLevel = NULL;
    lvlMngr->gLayer = gLayer;
    return lvlMngr;
}

/**
 * @brief Delete LevelMngr
 * @param lvlMngr LevelMngr to delete
 */
void LevelMngr_delete(LevelMngr *lvlMngr) {
    List_delete(lvlMngr->levels);
    free(lvlMngr);
}

/**
 * @brief Update LevelMngr
 * @param lvlMngr LevelMngr to update
 */
void LevelMngr_update(LevelMngr *lvlMngr) {
    if (lvlMngr->nextLevel != lvlMngr->currLevel) {
        if (lvlMngr->currLevel)
            Level_end(lvlMngr->currLevel);
        ObjectMngr_deleteAll(lvlMngr->gLayer->objMngr);
        lvlMngr->currLevel = lvlMngr->nextLevel;
        Level_init(lvlMngr->currLevel, lvlMngr->gLayer->objMngr);
    }
}

/**
 * @brief Add Level to LevelMngr
 * @param lvlMngr LevelMngr to add to
 * @param level   Level to add
 */
void LevelMngr_addLevel(LevelMngr *lvlMngr, Level *level) {
    List_push_back(lvlMngr->levels, level);
}
/**
 * @brief Set next Level
 * @param lvlMngr   LevelMngr to set on
 * @param levelName Name of Level to set
 */
void LevelMngr_setNextLevel(LevelMngr *lvlMngr, const char *levelName) {
    for (unsigned i = 0; i < lvlMngr->levels->size; i++) {
        Level *level = lvlMngr->levels->items[i];
        if (strcmp(level->name, levelName) == 0) {
            lvlMngr->nextLevel = level;
            return;
        }
    }
}

/// @}

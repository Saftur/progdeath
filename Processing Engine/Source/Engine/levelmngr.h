/**
 * @file levelmngr.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/26/18
 * @brief Level manager
 * @addtogroup Engine
 * @{
 */
#pragma once

#include "list.h"
#include "level.h"

typedef struct GameLayer GameLayer; ///< GameLayer forward declaration

/**
 * @brief Holds current level data
 */
typedef struct LevelMngr {
    List *levels;     ///< List of Level s
    Level *currLevel; ///< Current Level
    Level *nextLevel; ///< Next Level

    GameLayer *gLayer; ///< Parent GameLayer
} LevelMngr;

/**
 * @brief Create new LevelMngr
 * @param gLayer Parent GameLayer
 * @return New LevelMngr
 */
LevelMngr *LevelMngr_new(GameLayer *gLayer);
/**
 * @brief Delete LevelMngr
 * @param lvlMngr LevelMngr to delete
 */
void LevelMngr_delete(LevelMngr *lvlMngr);

/**
 * @brief Update LevelMngr
 * @param lvlMngr LevelMngr to update
 */
void LevelMngr_update(LevelMngr *lvlMngr);

/**
 * @brief Add Level to LevelMngr
 * @param lvlMngr LevelMngr to add to
 * @param level   Level to add
 */
void LevelMngr_addLevel(LevelMngr *lvlMngr, Level *level);
/**
 * @brief Set next Level
 * @param lvlMngr   LevelMngr to set on
 * @param levelName Name of Level to set
 */
void LevelMngr_setNextLevel(LevelMngr *lvlMngr, const char *levelName);

/// @}

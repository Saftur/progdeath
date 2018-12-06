/**
 * @file gamelayer.h
 * @author Arthur Bouvier (a.bouvier)
 * @brief Layer in game
 * @addtogroup Util
 * @{
 */
#pragma once

#include <stdbool.h>

#include "screenmngr.h"
#include "objectmngr.h"

/**
 * @brief Layer in game
 * Example: Layer 0 is game, Layer 1 is hud.
 */
typedef struct GameLayer {
    ScreenMngr *scrMngr;  ///< @brief GameLayer's ScreenMngr
    ObjectMngr *objMngr; ///< @brief GameLayer's ObjectMngr

    bool updateLower; ///< @brief Update lower layers
    bool drawLower;   ///< @brief Draw lower layers
} GameLayer;

/**
 * @brief Create new GameLayer
 * @param updateLower Update lower layers
 * @param drawLower   Draw lower layers
 */
GameLayer *GameLayer_new(bool updateLower, bool drawLower);
/**
 * @brief Delete GameLayer
 * @param gLayer GameLayer to delete
 */
void GameLayer_delete(GameLayer *gLayer);

/**
 * @brief Update GameLayer
 * @param gLayer GameLayer to update
 */
void GameLayer_update(GameLayer *gLayer);
/**
 * @brief Draw GameLayer
 * @param gLayer GameLayer to draw
 */
void GameLayer_draw(GameLayer *gLayer);

/// @}

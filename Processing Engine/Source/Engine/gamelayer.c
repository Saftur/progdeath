/**
 * @file gamelayer.c
 * @author Arthur Bouvier (a.bouvier)
 * @brief GameLayer implementation
 * @course GAM100F18
 * Copyright © 2018 DigiPen (USA) Corporation
 * @addtogroup Util
 * @{
 */
#include "gamelayer.h"

#include <stdlib.h>

/**
 * @brief Create new GameLayer
 * @param updateLower Update lower layers
 * @param drawLower   Draw lower layers
 */
GameLayer *GameLayer_new(bool updateLower, bool drawLower) {
    GameLayer *gLayer = malloc(sizeof(GameLayer));
    gLayer->scrMngr = ScreenMngr_new(gLayer);
    gLayer->objMngr = ObjectMngr_new(gLayer);
    gLayer->updateLower = updateLower;
    gLayer->drawLower = drawLower;
    return gLayer;
}

/**
 * @brief Delete GameLayer
 * @param gLayer GameLayer to delete
 */
void GameLayer_delete(GameLayer *gLayer) {
    ScreenMngr_delete(gLayer->scrMngr);
    ObjectMngr_delete(gLayer->objMngr);
}

/**
 * @brief Update GameLayer
 * @param gLayer GameLayer to update
 */
void GameLayer_update(GameLayer *gLayer) {
    ScreenMngr_update(gLayer->scrMngr);
    ObjectMngr_update(gLayer->objMngr);
}

/**
 * @brief Draw GameLayer
 * @param gLayer GameLayer to draw
 */
void GameLayer_draw(GameLayer *gLayer) {
    ObjectMngr_draw(gLayer->objMngr);
}

/// @}

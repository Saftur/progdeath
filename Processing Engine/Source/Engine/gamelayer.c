/**
 * @file gamelayer.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/16/18
 * @brief GameLayer implementation
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
    gLayer->lvlMngr = LevelMngr_new(gLayer);
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
    LevelMngr_delete(gLayer->lvlMngr);
    ObjectMngr_delete(gLayer->objMngr);
}

/**
 * @brief Update GameLayer
 * @param gLayer GameLayer to update
 */
void GameLayer_update(GameLayer *gLayer) {
    LevelMngr_update(gLayer->lvlMngr);
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

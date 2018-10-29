/**
 * @file screenmngr.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/26/18
 * @brief Screen manager
 * @addtogroup Engine
 * @{
 */
#pragma once

#include "list.h"
#include "screen.h"

typedef struct GameLayer GameLayer; ///< GameLayer forward declaration

/**
 * @brief Holds current screen data
 */
typedef struct ScreenMngr {
    List *screens;      ///< List of Screen s
    Screen *currScreen; ///< Current Screen
    Screen *nextScreen; ///< Next Screen

    GameLayer *gLayer; ///< Parent GameLayer
} ScreenMngr;

/**
 * @brief Create new ScreenMngr
 * @param gLayer Parent GameLayer
 * @return New ScreenMngr
 */
ScreenMngr *ScreenMngr_new(GameLayer *gLayer);
/**
 * @brief Delete ScreenMngr
 * @param scrMngr ScreenMngr to delete
 */
void ScreenMngr_delete(ScreenMngr *scrMngr);

/**
 * @brief Update ScreenMngr
 * @param scrMngr ScreenMngr to update
 */
void ScreenMngr_update(ScreenMngr *scrMngr);

/**
 * @brief Add Screen to ScreenMngr
 * @param scrMngr ScreenMngr to add to
 * @param screen   Screen to add
 */
void ScreenMngr_addScreen(ScreenMngr *scrMngr, Screen *screen);
/**
 * @brief Set next Screen
 * @param scrMngr   ScreenMngr to set on
 * @param screenName Name of Screen to set
 */
void ScreenMngr_setNextScreen(ScreenMngr *scrMngr, const char *screenName);

/// @}

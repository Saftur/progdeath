/**
 * @file screenmngr.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/26/18
 * @brief ScreenMngr implementation
 * @addtogroup Engine
 * @{
 */
#include "screenmngr.h"

#include <stdlib.h> // malloc
#include <string.h> // strcmp

#include "gamelayer.h"
#include "objectmngr.h"

/**
 * @brief Create new ScreenMngr
 * @param gLayer Parent GameLayer
 * @return New ScreenMngr
 */
ScreenMngr *ScreenMngr_new(GameLayer *gLayer) {
    ScreenMngr *scrMngr = malloc(sizeof(ScreenMngr));
    scrMngr->screens = List_new(10, Screen_delete);
    scrMngr->currScreen = NULL;
    scrMngr->nextScreen = NULL;
    scrMngr->gLayer = gLayer;
    return scrMngr;
}

/**
 * @brief Delete ScreenMngr
 * @param scrMngr ScreenMngr to delete
 */
void ScreenMngr_delete(ScreenMngr *scrMngr) {
    List_delete(scrMngr->screens);
    free(scrMngr);
}

/**
 * @brief Update ScreenMngr
 * @param scrMngr ScreenMngr to update
 */
void ScreenMngr_update(ScreenMngr *scrMngr) {
    if (scrMngr->nextScreen != scrMngr->currScreen) {
        if (scrMngr->currScreen)
            Screen_end(scrMngr->currScreen);
        ObjectMngr_deleteAll(scrMngr->gLayer->objMngr);
        scrMngr->currScreen = scrMngr->nextScreen;
        Screen_init(scrMngr->currScreen, scrMngr->gLayer->objMngr);
    }
}

/**
 * @brief Add Screen to ScreenMngr
 * @param scrMngr ScreenMngr to add to
 * @param screen   Screen to add
 */
void ScreenMngr_addScreen(ScreenMngr *scrMngr, Screen *screen) {
    List_push_back(scrMngr->screens, screen);
}
/**
 * @brief Set next Screen
 * @param scrMngr   ScreenMngr to set on
 * @param screenName Name of Screen to set
 */
void ScreenMngr_setNextScreen(ScreenMngr *scrMngr, const char *screenName) {
    for (unsigned i = 0; i < scrMngr->screens->size; i++) {
        Screen *screen = scrMngr->screens->items[i];
        if (strcmp(screen->name, screenName) == 0) {
            scrMngr->nextScreen = screen;
            return;
        }
    }
}

/// @}

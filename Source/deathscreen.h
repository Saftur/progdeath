/**
 * @file deathscreen.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/18/18
 * @brief Death screen screen
 * @addtogroup Game-Screens
 * @{
 */
#pragma once

typedef struct ObjectMngr ObjectMngr; ///< @brief ObjectMngr forward declaration

extern int end_score; ///< @brief End score
extern int end_time;  ///< @brief End time

/**
 * @brief Initialize Death Screen
 * @param objMngr ObjectMngr to load screen on
 */
void deathScreenInit(ObjectMngr *objMngr);

/// @}

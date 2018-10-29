/**
 * @file editorscreen.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/28/18
 * @brief Editor Screen
 * @addtogroup Game-Levels
 * @{
 */
#pragma once

#include <stdbool.h>

typedef struct ObjectMngr ObjectMngr; ///< @brief ObjectMngr forward declaration

/**
 * @brief Initialize Editor Screen
 * @param objMngr ObjectMngr to load level on
 */
void editorScreenInit(ObjectMngr *objMngr);

/// @}

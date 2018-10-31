/**
 * @file editorscreen.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/31/18
 * @brief Editor Screen
 * @addtogroup Game-Screens
 * @{
 */
#pragma once

#include <stdbool.h>
#include <C_Processing.h>

typedef struct ObjectMngr ObjectMngr; ///< @brief ObjectMngr forward declaration
typedef struct Screen Screen;         ///< @brief Screen forward declaration
typedef struct CodeBlock CodeBlock;   ///< @brief CodeBlock forward declaration

void setGrabbed(CodeBlock *block);
void addToBoard(CodeBlock *block, vec2_t pos);
int isOnList(vec2_t pos);

/**
 * @brief Initialize Editor Screen
 * @param objMngr ObjectMngr to load screen on
 */
void editorScreenInit(ObjectMngr *objMngr);
/**
 * @brief Update Editor Screen
 * @param screen Screen to update
 */
void editorScreenUpdate(Screen *screen);
/**
 * @brief End Editor Screen
 * @param objMngr ObjectMngr screen was loaded on
 */
void editorScreenEnd(ObjectMngr *objMngr);

/// @}

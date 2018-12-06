/**
 * @file engine.h
 * @author Arthur Bouvier (a.bouvier)
 * @brief Main Engine
 * @addtogroup Util
 * @{
 */
#pragma once

#include <stdbool.h>

#include <C_Processing.h>

#include "gamelayer.h"

typedef bool (*CollCheckFunc)(Component*, Component*);   ///< @brief Collision checking function signature
typedef void (*CollResolveFunc)(Component*, Component*); ///< @brief Collision resolving function signature

typedef void (*KeyCallbackFunc)(Key, KeyState, void*);
typedef void (*MouseCallbackFunc)(MouseButton, KeyState, void*);
typedef void (*WheelCallbackFunc)(float, float);

/**
 * @brief Contains Engine data
 */
typedef struct Engine {
    List *gLayers;        ///< @brief List of GameLayer s
    unsigned firstUpdate; ///< @brief Index of first GameLayer to update
    unsigned firstDraw;   ///< @brief Index of first GameLayer to draw

    List *collChecks;   ///< @brief 2D List of collision checking functions
    List *collResolves; /**< @brief 2D List of collision resolving functions
                         * 2D List s looks like this:
                         *                     type2
                         * Comp type |   1   |   2   |   3   |
                         *        1  | func  |       |       |
                         * type1  2  | func  | func  |       |
                         *        3  | func  | func  | func  |
                         */

    List *keyCallbacks;
    List *mouseCallbacks;
    List *wheelCallbacks;
} Engine;

extern Engine *engine; ///< @brief The primary game Engine object (must be initialized & deleted)

/**
 * @brief Initialize the Engine
 * @param numLayers Starting number of layers
 */
void Engine_init(unsigned numLayers);
/**
 * @brief Delete Engine
 */
void Engine_delete();

/**
 * @brief Update Engine
 */
void Engine_update();

/**
 * @brief Initialize GameLayer
 * @param layerNum    GameLayer index in array
 * @param updateLower Update lower layers
 * @param drawLower   Draw lower layers
 */
void Engine_initLayer(unsigned layerNum, bool updateLower, bool drawLower);
/**
 * @brief Delete GameLayer
 * @param layerNum GameLayer index in array
 */
void Engine_deleteLayer(unsigned layerNum);
/**
 * @brief Get a GameLayer
 * @param layerNum GameLayer index in array
 * @return GameLayer at given index
 */
GameLayer *Engine_getLayer(unsigned layerNum);

/**
 * @brief Initialize list of collision checking & resolving functions
 * @param numComps Total number of Component s
 */
void Engine_initCollFuncList(unsigned numComps);
/**
 * @brief Add a collision checking function to the list
 * @param type1 Type of first Component
 * @param type2 Type of second Component
 * @param func      Collision checking function
 */
void Engine_addCollCheckFunc(unsigned type1, unsigned type2, CollCheckFunc func);
/**
 * @brief Get a collision checking function from the list
 * @param type1 Type of first Component
 * @param type2 Type of second Component
 * @return Collision checking function
 */
CollCheckFunc Engine_getCollCheckFunc(unsigned type1, unsigned type2);
/**
 * @brief Add a collision resolving function to the list
 * @param type1 Type of first Component
 * @param type2 Type of second Component
 * @param func      Collision resolving function
 */
void Engine_addCollResolveFunc(unsigned type1, unsigned type2, CollResolveFunc func);
/**
 * @brief Get a collision resolving function from the list
 * @param type1 Type of first Component
 * @param type2 Type of second Component
 * @return Collision resolving function
 */
CollResolveFunc Engine_getCollResolveFunc(unsigned type1, unsigned type2);

void Engine_addKeyCallback(KeyCallbackFunc func, void *upvalue);
void Engine_removeKeyCallback(KeyCallbackFunc func, void *upvalue);
void Engine_addMouseCallback(MouseCallbackFunc func, void *upvalue);
void Engine_removeMouseCallback(MouseCallbackFunc func, void *upvalue);
void Engine_addWheelCallback(WheelCallbackFunc func, void *upvalue);
void Engine_removeWheelCallback(WheelCallbackFunc func, void *upvalue);

/// @}

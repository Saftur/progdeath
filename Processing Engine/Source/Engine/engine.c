/**
 * @file engine.c
 * @author Arthur Bouvier (a.bouvier)
 * @brief Engine implementation
 * @course GAM100F18
 * Copyright � 2018 DigiPen (USA) Corporation
 * @addtogroup Util
 * @{
 */
#include "engine.h"

#include <stdlib.h>

#include "component.h"
#include "util.h"
#include "focus.h"

Engine *engine = NULL;

static void keyCallback(Key key, KeyState state);
static void mouseCallback(MouseButton button, KeyState state);
static void wheelCallback(float xoffset, float yoffset);

typedef struct KeyCallback {
    KeyCallbackFunc func;
    void *upvalue;
} KeyCallback;

static KeyCallback *KeyCallback_new(KeyCallbackFunc func, void *upvalue) {
    KeyCallback *new = malloc(sizeof(KeyCallback));
    new->func = func;
    new->upvalue = upvalue;
    return new;
}

typedef struct MouseCallback {
    MouseCallbackFunc func;
    void *upvalue;
} MouseCallback;

static MouseCallback *MouseCallback_new(MouseCallbackFunc func, void *upvalue) {
    MouseCallback *new = malloc(sizeof(MouseCallback));
    new->func = func;
    new->upvalue = upvalue;
    return new;
}

typedef struct WheelCallback {
    WheelCallbackFunc func;
    void *upvalue;
} WheelCallback;

static WheelCallback *WheelCallback_new(WheelCallbackFunc func, void *upvalue) {
    WheelCallback *new = malloc(sizeof(WheelCallback));
    new->func = func;
    new->upvalue = upvalue;
    return new;
}

/**
 * @brief Initialize the Engine
 * @param numLayers Starting number of layers
 */
void Engine_init(unsigned numLayers) {
    Engine_delete();

    engine = malloc(sizeof(Engine));
    engine->gLayers = List_new(numLayers, NULL, GameLayer_delete);
    for (unsigned i = 0; i < numLayers; i++)
        List_push_back(engine->gLayers, NULL);
    engine->firstUpdate = 0;
    engine->firstDraw = 0;

    engine->collChecks = NULL;

    engine->keyCallbacks = List_new(10, NULL, free);
    engine->mouseCallbacks = List_new(10, NULL, free);
    engine->wheelCallbacks = List_new(10, NULL, free);

    setKeyboardInputCallback(keyCallback);
    setMouseInputCallback(mouseCallback);
    setMouseWheelCallback(wheelCallback);

    initFocus();
}

/**
 * @brief Delete Engine
 */
void Engine_delete() {
    if (!engine) return;
    List_delete(engine->gLayers);
    free(engine);

    endFocus();
}

/**
 * @brief Update Engine
 */
void Engine_update() {
    for (unsigned i = engine->firstUpdate; i < engine->gLayers->size; i++) {
        GameLayer *gLayer = engine->gLayers->items[i];
        if (gLayer)
            GameLayer_update(gLayer);
    }

    for (unsigned i = engine->firstDraw; i < engine->gLayers->size; i++) {
        GameLayer *gLayer = engine->gLayers->items[i];
        if (gLayer)
            GameLayer_draw(gLayer);
    }
}

/**
 * @brief Initialize GameLayer
 * @param layerNum    GameLayer index in array
 * @param updateLower Update lower layers
 * @param drawLower   Draw lower layers
 */
void Engine_initLayer(unsigned layerNum, bool updateLower, bool drawLower) {
    if (layerNum >= engine->gLayers->size) {
        List_reserve(engine->gLayers, layerNum+1);
        for (unsigned i = engine->gLayers->size; i < engine->gLayers->max; i++)
            List_push_back(engine->gLayers, NULL);
    }

    engine->gLayers->items[layerNum] = GameLayer_new(updateLower, drawLower);

    if (!updateLower && layerNum > engine->firstUpdate)
        engine->firstUpdate = layerNum;
    if (!drawLower && layerNum > engine->firstDraw)
        engine->firstDraw = layerNum;
}

/**
 * @brief Delete GameLayer
 * @param layerNum GameLayer index in array
 */
void Engine_deleteLayer(unsigned layerNum) {
    if (layerNum >= engine->gLayers->size || !engine->gLayers->items[layerNum])
        return;

    GameLayer **gLayer = (GameLayer**)(engine->gLayers->items+layerNum);

    // Update `engine->firstUpdate` value
    if (!(*gLayer)->updateLower && layerNum == engine->firstUpdate) {
        for (unsigned i = layerNum; i <= layerNum; i--) {
            GameLayer *gLayer2 = engine->gLayers->items[i];
            if (gLayer2 && !gLayer2->updateLower) {
                engine->firstUpdate = i;
                break;
            }
        }
    }
    // Update `engine->firstDraw` value
    if (!(*gLayer)->drawLower && layerNum == engine->firstDraw) {
        for (unsigned i = layerNum; i <= layerNum; i--) {
            GameLayer *gLayer2 = engine->gLayers->items[i];
            if (gLayer2 && !gLayer2->drawLower) {
                engine->firstDraw = i;
                break;
            }
        }
    }

    GameLayer_delete(*gLayer);
    *gLayer = NULL;
}

/**
 * @brief Get a GameLayer
 * @param layerNum GameLayer index in array
 */
GameLayer *Engine_getLayer(unsigned layerNum) {
    if (layerNum >= engine->gLayers->size)
        return NULL;
    return engine->gLayers->items[layerNum];
}

/**
 * @brief Initialize list of collision checking & resolving functions
 * @param numComps Total number of Component s
 */
void Engine_initCollFuncList(unsigned numComps) {
    engine->collChecks = List_new(numComps, List_copy, List_delete);
    engine->collResolves = List_new(numComps, List_copy, List_delete);
    for (unsigned i = 0; i < numComps; i++) {
        // Avoid duplicate entries in 2D list
        unsigned listSize = i+1;
        List *checks = List_new(listSize, NULL, NULL);
        List *resolves = List_new(listSize, NULL, NULL);
        List_push_back(engine->collChecks, checks);
        List_push_back(engine->collResolves, resolves);
        for (unsigned j = 0; j < listSize; j++) {
            List_push_back(checks, NULL);
            List_push_back(resolves, NULL);
        }
    }
}

/**
 * @brief Get a pointer to the CollCheckFunc of given types
 * @param type1 Type of first Component
 * @param type2 Type of second Component
 * @return Pointer to CollCheckFunc in 2D List
 */
static CollCheckFunc *getCollCheckFuncPointer(unsigned type1, unsigned type2) {
    if (!type1 || !type2) return NULL;
    type1--; type2--;
    if (type2 > type1)
        swapUnsigned(&type1, &type2);
    return &(((List*)engine->collChecks->items[type1])->items[type2]);
}

/**
 * @brief Add a collision checking function to the list
 * @param type1 Type of first Component
 * @param type2 Type of second Component
 * @param func      Collision checking function
 */
void Engine_addCollCheckFunc(unsigned type1, unsigned type2, CollCheckFunc func) {
    CollCheckFunc *funcPtr = getCollCheckFuncPointer(type1, type2);
    if (funcPtr)
        *funcPtr = func;
}

/**
 * @brief Get a collision checking function from the list
 * @param type1 Type of first Component
 * @param type2 Type of second Component
 * @return Collision checking function
 */
CollCheckFunc Engine_getCollCheckFunc(unsigned type1, unsigned type2) {
    CollCheckFunc *funcPtr = getCollCheckFuncPointer(type1, type2);
    if (funcPtr)
        return *funcPtr;
    else return NULL;
}

/**
 * @brief Get a pointer to the CollCheckFunc of given types
 * @param type1 Type of first Component
 * @param type2 Type of second Component
 * @return Pointer to CollCheckFunc in 2D List
 */
static CollCheckFunc *getCollResolveFuncPointer(unsigned type1, unsigned type2) {
    if (!type1 || !type2) return NULL;
    type1--; type2--;
    if (type2 > type1)
        swapUnsigned(&type1, &type2);
    return &(((List*)engine->collResolves->items[type1])->items[type2]);
}

/**
 * @brief Add a collision resolving function to the list
 * @param type1 Type of first Component
 * @param type2 Type of second Component
 * @param func      Collision resolving function
 */
void Engine_addCollResolveFunc(unsigned type1, unsigned type2, CollResolveFunc func) {
    CollCheckFunc *funcPtr = getCollResolveFuncPointer(type1, type2);
    if (funcPtr)
        *funcPtr = func;
}

/**
 * @brief Get a collision resolving function from the list
 * @param type1 Type of first Component
 * @param type2 Type of second Component
 * @return Collision resolving function
 */
CollResolveFunc Engine_getCollResolveFunc(unsigned type1, unsigned type2) {
    CollCheckFunc *funcPtr = getCollResolveFuncPointer(type1, type2);
    if (funcPtr)
        return *funcPtr;
    else return NULL;
}

void Engine_addKeyCallback(KeyCallbackFunc func, void *upvalue) {
    List_push_back(engine->keyCallbacks, KeyCallback_new(func, upvalue));
}

void Engine_removeKeyCallback(KeyCallbackFunc func, void *upvalue) {
    for (unsigned i = 0; i < engine->keyCallbacks->size; i++) {
        KeyCallback *callback = engine->keyCallbacks->items[i];
        if (callback->func == func && callback->upvalue) {
            List_remove(engine->keyCallbacks, i);
            return;
        }
    }
}

void Engine_addMouseCallback(MouseCallbackFunc func, void *upvalue) {
    List_push_back(engine->mouseCallbacks, MouseCallback_new(func, upvalue));
}

void Engine_removeMouseCallback(MouseCallbackFunc func, void *upvalue) {
    for (unsigned i = 0; i < engine->mouseCallbacks->size; i++) {
        MouseCallback *callback = engine->mouseCallbacks->items[i];
        if (callback->func == func && callback->upvalue) {
            List_remove(engine->mouseCallbacks, i);
            return;
        }
    }
}

void Engine_addWheelCallback(WheelCallbackFunc func, void *upvalue) {
    List_push_back(engine->wheelCallbacks, WheelCallback_new(func, upvalue));
}

void Engine_removeWheelCallback(WheelCallbackFunc func, void *upvalue) {
    for (unsigned i = 0; i < engine->wheelCallbacks->size; i++) {
        WheelCallback *callback = engine->wheelCallbacks->items[i];
        if (callback->func == func && callback->upvalue) {
            List_remove(engine->wheelCallbacks, i);
            return;
        }
    }
}

static void keyCallback(Key key, KeyState state) {
    for (unsigned i = 0; i < engine->keyCallbacks->size; i++) {
        KeyCallback *callback = engine->keyCallbacks->items[i];
        callback->func(key, state, callback->upvalue);
    }
}

static void mouseCallback(MouseButton button, KeyState state) {
    for (unsigned i = 0; i < engine->mouseCallbacks->size; i++) {
        MouseCallback *callback = engine->mouseCallbacks->items[i];
        callback->func(button, state, callback->upvalue);
    }
}

static void wheelCallback(float xoffset, float yoffset) {
    for (unsigned i = 0; i < engine->wheelCallbacks->size; i++) {
        WheelCallback *callback = engine->wheelCallbacks->items[i];
        callback->func(xoffset, yoffset, callback->upvalue);
    }
}

/// @}

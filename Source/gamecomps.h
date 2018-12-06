/**
 * @file gamecomps.h
 * @author Arthur Bouvier (a.bouvier)
 * @brief Game-side Component type ids
 * @addtogroup Game-Components
 * @{
 */
#pragma once

#include <Engine/component.h>

/**
 * @brief Game Component type ids
 */
enum _Game_Comp_TypeId {
    ENTITY = NUM_ENGINE_COMPS,
    CODEBLOCKLIST,
    CODEBLOCKBOARD,
    CBGRABCOMP,

    NUM_TOTAL_COMPS
};

/// @}

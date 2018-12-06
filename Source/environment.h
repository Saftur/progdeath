/**
 * @file environment.h
 * @author Connor Meyers(connor.meyers)
 * @brief Environment placement and types
 * @course GAM100F18
 * Copyright © 2018 DigiPen (USA) Corporation
 * @addtogroup Components
 * @{
 */
#pragma once

typedef enum EnvironmentType
{
    ENV_GRASS,
    ENV_MOUNTAIN,
    ENV_FIRE,
    ENV_WATER,
    ENV_TREE,
    ENVIRONMENT_COUNT,
    ENVIRONMENT_FIRST = ENV_MOUNTAIN,
}EnvironmentType;

/// @}
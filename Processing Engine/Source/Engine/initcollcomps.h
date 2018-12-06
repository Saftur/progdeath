/**
 * @file initcollcomps.h
 * @author Arthur Bouvier (a.bouvier)
 * @brief Initialize collision Component vars
 * @addtogroup Util
 * @{
 */
#pragma once

/**
 * @brief Sets variables to Components depending on their types
 * @param typeId1 Type id of var1
 * @param var1    First var
 * @param var2    Second var
 * @param comp1   First Component
 * @param comp2   Second Component
 */
#define INITCOLLCOMPS(typeId1, var1, var2, comp1, comp2) \
if (comp1->typeId == typeId1) { \
    var1 = comp1; \
    var2 = comp2; \
} else { \
    var1 = comp2; \
    var2 = comp1; \
}

/// @}

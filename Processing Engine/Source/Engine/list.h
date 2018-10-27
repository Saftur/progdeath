/**
 * @file list.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/18/18
 * @brief C++-vector-style array
 * @addtogroup Util
 * @{
 */
#pragma once

#include <stdlib.h> // size_t, malloc

/**
 * @brief C++-vector-style array
 */
typedef struct List {
    void **items;   ///< @brief Array of items
    size_t max;     ///< @brief Current allocated size of array
    size_t size;    ///< @brief Number of items in array

    void (*delFunc)(void *item); ///< @brief Delete function for items
} List;

/**
 * @brief Create new List
 * @param max     Starting allocation size
 * @param delFunc Item delete function
 * @return New List
 */
List *List_new(size_t max, void (*delFunc)(void *item));
/**
 * @brief Delete List
 * @param list List to delete
 */
void List_delete(List *list);

/**
 * @brief Re-allocate space for List
 * @param list List to reserve size on
 * @param size New allocated size
 */
void List_reserve(List *list, size_t size);
/**
 * @brief Add item to list
 * @param list List to add to
 * @param item Item to add
 */
void List_push_back(List *list, void *item);
/**
 * @brief Delete last item in List
 * @param list List to delete from
 */
void List_pop(List *list);
/**
 * @brief Remove an item from the list
 * @param list List to delete from
 * @param i    Index of item to delete
 */
void List_remove(List *list, unsigned i);
/**
 * @brief Delete all items in List
 * @param list List to delete from
 */
void List_clear(List *list);

/// @}

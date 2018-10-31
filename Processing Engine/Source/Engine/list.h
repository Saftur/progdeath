/**
 * @file list.h
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/29/18
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

    void *(*copyFunc)(void *item); ///< @brief Copy function for items
    void (*delFunc)(void *item);   ///< @brief Delete function for items
} List;

/**
 * @brief Create new List
 * @param max      Starting allocation size
 * @param copyFunc Item copy function
 * @param delFunc  Item delete function
 * @return New List
 */
List *List_new(size_t max, void *(*copyFunc)(void *item), void (*delFunc)(void *item));
/**
 * @brief Copy a List
 * @param list List to copy
 * @return Copy of List
 */
List *List_copy(List *list);
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
 * @brief Resize array, setting all new values to given value
 * @param list List to resize
 * @param size New size
 * @param item Value to initialize new items to
 */
void List_resize(List *list, size_t size, void *item);
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
 * @brief Remove an item from the list without calling delFunc
 * @param list List to delete from
 * @param i    Index of item to delete
 */
#define List_remove_nodelete(list, i) {void *delFunc = list->delFunc; list->delFunc = NULL; List_remove(list, i); list->delFunc = delFunc;}
/**
 * @brief Delete all items in List
 * @param list List to delete from
 */
void List_clear(List *list);

/**
 * @brief Find an item in a List
 * @param list  List to search in
 * @param item  Item to search for
 * @param equal Function to check if equal (regular check if NULL)
 * @return Pointer to item
 */
void **List_find(List *list, void *item, int (*equal)(void*, void*));

/// @}

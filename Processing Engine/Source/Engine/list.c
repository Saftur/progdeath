/**
 * @file list.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/29/18
 * @brief List implementation
 * @addtogroup Util
 * @{
 */
#include "list.h"

#include <string.h> // memcpy

/**
 * @brief Create new List
 * @param max     Starting allocation size
 * @param delFunc Item delete function
 * @return New List
 */
List *List_new(size_t max, void (*delFunc)(void *item)) {
    List *list = malloc(sizeof(List));
    list->max = max;
    list->size = 0;
    list->items = malloc(max * sizeof(void*));
    list->delFunc = delFunc;
    return list;
}

/**
 * @brief Delete List
 * @param list List to delete
 */
void List_delete(List *list) {
    List_clear(list);
    free(list);
}

/**
 * @brief Re-allocate space for List
 * @param list List to reserve size on
 * @param size New allocated size
 */
void List_reserve(List *list, size_t size) {
    if (size > list->max) {
        void **oldItems = list->items;
        list->max = size;
        list->items = malloc(list->max * sizeof(void*));
        memcpy(list->items, oldItems, list->size * sizeof(void*));
        free(oldItems);
    }
}

/**
 * @brief Resize array, setting all new values to given value
 * @param list List to resize
 * @param size New size
 * @param item Value to initialize new items to
 */
void List_resize(List *list, size_t size, void *item) {
    if (size > list->size) {
        List_reserve(list, size);
        for (unsigned i = list->size; i < size; i++)
            list->items[i] = item;
        list->size = size;
    } else if (size < list->size) {
        for (unsigned i = size; i < list->size; i++)
            if (list->delFunc)
                list->delFunc(list->items[i]);
        list->size = size;
    }
}

/**
 * @brief Add item to list
 * @param list List to add to
 * @param item Item to add
 */
void List_push_back(List *list, void *item) {
    if (list->size >= list->max)
        List_reserve(list, list->max * 2);
    list->items[list->size++] = item;
}

/**
 * @brief Delete last item in List
 * @param list List to delete from
 */
void List_pop(List *list) {
    if (list->size == 0) return;
    list->size--;
    if (list->delFunc)
        list->delFunc(list->items[list->size]);
}

/**
 * @brief Remove an item from the list
 * @param list List to delete from
 * @param i    Index of item to delete
 */
void List_remove(List *list, unsigned i) {
    if (i >= list->size)
        return;
    if (list->delFunc)
        list->delFunc(list->items[i]);
    list->size--;
    if (list->size > 0)
        memmove(list->items+i, list->items+i+1, (list->size-i) * sizeof(void*));
}

/**
 * @brief Delete all items in List
 * @param list List to delete from
 */
void List_clear(List *list) {
    for (unsigned i = 0; i < list->size; i++)
        if (list->delFunc)
            list->delFunc(list->items[i]);
    list->size = 0;
}

/**
 * @brief Find an item in a List
 * @param list  List to search in
 * @param item  Item to search for
 * @param equal Function to check if equal (regular check if NULL)
 * @return Pointer to item in list
 */
void **List_find(List *list, void *item, int (*equal)(void*, void*)) {
    for (unsigned i = 0; i < list->size; i++)
        if (equal ? equal(list->items[i], item) : list->items[i] == item)
            return list->items+i;
    return NULL;
}

/// @}

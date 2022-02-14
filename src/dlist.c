/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "dlist.h"

#include <stdlib.h>
#include <string.h>



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void DList_init(Dlist* list, void (* destroy)(void* data))
{
    // Init the list
    list->size = 0;
    list->destroy = destroy;
    list->head = NULL;
    list->tail = NULL;
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void DList_destroy(Dlist* list)
{
    void* data;

    // Remove each element
    while (DList_size(list) > 0)
    {
        if (DList_remove(list, DList_tail(list), (void**) &data) == 0 && list->destroy != NULL)
        {
            // Call a user-defined function to free dynamically allocated data
            list->destroy(data);
        }
    }

    // No operations are allowed now, but clear the structure as a precaution
    memset(list, 0, sizeof(Dlist));
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
int DList_insertNext(Dlist* list, DlistItem* item, const void* data)
{
    DlistItem* new_element;

    // Do not allow a NULL element unless the list is empty
    if (item == NULL && DList_size(list) != 0)
    {
        return -1;
    }

    // Allocate storage for the element
    if ((new_element = (DlistItem*) malloc(sizeof(DlistItem))) == NULL)
    {
        return -1;
    }

    // Insert the new element into the list
    new_element->data = (void*) data;

    // Handle insertion when the list is empty
    if (DList_size(list) == 0)
    {
        list->head = new_element;
        list->head->prev = NULL;
        list->head->next = NULL;
        list->tail = new_element;
    }

    // Handle insertion when the list is not empty
    else
    {
        new_element->next = item->next;
        new_element->prev = item;

        if (item->next == NULL)
        {
            list->tail = new_element;
        }
        else
        {
            item->next->prev = new_element;
        }

        item->next = new_element;
    }

    // Adjust the size of the list to account for the inserted element
    list->size++;

    return 0;
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
int DList_insertPrev(Dlist* list, DlistItem* item, const void* data)
{
    DlistItem* new_item;

    // Do not allow a NULL element unless the list is empty
    if (item == NULL && DList_size(list) != 0)
    {
        return -1;
    }

    // Allocate storage to be managed by the abstract data type
    if ((new_item = (DlistItem*) malloc(sizeof(DlistItem))) == NULL)
    {
        return -1;
    }

    // Insert the new element into the list
    new_item->data = (void*) data;

    // Handle insertion when the list is empty
    if (DList_size(list) == 0)
    {
        list->head = new_item;
        list->head->prev = NULL;
        list->head->next = NULL;
        list->tail = new_item;
    }

    // Handle insertion when the list is not empty
    else
    {
        new_item->next = item;
        new_item->prev = item->prev;

        if (item->prev == NULL)
        {
            list->head = new_item;
        }
        else
        {
            item->prev->next = new_item;
        }

        item->prev = new_item;
    }

    // Adjust the size of the list to account for the new element
    list->size++;

    return 0;
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
int DList_remove(Dlist* list, DlistItem* item, void** data)
{
    // Do not allow a NULL element or removal from an empty list
    if (item == NULL || DList_size(list) == 0)
    {
        return -1;
    }

    // Remove the element from the list
    *data = item->data;

    // Handle removal from the head of the list
    if (item == list->head)
    {
        list->head = item->next;

        if (list->head == NULL)
        {
            list->tail = NULL;
        }
        else
        {
            item->next->prev = NULL;
        }
    }

    // Handle removal from other than the head of the list
    else
    {
        item->prev->next = item->next;

        if (item->next == NULL)
        {
            list->tail = item->prev;
        }
        else
        {
            item->next->prev = item->prev;
        }
    }

    // Free the storage allocated by the abstract data type
    free(item);

    // Adjust the size of the list to account for the removed element
    list->size--;

    return 0;
}

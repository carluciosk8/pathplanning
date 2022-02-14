/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "list.h"

#include <stdlib.h>
#include <string.h>



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void List_init(List* list, void (*destroy)(void* data))
{
    // Initialize the list
    list->size = 0;
    list->destroy = destroy;
    list->head = NULL;
    list->tail = NULL;
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void List_destroy(List* list)
{
    void* data;

    // Remove each element
    while (List_size(list) > 0)
    {
        if (List_removeNext(list, NULL, (void**)&data) == 0 && list->destroy != NULL)
        {
            //*  Call a user-defined function to free dynamically allocated data
            list->destroy(data);
        }
    }

    // No operations are allowed now, but clear the structure as a precaution
    memset(list, 0, sizeof(List));
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
int List_insertNext(List* list, ListItem* element, const void* data)
{
    ListItem* new_element;

    // Allocate storage for the element
    if ((new_element = (ListItem*)malloc(sizeof(ListItem))) == NULL)
    {
        return -1;
    }

    // Insert the element into the list
    new_element->data = (void*)data;

    // Handle insertion at the head of the list
    if (element == NULL)
    {
        if (List_size(list) == 0)
        {
            list->tail = new_element;
        }

        new_element->next = list->head;
        list->head = new_element;
    }

    // Handle insertion somewhere other than at the head
    else
    {
        if (element->next == NULL)
        {
            list->tail = new_element;
        }

        new_element->next = element->next;
        element->next = new_element;
    }

    // Adjust the size of the list to account for the inserted element
    list->size++;

    return 0;
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
int List_removeNext(List* list, ListItem* element, void** data)
{
    ListItem* old_element;

    // Do not allow removal from an empty list
    if (List_size(list) == 0)
    {
        return -1;
    }

    // Handle removal from the head of the list
    if (element == NULL)
    {
        *data = list->head->data;
        old_element = list->head;
        list->head = list->head->next;

        if (List_size(list) == 0)
        {
            list->tail = NULL;
        }
    }

    // Handle removal from somewhere other than the head
    else
    {
        if (element->next == NULL)
        {
            return -1;
        }

        *data = element->next->data;
        old_element = element->next;
        element->next = element->next->next;

        if (element->next == NULL)
        {
            list->tail = element;
        }
    }

    // Free the storage allocated by the abstract data type
    free(old_element);

    // Adjust the size of the list to account for the removed element
    list->size--;

    return 0;
}

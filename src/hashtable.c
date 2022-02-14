/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "hashtable.h"

#include <stdlib.h>
#include <string.h>

#include "list.h"



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
int HashTable_init(HashTable* htbl,int buckets,int (* h)(const void* key),int (* match)(const void* key1, const void* key2),void (* destroy)(void* data))
{
    int i;

    // Allocate space for the hash table.
    if ((htbl->table = (List*)malloc(buckets * sizeof(List))) == NULL)
    {
        return -1;
    }

    // Initialize the buckets.
    htbl->buckets = buckets;

    for (i = 0; i < htbl->buckets; i++)
    {
        List_init(&htbl->table[i], destroy);
    }

    // Encapsulate the functions.
    htbl->h = h;
    htbl->match = match;
    htbl->destroy = destroy;

    // Initialize the number of elements in the table.
    htbl->size = 0;

    return 0;
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void HashTable_destroy(HashTable* htbl)
{
    int i;

    // Destroy each bucket.
    for (i = 0; i < htbl->buckets; i++)
    {
        List_destroy(&htbl->table[i]);
    }

    // Free the storage allocated for the hash table.
    free(htbl->table);

    // No operations are allowed now, but clear the structure as a precaution.
    memset(htbl, 0, sizeof(HashTable));
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
int HashTable_insert(HashTable* htbl, const void* data)
{
    void* temp;
    int bucket;
    int retval;

    // Do nothing if the data is already in the table.
    temp = (void*) data;

    if (HashTable_lookup(htbl, &temp) == 0)
    {
        return 1;
    }

    // Hash the key.
    bucket = htbl->h(data) % htbl->buckets;

    // Insert the data into the bucket.
    retval = List_insertNext(&htbl->table[bucket], NULL, data);
    if (retval == 0)
    {
        htbl->size++;
    }

    return retval;
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
int HashTable_remove(HashTable* htbl, void** data)
{
    ListItem* element;
    ListItem* prev;
    int bucket;

    // Hash the key.
    bucket = htbl->h(*data) % htbl->buckets;

    // Search for the data in the bucket.
    prev = NULL;

    for ( element = List_head(&htbl->table[bucket]); element != NULL; element = List_next(element) )
    {
        if (htbl->match(*data, List_data(element)))
        {
            // Remove the data from the bucket.
            if (List_removeNext(&htbl->table[bucket], prev, data) == 0)
            {
                htbl->size--;
                return 0;
            }
            else
            {
                return -1;
            }
        }
        prev = element;
    }

    // Return that the data was not found.
    return -1;
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
int HashTable_lookup(const HashTable* htbl, void** data)
{
    ListItem* element;
    int bucket;

    // Hash the key.
    bucket = htbl->h(*data) % htbl->buckets;

    // Search for the data in the bucket.
    for (element = List_head(&htbl->table[bucket]); element != NULL; element = List_next(element))
    {
        if (htbl->match(*data, List_data(element)))
        {
            // Pass back the data from the table.
            *data = List_data(element);
            return 0;
        }
    }

    // Return that the data was not found.
    return -1;
}

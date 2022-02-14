/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__


#include "list.h"


typedef struct
{
    List*  table;
    int    buckets;
    int    size;
    int    (*h)       (const void *key);
    int    (*match)   (const void *key1, const void *key2);
    void   (*destroy) (void *data);
}
HashTable;


int     HashTable_init    (HashTable* htbl, int buckets,
                           int  (*h) (const void* key),
                           int  (*match) (const void* key1, const void* key2),
                           void (*destroy) (void* data));
void    HashTable_destroy (HashTable* htbl);
int     HashTable_insert  (HashTable* htbl, const void* data);
int     HashTable_remove  (HashTable* htbl, void** data);
int     HashTable_lookup  (const HashTable* htbl, void** data);

#define HashTable_size(htbl) ((htbl)->size)


#endif

/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __NODETABLE_H_
#define __NODETABLE_H_


#include "hashtable.h"
#include "node.h"
#include "waypoint.h"


typedef HashTable NodeTable;

#define NODETABLE_SIZE 100000

#define NodeTable_init(table)          HashTable_init(table, NODETABLE_SIZE, Node_hash, Node_match, Node_destroy)
#define NodeTable_destroy              HashTable_destroy
#define NodeTable_insert               HashTable_insert
#define NodeTable_remove               HashTable_remove
#define NodeTable_size                 HashTable_size
#define NodeTable_lookup(table, node)  HashTable_lookup(table, (void**) node)

bool NodeTable_getNode     (NodeTable* table, Node** node, Waypoint* loc);


#endif

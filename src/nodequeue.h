/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __NODEQUEUE_H_
#define __NODEQUEUE_H_


#include "heap.h"
#include "node.h"


typedef Heap NodeQueue;


#define NodeQueue_init(queue)          Heap_init(queue, Node_compare, Node_destroy)
#define NodeQueue_pop(queue, node_obj) Heap_extract(queue, (void**) node_obj)
#define NodeQueue_size                 Heap_size
#define NodeQueue_push                 Heap_insert
#define NodeQueue_empty(queue)         (Heap_size(queue) == 0)


#endif

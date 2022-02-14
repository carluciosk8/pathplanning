/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __NODE_H_
#define __NODE_H_

#include "bool.h"


typedef struct Node
{
    float cost;
    float estimate;
    void* info;
    bool  open;
    bool  closed;
    struct Node* parent;
}
Node;


#define Node_data(node)     ( (node)->info   )
#define Node_parent(node)   ( (node)->parent )
#define Node_inOpen(node)   ( (node)->open   )
#define Node_inClosed(node) ( (node)->closed )

void  Node_print(Node* node);

void  Node_destroy(void* node );
int   Node_compare(const void* node1, const void* node2);
int   Node_match(const void* node1, const void* node2);
int   Node_hash(const void* node);


#endif

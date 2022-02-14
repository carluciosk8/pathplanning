/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "nodetable.h"

#include <stdio.h>
#include <stdlib.h>



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
bool NodeTable_getNode(NodeTable* table, Node** node, Waypoint* loc)
{
    Node* tmpNode = malloc( sizeof(Node) );
    Node* tmp = tmpNode;
    bool r = false;

    Node_data(tmpNode) = loc;

    if ( NodeTable_lookup(table, &tmpNode) == 0 )
    {
        free(tmp);
        *node = tmpNode;
    }
    else
    {
        *node = tmpNode;
        r = true;

        Node_data(*node) = loc;
        Node_inOpen(*node) = false;
        Node_inClosed(*node) = false;
        NodeTable_insert(table, *node);
    }

    return r;
}

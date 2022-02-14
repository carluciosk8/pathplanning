/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "path.h"

#include <stdio.h>


//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void Path_create(Path *path, Node* node)
{
    Node* parent_node;
    DlistItem* element;


    DList_insertPrev(path, NULL, node);
    element = DList_tail(path);
    parent_node = Node_parent(node);

    while ( parent_node != NULL )
    {
        DList_insertPrev(path, element, parent_node);
        parent_node = Node_parent(parent_node);
        element = DList_prev(element);
    }
}

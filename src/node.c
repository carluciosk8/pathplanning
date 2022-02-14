/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "node.h"

#include <stdio.h>
#include <stdlib.h>

#include "bool.h"
#include "waypoint.h"
#include "astar.h"
#include "world.h"



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void Node_destroy(void* node )
{
    free( (Node*) node );
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
int  Node_compare(const void* node1, const void* node2)
{
    float f1 = f((Node*) node1);
    float f2 = f((Node*) node2);

    if ( f1 > f2 )
    {
        return -1;
    }
    else
    if ( f1 < f2 )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
int  Node_match(const void* node1, const void* node2)
{
    return Waypoint_match((Waypoint*) Node_data((Node*) node1), (Waypoint*) Node_data((Node*) node2));
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
int Node_hash(const void* node)
{
    Waypoint* loc;

    loc = (Waypoint*) Node_data( (Node*) node);

    return ( World_width(&g_world)*(loc->y) + (loc->x) );
}

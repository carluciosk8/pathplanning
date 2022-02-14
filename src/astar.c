/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "astar.h"

#include <stdio.h>
#include <stdio.h>
#include <math.h>

#include "world.h"


void Astar_expand    (Astar* astar, Node* node, Node* goal);
void Astar_expandOne (Astar* astar, Node* node, Node* goal, int x, int y);



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void Astar_create(Astar* astar)
{
    astar->path_found        = false;
    astar->iterations        = 0;
    astar->allocated_nodes   = 2;
    astar->revisited_nodes   = 0;
    astar->open_pqueue       = malloc(sizeof(NodeQueue));
    astar->closed_table      = malloc(sizeof(NodeTable));
    astar->solution          = malloc(sizeof(Path));

    switch (g_world.heuristic)
    {
        case 0:  astar->estimate = Waypoint_euclideanDistance; break;
        case 1:  astar->estimate = Waypoint_diagshortDistance; break;
        case 2:  astar->estimate = Waypoint_manhattanDistance; break;
        default: astar->estimate = NULL;
    }

    // initialize result, path, open pqueue and closed table
    NodeQueue_init(astar->open_pqueue);
    NodeTable_init(astar->closed_table);
    Path_init(astar->solution);
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
bool Astar_search(Astar* astar, Node* start, Node* goal)
{
    Node*      node;

    // prepare start node
    g(start) = 0;
    h(start) = Astar_estimate(astar, start, goal);
    Node_inClosed(start) = false;
    Node_parent(start) = NULL;

    // push start node on open
    Node_inOpen(start) = true;
    NodeQueue_push(astar->open_pqueue, start);

    // draw open node
    World_drawCell(&g_world, Node_data(start), WORLD_RGB_OPEN);

    // main loop: process open queue while itsn't empty
    while ( !NodeQueue_empty(astar->open_pqueue) )
    {
        // pop a node from open, this is the best f(node) value
        NodeQueue_pop(astar->open_pqueue, &node);
        Node_inOpen(node) = false;

        // draw pop node on the world
        World_drawCell(&g_world, Node_data(node), WORLD_RGB_POP);

        // if node is goal then success
        if ( Node_match(node, goal) )
        {
            // construct the path backward
            Path_create(astar->solution, node);

            // draw path
            World_drawPath(&g_world, astar->solution, WORLD_RGB_PATH);

            // return success
            Astar_pathFound(astar) = true;
            return true;
        }

        // expand node and put all they children in open
        Astar_expand(astar, node, goal);

        // put expanded node on closed
        Node_inClosed(node) = true;
        NodeTable_insert(astar->closed_table, node);

        // draw closed node on the world
        World_drawCell(&g_world, Node_data(node), WORLD_RGB_CLOSED);

        // increment iteration counter
        Astar_iterations(astar)++;
    }

    // path not found: return failure
    return false;
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void Astar_expand(Astar* astar, Node* node, Node* goal)
{
    Astar_expandOne(astar, node, goal,  0,  1);
    Astar_expandOne(astar, node, goal,  1,  1);
    Astar_expandOne(astar, node, goal,  1,  0);
    Astar_expandOne(astar, node, goal,  1, -1);
    Astar_expandOne(astar, node, goal,  0, -1);
    Astar_expandOne(astar, node, goal, -1, -1);
    Astar_expandOne(astar, node, goal, -1,  0);
    Astar_expandOne(astar, node, goal, -1,  1);
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void Astar_expandOne(Astar* astar, Node* node, Node* goal, int x, int y)
{
    Waypoint* nodeLoc = (Waypoint*) Node_data(node);

    x = (nodeLoc->x) + x;
    y = (nodeLoc->y) + y;

    if ( x < g_world.map_width && x >= 0 && y < g_world.map_height && y > 0 && g_world.map[y][x] != '#' )
    {
        Waypoint* childLoc = malloc( sizeof(Waypoint) );
        Node* child;
        bool is_new;
        float newCost;

        childLoc->x = x;
        childLoc->y = y;

        newCost = g(node) + Waypoint_euclideanDistance(nodeLoc, childLoc);
        is_new = NodeTable_getNode(Astar_closed(astar), &child, childLoc);

        if ( ( !Node_inOpen(child) && !Node_inClosed(child) ) || newCost < g(child) )
        {
            Node_inOpen(child) = true;
            Node_inClosed(child) = false;
            Node_parent(child) = node;
            g(child) = newCost;
            h(child) = Astar_estimate(astar, child, goal);
            NodeQueue_push(Astar_open(astar), child);

            // draw pop node on the world
            World_drawCell(&g_world, Node_data(child), WORLD_RGB_OPEN);
            World_drawLink(&g_world, Node_data(child), Node_data(node), WORLD_RGB_LINK);

            if ( is_new )
            {
                Astar_allocatedNodes(astar)++;
            }
            else
            {
                Astar_revisitedNodes(astar)++;
            }
        }
    }
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void Astar_print(Astar* astar)
{
    if ( Astar_pathFound(astar) )
    {
        printf("\n\nPath found\n\n");
        printf("   Path size: %d\n", DList_size(Astar_path(astar)) );
        printf("   Number of iterations: %d\n", Astar_iterations(astar));
        printf("   Number of allocated nodes: %d\n", Astar_allocatedNodes(astar));
        printf("   Number of revisited nodes: %d\n", Astar_revisitedNodes(astar));
        printf("   Number of open nodes: %d\n", NodeQueue_size(Astar_open(astar)));
        printf("   Number of closed nodes: %d\n\n\n", NodeTable_size(Astar_closed(astar)) - NodeQueue_size(Astar_open(astar)) );
    }
    else
    {
        printf("\n\nPath not found\n\n");
        printf("   Number of iterations: %d\n", Astar_iterations(astar));
        printf("   Number of allocated nodes: %d\n", Astar_allocatedNodes(astar));
        printf("   Number of revisited nodes: %d\n", Astar_revisitedNodes(astar));
        printf("   Number of closed nodes: %d\n\n\n", NodeTable_size(Astar_closed(astar)) - NodeQueue_size(Astar_open(astar)) );
    }
}

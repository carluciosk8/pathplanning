/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "astar-mpi.h"

#include <stdio.h>
#include <math.h>

#include "world.h"
#include "primitives.h"
#include "comm-mpi.h"



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void AstarMpi_create(AstarMpi* astar_mpi)
{
    Astar_create(&astar_mpi->astar);

    astar_mpi->slave_table    = malloc( sizeof(NodeTable) );
    astar_mpi->slave_solution = malloc( sizeof(Path) );

    NodeTable_init(astar_mpi->slave_table);
    Path_init(astar_mpi->slave_solution);
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
bool AstarMpi_searchMaster(AstarMpi* astar_mpi, Node* start, Node* goal)
{
    Node*  node;
    Node*  slave_node;

    int found;


    // prepare start node
    g(start) = 0;
    h(start) = Astar_estimate(&astar_mpi->astar, start, goal);
    Node_inClosed(start) = false;
    Node_parent(start) = NULL;

    // push start node on open
    Node_inOpen(start) = true;
    NodeQueue_push(astar_mpi->astar.open_pqueue, start);

    // draw start node
    World_drawCell(&g_world, Node_data(start), WORLD_RGB_OPEN);
    World_drawCell(&g_world, Node_data(goal),  RGB_DARKEN(WORLD_RGB_CLOSED));

    // main loop: process open queue while itsn't empty
    while ( !NodeQueue_empty(astar_mpi->astar.open_pqueue) )
    {
        // pop a node from open, this is the best f(node) value
        NodeQueue_pop(astar_mpi->astar.open_pqueue, &node);
        Node_inOpen(node) = false;

        // draw pop node on the world
        World_drawCell(&g_world, Node_data(node), WORLD_RGB_POP);

        // test if node is in slave table
        slave_node = node;
        found = NodeTable_lookup(astar_mpi->slave_table, &slave_node);

        // if node is goal or slave expanded then success
        if ( Node_match(node, goal) || (found == 0) )
        {
            // send search success and common node to slave
            send_found(true, node, SLAVE);

            // receive partial path from slave
            recv_path(astar_mpi->slave_solution, SLAVE);

            // construct the path backward
            Path_create(astar_mpi->astar.solution, node);

            // draw path
            World_drawPath(&g_world, astar_mpi->astar.solution, WORLD_RGB_PATH);
            World_drawPath(&g_world, astar_mpi->slave_solution, RGB_DARKEN(WORLD_RGB_PATH));

            // return success
            Astar_pathFound(&astar_mpi->astar) = true;
            return true;
        }
        else
        {
            // send search not found
            send_found(false, node, SLAVE);
        }

        // expand node and put all they children in open
        AstarMpi_expandMaster(astar_mpi, node, goal);

        // put expanded node on closed
        Node_inClosed(node) = true;
        NodeTable_insert(astar_mpi->astar.closed_table, node);

        // draw closed node on the world
        World_drawCell(&g_world, Node_data(node), WORLD_RGB_CLOSED);

        // increment iteration counter
        Astar_iterations(&astar_mpi->astar)++;
    }

    // path not found: return failure
    return false;
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
bool AstarMpi_searchSlave(AstarMpi* astar_mpi, Node* start, Node* goal)
{
    Node* node;
    Node* commonNode;

    bool masterFound;

    // prepare start node
    g(start) = 0;
    h(start) = Astar_estimate(&astar_mpi->astar, start, goal);
    Node_inClosed(start) = false;
    Node_parent(start) = NULL;

    // push start node on open
    Node_inOpen(start) = true;
    NodeQueue_push(astar_mpi->astar.open_pqueue, start);

    // main loop: process open queue while itsn't empty
    while ( !NodeQueue_empty(astar_mpi->astar.open_pqueue) )
    {
        // pop a node from open, this is the best f(node) value
        NodeQueue_pop(astar_mpi->astar.open_pqueue, &node);
        Node_inOpen(node) = false;

        // receive information from master about search
        recv_found(&masterFound, &commonNode, MASTER);

        // if node is goal then success
        if ( Node_match(node, goal) || masterFound )
        {
            // Construct the path backward
            NodeTable_lookup(astar_mpi->astar.closed_table, &commonNode);
            Path_create(astar_mpi->astar.solution, commonNode);

            // send partial path to master
            send_path(astar_mpi->astar.solution, MASTER);

            // return success
            Astar_pathFound(&astar_mpi->astar) = true;
            return true;
        }

        // expand node and put all they children in open
        AstarMpi_expandSlave(astar_mpi, node, goal);

        // put expanded node on closed
        Node_inClosed(node) = true;
        NodeTable_insert(astar_mpi->astar.closed_table, node);

        // increment iteration counter
        Astar_iterations(&astar_mpi->astar)++;
    }

    // path not found: return failure
    return false;
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
bool AstarMpi_search(AstarMpi* astar_mpi, Node* start, Node* goal)
{
    switch (astar_mpi->id_proc)
    {
        case MASTER: return AstarMpi_searchMaster(astar_mpi, start, goal ); break;
        case SLAVE:  return AstarMpi_searchSlave (astar_mpi, goal,  start); break;
        default: return false;
    }
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void AstarMpi_expandMaster(AstarMpi* astar_mpi, Node* node, Node* goal)
{
    Node* child;
    Node* slave;
    int x[] = {  0, 1, 1,  1,  0, -1, -1, -1 };
    int y[] = {  1, 1, 0, -1, -1, -1,  0,  1 };
    int i;
    for (i = 0; i < 8; ++i)
    {
        child = AstarMpi_expandOne(astar_mpi, node, goal, x[i], y[i]);
        recv_node(&slave, SLAVE);
        NodeTable_insert(AstarMpi_slaveTable(astar_mpi), slave);

        if (  Node_inOpen(slave) && !Node_inClosed(slave)) World_drawCell(&g_world, Node_data(slave), RGB_DARKEN(WORLD_RGB_OPEN));
        else
        if ( !Node_inOpen(slave) &&  Node_inClosed(slave)) World_drawCell(&g_world, Node_data(slave), RGB_DARKEN(WORLD_RGB_CLOSED));
        else
        if (  Node_inOpen(slave) &&  Node_inClosed(slave)) World_drawCell(&g_world, Node_data(slave), RGB_DARKEN(WORLD_RGB_POP));
        else
        if ( !Node_inOpen(slave) && !Node_inClosed(slave)) World_drawCell(&g_world, Node_data(slave), RGB_DARKEN(WORLD_RGB_CLOSED));

        if (child)
        {
            World_drawCell(&g_world, Node_data(child), WORLD_RGB_OPEN);
            World_drawLink(&g_world, Node_data(child), Node_data(node), WORLD_RGB_LINK);
        }
    }
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void AstarMpi_expandSlave(AstarMpi* astar_mpi, Node* node, Node* goal)
{
    Node* child;
    int x[] = {  0, 1, 1,  1,  0, -1, -1, -1 };
    int y[] = {  1, 1, 0, -1, -1, -1,  0,  1 };
    int i;
    for (i = 0; i < 8; ++i)
    {
        child = AstarMpi_expandOne(astar_mpi, node, goal, x[i], y[i]);
        send_node( (child ? child : node), MASTER );
    }
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
Node* AstarMpi_expandOne(AstarMpi* astar_mpi, Node* node, Node* goal, int x, int y)
{
    Waypoint* node_waypoint = (Waypoint*) Node_data(node);

    x = (node_waypoint->x) + x;
    y = (node_waypoint->y) + y;

    if ( 0 <= x && x < g_world.map_width && 0 <= y && y < g_world.map_height && g_world.map[y][x] != '#' )
    {
        Waypoint* child_waypoint = malloc( sizeof(Waypoint) );
        Node* child;
        bool is_new;
        float new_cost;

        child_waypoint->x = x;
        child_waypoint->y = y;

        new_cost = g(node) + Waypoint_euclideanDistance(node_waypoint, child_waypoint);
        is_new = NodeTable_getNode(Astar_closed(&astar_mpi->astar), &child, child_waypoint);

        if ( ( !Node_inOpen(child) && !Node_inClosed(child) ) || new_cost < g(child) )
        {
            Node_inOpen(child) = true;
            Node_inClosed(child) = false;
            Node_parent(child) = node;
            g(child) = new_cost;
            h(child) = Astar_estimate(&astar_mpi->astar, child, goal);
            NodeQueue_push(Astar_open(&astar_mpi->astar), child);

            if ( is_new )
            {
                Astar_allocatedNodes(&astar_mpi->astar)++;
            }
            else
            {
                Astar_revisitedNodes(&astar_mpi->astar)++;
            }

            return child;
        }
    }

    return NULL;
}

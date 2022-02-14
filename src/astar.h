/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __ASTAR_H_
#define __ASTAR_H_


#include "node.h"
#include "path.h"
#include "nodetable.h"
#include "nodequeue.h"


typedef struct
{
    bool        path_found;
    int         iterations;
    int         allocated_nodes;
    int         revisited_nodes;
    NodeQueue*  open_pqueue;
    NodeTable*  closed_table;
    Path*       solution;
    float (*estimate) (const Waypoint* way1, const Waypoint* way2);
}
Astar;


void Astar_create(Astar* astar);
bool Astar_search(Astar* astar, Node* start, Node* goal);
void Astar_print (Astar* astar);


#define g(node) ( (node)->cost )
#define h(node) ( (node)->estimate )
#define f(node) ( g(node) + h(node) )

#define Astar_pathFound(astar)               (astar)->path_found
#define Astar_iterations(astar)              (astar)->iterations
#define Astar_open(astar)                    (astar)->open_pqueue
#define Astar_closed(astar)                  (astar)->closed_table
#define Astar_allocatedNodes(astar)          (astar)->allocated_nodes
#define Astar_revisitedNodes(astar)          (astar)->revisited_nodes
#define Astar_path(astar)                    (astar)->solution
#define Astar_estimate(astar, node, goal)    (((astar)->estimate) ? (astar)->estimate((Waypoint*) Node_data(node), (Waypoint*) Node_data(goal)) : 0)


#endif

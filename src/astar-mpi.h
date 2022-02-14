/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __ASTAR_MPI_H_
#define __ASTAR_MPI_H_


#include "astar.h"


#define  MASTER   0
#define  SLAVE    1


typedef struct
{
    Astar        astar;
    int          id_proc;
    NodeTable*   slave_table;
    Path*        slave_solution;
}
AstarMpi;


void  AstarMpi_create       (AstarMpi* astar_mpi);
bool  AstarMpi_search       (AstarMpi* astar_mpi, Node* start, Node* goal);
bool  AstarMpi_searchMaster (AstarMpi* astar_mpi, Node* start, Node* goal);
bool  AstarMpi_searchSlave  (AstarMpi* astar_mpi, Node* start, Node* goal);
void  AstarMpi_expandMaster (AstarMpi* astar_mpi, Node* node,  Node* goal);
void  AstarMpi_expandSlave  (AstarMpi* astar_mpi, Node* node,  Node* goal);
Node* AstarMpi_expandOne    (AstarMpi* astar_mpi, Node* node,  Node* goal, int x, int y);


#define AstarMpi_slaveTable(astar_mpi)       (astar_mpi)->slave_table
#define AstarMpi_slaveSolution(astar_mpi)    (astar_mpi)->slave_solution


#endif

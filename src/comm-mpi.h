/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __COMM_MPI_H_
#define __COMM_MPI_H_


#include "node.h"
#include "path.h"


void send_node(Node*  node, int idProc);
void recv_node(Node** node, int idProc);

void send_found(bool  found, Node*  node, int idProc);
void recv_found(bool* found, Node** node, int idProc);

void send_path(Path* path, int idProc);
void recv_path(Path* path, int idProc);


#endif

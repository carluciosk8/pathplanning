/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "comm-mpi.h"

#include <mpi.h>

#include "waypoint.h"



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void send_node(Node* node, int idProc)
{
    MPI_Send(node, sizeof(Node), MPI_BYTE, idProc, 0, MPI_COMM_WORLD);
    MPI_Send(Node_data(node), sizeof(Waypoint), MPI_BYTE, idProc, 0, MPI_COMM_WORLD);
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void recv_node(Node** node, int idProc)
{
    Waypoint* loc = malloc(sizeof(Waypoint));

    *node = malloc(sizeof(Node));

    MPI_Recv(*node, sizeof(Node), MPI_BYTE, idProc, 0, MPI_COMM_WORLD, 0);
    MPI_Recv(loc, sizeof(Waypoint), MPI_BYTE, idProc, 0, MPI_COMM_WORLD, 0);

    Node_data((*node)) = loc;
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void send_found(bool found, Node* node, int idProc)
{
    MPI_Send(&found, sizeof(bool), MPI_BYTE, idProc, 0, MPI_COMM_WORLD);
    send_node(node, idProc);
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void recv_found(bool* found, Node** node, int idProc)
{
    MPI_Recv(found, sizeof(bool), MPI_BYTE, idProc, 0, MPI_COMM_WORLD, 0);
    recv_node(node, idProc);
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void send_path(Path* path, int idProc)
{
    Node* node_obj;
    DlistItem* delmt;
    bool f = false;
    bool t = true;


    delmt = DList_head(path);
    while ( delmt != NULL )
    {
        node_obj = (Node*) DList_data(delmt);
        send_node(node_obj, idProc);
        delmt = DList_next(delmt);
        if ( delmt != NULL )
        {
            MPI_Send(&f, sizeof(bool), MPI_BYTE, idProc, 0, MPI_COMM_WORLD);
        }
        else
        {
            MPI_Send(&t, sizeof(bool), MPI_BYTE, idProc, 0, MPI_COMM_WORLD);
        }
    }
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void recv_path(Path* path, int idProc)
{
    Node* node_obj;
    DlistItem* delmt;
    bool end = false;

    recv_node( &node_obj, idProc );

    DList_insertPrev(path, NULL, node_obj);
    delmt = DList_tail(path);

    MPI_Recv( &end, sizeof(bool), MPI_BYTE, idProc, 0, MPI_COMM_WORLD, 0 );
    while ( !end )
    {
        recv_node( &node_obj, idProc );

        DList_insertPrev(path, delmt, node_obj);
        delmt = DList_prev(delmt);

        MPI_Recv( &end, sizeof(bool), MPI_BYTE, idProc, 0, MPI_COMM_WORLD, 0 );
    }
}

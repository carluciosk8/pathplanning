/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#include "timer.h"
#include "world.h"
#include "astar-mpi.h"
#include "globals.h"



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
int main(int argc, char** argv)
{
    Timer    timer;
    AstarMpi astar_mpi;
    Node     start;
    Node     goal;
    int      num_procs;


    // initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &astar_mpi.id_proc);
    if ( num_procs != 2 )
    {
        printf("ERROR: number of processors must have be equal 2\n");
        exit(1);
    }

    // initialize the world
    World_init(argc, argv);
    World_load(&g_world);
    if (astar_mpi.id_proc == MASTER) World_draw(&g_world);

    switch (g_world.heuristic)
    {
        case 0:  astar_mpi.astar.estimate = Waypoint_euclideanDistance; break;
        case 1:  astar_mpi.astar.estimate = Waypoint_diagshortDistance; break;
        case 2:  astar_mpi.astar.estimate = Waypoint_manhattanDistance; break;
        default: astar_mpi.astar.estimate = NULL;
    }

    if (astar_mpi.id_proc == MASTER)
    {
        if (g_world.pause_start == -1) wait_key() else SDL_Delay(g_world.pause_start);
        Timer_start(&timer);
    }

    Node_data(&start) = &g_world.start;
    Node_data(&goal)  = &g_world.goal;
    Node_parent(&start) = NULL;
    AstarMpi_create(&astar_mpi);
    AstarMpi_search(&astar_mpi, &start, &goal);

    if (astar_mpi.id_proc == MASTER)
    {
        Timer_stop(&timer);
        Timer_print(&timer);
        if (!g_world.draw_enable)
        {
            g_world.draw_enable = true;
            World_update(&g_world);
        }
        if (g_world.pause_end == -1) wait_key() else SDL_Delay(g_world.pause_end);
    }

    MPI_Finalize();

    return 0;
}

/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include <stdlib.h>
#include <stdio.h>

#include "timer.h"
#include "world.h"
#include "astar.h"
#include "globals.h"



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
int main(int argc, char** argv)
{
    Timer timer;
    Astar astar;
    Node  start;
    Node  goal;


    World_init(argc, argv);
    World_load(&g_world);
    World_draw(&g_world);


    Node_parent(&start) = NULL;
    Node_data(&start) = &g_world.start;
    Node_data(&goal) = &g_world.goal;

    if (g_world.pause_start == -1) wait_key() else SDL_Delay(g_world.pause_start);

    Timer_start(&timer);
    Astar_create(&astar);
    Astar_search(&astar, &start, &goal);
    Timer_stop(&timer);
    Timer_print(&timer);

    if (!g_world.draw_enable)
    {
        g_world.draw_enable = true;
        World_update(&g_world);
    }

    if (g_world.pause_end == -1) wait_key() else SDL_Delay(g_world.pause_end);

    return 0;
}



void World_pathplanning(World* world)
{

}

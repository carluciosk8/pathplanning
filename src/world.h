/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __WORLD_H_
#define __WORLD_H_


#include <SDL.h>

#include "path.h"
#include "waypoint.h"

#define WORLD_RGB_WALL     0xff000000
#define WORLD_RGB_FREEWAY  0xffffffff
#define WORLD_RGB_START    0xff800000
#define WORLD_RGB_GOAL     0xff008000
#define WORLD_RGB_OPEN     0xffffff00
#define WORLD_RGB_POP      0xff00ff00
#define WORLD_RGB_CLOSED   0xffff0000
#define WORLD_RGB_LINK     0xff000000
#define WORLD_RGB_PATH     0xff0000ff


typedef struct
{
    char** map;
    int    map_width;
    int    map_height;
    int    cell_width;
    int    cell_height;
    Waypoint start;
    Waypoint goal;

    char*  filename;
    char   heuristic;
    int    pause_start;
    int    pause_end;
    Uint32 draw_delay;

    SDL_Surface* surface;
    SDL_Surface* background;
    SDL_Surface* foreground;
    bool         draw_enable;
}
World;

extern World g_world;

void World_init     (int argc, char** argv);
int  World_load     (World* world);
void World_draw     (World* world);
void World_update   (World* world);
void World_drawCell (World* world, Waypoint* waypoint, Uint32 color);
void World_drawLink (World* world, Waypoint* waypoint, Waypoint* chld, Uint32 color);
void World_drawPath (World* world, Path* path, Uint32 color);

#define World_width(world)   ( (world)->map_width )
#define World_height(world)  ( (world)->map_height )


#endif

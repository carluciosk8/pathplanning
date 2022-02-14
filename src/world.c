/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "world.h"

#include <stdlib.h>
#include <stdio.h>

#include "primitives.h"



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void World_init(int argc, char** argv)
{
    g_world.filename    = NULL;
    g_world.heuristic   = 0;
    g_world.draw_enable = false;
    g_world.pause_start = 0;
    g_world.pause_end   = -1;

    int opt;
    for ( opt = 1; opt < argc; opt++ )
    {
        // parse map file option
        if ( strcmp(argv[opt], "-m" ) == 0 && ++opt < argc )
        {
            g_world.filename = argv[opt];
        }

        // parse heuristic option
        else
        if ( strcmp(argv[opt], "-e" ) == 0 && ++opt < argc )
        {
            if ( strcmp(argv[opt], "euclidean") == 0 )
            {
                g_world.heuristic = 0;
            }
            else
            if ( strcmp(argv[opt], "diagonal") == 0 )
            {
                g_world.heuristic = 1;
            }
            else
            if ( strcmp(argv[opt], "manhattan") == 0 )
            {
                g_world.heuristic = 2;
            }
            else
            if ( strcmp(argv[opt], "dijkstra") == 0 )
            {
                g_world.heuristic = 3;
            }
        }

        // parse interactive mode option
        else
        if ( !strcmp(argv[opt], "-i" ) && opt < argc )
        {
            g_world.draw_enable = true;
        }

        // parse delay time between interactions option
        else
        if ( strcmp(argv[opt], "-d" ) == 0 && ++opt < argc )
        {
            g_world.draw_delay = atoi(argv[opt]);
        }

        // parse delay time before start option
        else
        if ( strcmp(argv[opt], "-s" ) == 0 && ++opt < argc )
        {
            g_world.pause_start = atoi(argv[opt]);
        }

        // parse delay time after finish option
        else
        if ( strcmp(argv[opt], "-f" ) == 0 && ++opt < argc )
        {
            g_world.pause_end = atoi(argv[opt]);
        }

        // parse help option
        else
        if ( !strcmp(argv[opt], "-h" ) && opt < argc )
        {
            printf("usage: %s [OPTION]...                                                          \n", argv[0]);
            printf("                                                                               \n");
            printf("-h             print this help                                                 \n");
            printf("-m FILE        map filename to be solved                                       \n");
            printf("-e ESTIMATIVE  heuristic estimative (euclidean, manhattan, diagonal, dijkstra) \n");
            printf("-i             interactive mode                                                \n");
            printf("-d #           delay time between interactions (interactive mode)              \n");
            printf("-s #           delay time before start, -1 to wait key press (interactive mode)\n");
            printf("-f #           delay time after finish, -1 to wait key press (interactive mode)\n");
            exit(1);
        }
    }
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
int World_load(World* world)
{
    FILE* file;
    int x;
    int y;

    world->start.x = 0;
    world->start.y = 1;
    world->goal.x = 0;
    world->goal.y = 0;

    if ( world->filename )
    {
        file = fopen( world->filename, "r" );
    }
    else
    {
        file = stdin;
    }

    fscanf( file, "%d %d", &(world->map_width), &(world->map_height) );
    fscanf( file, "%d %d", &(world->cell_width), &(world->cell_height) );

    world->start.x = 0;
    world->start.y = 1;
    world->goal.x = world->map_width - 1;
    world->goal.y = world->map_height - 2;

    if ( !(world->map = malloc(sizeof(char*)*world->map_height)) )
    {
        return 0;
    }

    for ( y = 0; y < world->map_height; y++ )
    {
        if (!(world->map[y] = malloc(sizeof(char)*world->map_width)))
        {
            return 0;
        }

        for (x = 0; x < world->map_width; x++)
        {
            do
            {
                fscanf( file, "%c", &(world->map[y][x]) );
                if (world->map[y][x] == 'S')
                {
                    world->start.x = x;
                    world->start.y = y;
                }
                else
                if (world->map[y][x] == 'G')
                {
                    world->goal.x = x;
                    world->goal.y = y;
                }
            }
            while((world->map[y][x] == '\n') || (world->map[y][x] == '\0'));
        }
    }

    world->map[world->start.y][world->start.x] = 'S';
    world->map[world->goal.y][world->goal.x] = 'G';

    return 1;
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void World_draw(World* world)
{
    int i;
    int j;
    int width = (world->map_width)*(world->cell_width);
    int height = (world->map_height)*(world->cell_height);

    // initialize SDL
    if ( SDL_Init( SDL_INIT_EVERYTHING ) )
    {
        fprintf( stderr, "SDL_ERROR: Coudn't initialize SDL.\n" );
        exit(1);
    }

    world->surface = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE);
    if ( world->surface == NULL )
    {
        fprintf( stderr, "SDL_ERROR: Coudn't set up video.\n" );
        exit(1);
    }

    world->background = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, world->surface->format->BitsPerPixel, world->surface->format->Rmask,world->surface->format->Gmask,world->surface->format->Bmask,world->surface->format->Amask);
    if ( world->background == NULL )
    {
        fprintf( stderr, "SDL_ERROR: Coudn't allocate surface.\n" );
        exit(1);
    }

    world->foreground = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, world->surface->format->BitsPerPixel, world->surface->format->Rmask,world->surface->format->Gmask,world->surface->format->Bmask,world->surface->format->Amask);
    if ( world->foreground == NULL )
    {
        fprintf( stderr, "SDL_ERROR: Coudn't allocate surface.\n" );
        exit(1);
    }

    SDL_SetColorKey(world->foreground, SDL_SRCCOLORKEY, WORLD_RGB_FREEWAY);
    SDL_FillRect(world->background, NULL, WORLD_RGB_FREEWAY);
    SDL_FillRect(world->foreground, NULL, WORLD_RGB_FREEWAY);

    // draw world
    for ( i = 0; i < world->map_height; i++ )
    {
        for ( j = 0; j < world->map_width; j++)
        {
            int x1 = j*(world->cell_width);
            int y1 = i*(world->cell_height);
            int x2 = x1+world->cell_width;
            int y2 = y1+world->cell_height;
            int xc = x1 + (world->cell_width/2);
            int yc = y1 + (world->cell_height/2);
            int rx = world->cell_width/4;
            int ry = world->cell_height/4;

            box(world->foreground, x1, y1, x2, y2, WORLD_RGB_WALL);

            if ( world->map[i][j] == '#' )                             fillBox(world->foreground, x1, y1, x2, y2, WORLD_RGB_WALL);
            else
            if ( world->map[i][j] == 'S' || world->map[i][j] == 's'  ) fillEllipse(world->foreground, xc, yc, rx, ry, WORLD_RGB_START);
            else
            if ( world->map[i][j] == 'G' || world->map[i][j] == 'g'  ) fillEllipse(world->foreground, xc, yc, rx, ry, WORLD_RGB_GOAL);
        }
    }

    World_update(world);
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void World_update(World* world)
{
    if (world->draw_enable)
    {
        SDL_Event event;

        SDL_BlitSurface(world->background, NULL, world->surface, NULL);
        SDL_BlitSurface(world->foreground, NULL, world->surface, NULL);
        SDL_Flip(world->surface);

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    exit(1);

                case SDLK_UP:
                    (&g_world)->draw_delay += 100;
                    break;

                case SDLK_DOWN:
                    if ((&g_world)->draw_delay > 0)
                    {
                        (&g_world)->draw_delay -= 100;
                    }
                    break;

                case SDLK_PRINT:
                    while ( (event.type) != SDL_KEYUP)
                    {
                        SDL_PollEvent(&event);
                    }
                    SDL_SaveBMP((&g_world)->surface, "map.bmp");
                    break;

                default:
                    break;
                }
            }
        }
    }

    SDL_Delay(world->draw_delay);
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void World_drawCell(World* world, Waypoint* waypoint, Uint32 color)
{
    SDL_Rect cell;
    cell.x = (waypoint->x)*(world->cell_width);
    cell.y = (waypoint->y)*(world->cell_height);
    cell.w = world->cell_width;
    cell.h = world->cell_height;

    SDL_FillRect(world->background, &cell, color);

    World_update(world);
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void World_drawLink(World* world, Waypoint* waypoint, Waypoint* chld, Uint32 color)
{
    int x;
    int y;
    int x1;
    int y1;
    int x2;
    int y2;
    int dx;
    int dy;
    int c;

    x2 = (chld->x)*(world->cell_width) + (world->cell_width)/2;
    y2 = (chld->y)*(world->cell_height) + (world->cell_height)/2;
    x1 = (waypoint->x)*(world->cell_width) + (world->cell_width)/2;
    y1 = (waypoint->y)*(world->cell_height) + (world->cell_height)/2;

    if (x1 == x2 && y1 != y2)
    {
        dy = y1 - y2;
        dy /= abs(dy);
        y1 -= dy*(world->cell_height)/8;
        y2 += dy*(world->cell_height)/8;
        for (c = 0; c <= (world->cell_height)/4; c++)
        {
            line(world->foreground, x2-c, y2+c*dy, x2+c, y2+c*dy, color);
        }
        for (c = -(world->cell_width)/8+1; c < (world->cell_width)/8; c++)
        {
            line(world->foreground, x1+c, y1-dy*(world->cell_height)/8, x2+c, y2+dy*(world->cell_height)/8, color);
        }
    }
    else
    if (x1 != x2 && y1 == y2)
    {
        dx = x1 - x2;
        dx /= abs(dx);
        x1 -= dx*((world->cell_width)/8);
        x2 += dx*((world->cell_width)/8);
        for (c = 0; c <= (world->cell_width)/4; c++)
        {
            line(world->foreground, x2+c*dx, y2-c, x2+c*dx, y2+c, color);
        }
        for (c = -(world->cell_height)/8+1; c < (world->cell_height)/8; c++)
        {
            line(world->foreground, x1-dx*(world->cell_width)/8, y1+c, x2+dx*(world->cell_width)/8, y2+c, color);
        }
    }
    else
    if (x1 != x2 && y1 != y2)
    {
        dx = x2 - x1;
        dx /= abs(dx);
        dy = y2 - y1;
        dy /= abs(dy);
        x1 += dx*(world->cell_width)/8 + 4*dx;
        y1 += dy*(world->cell_height)/8 + 4*dy;
        x2 -= dx*(world->cell_width)/8 + dx;
        y2 -= dy*(world->cell_height)/8 + dy;

        for (x = x2 - dx*(world->cell_width)/4 - dx; x != x2; x += dx)
        {
            line(world->foreground, x2, y2 - dy*(world->cell_height)/4 - dy, x, y2, color);
        }
        for (y = y2 - dy*(world->cell_height)/4 -dy; y != y2; y += dy)
        {
            line(world->foreground, x2 - dx*(world->cell_width)/4 - dx, y2, x2, y, color);
        }

        for (c = 0; c <= (world->cell_width)/8; c++)
        {
            line(world->foreground, x1 + c*dx, y1, x2, y2 - c*dy, color);
            line(world->foreground, x1, y1 + c*dy, x2 - c*dx, y2, color);
        }
    }

    World_update(world);
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void World_drawPath(World* world, Path* path, Uint32 color)
{
    DlistItem* item;
    Node* node1;
    Node* node2;

    item = DList_tail(path);
    node1 = (Node*) DList_data(item);
    item = DList_prev(item);

    while ( item != NULL )
    {
        node2 = (Node*) DList_data(item);
        World_drawLink(world, Node_data(node1), Node_data(node2), color);
        node1 = node2;
        item = DList_prev(item);
    }
}

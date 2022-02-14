/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "waypoint.h"

#include <stdlib.h>
#include <math.h>



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
float Waypoint_euclideanDistance(const Waypoint* way1, const Waypoint* way2)
{
    float dx = way2->x - way1->x;
    float dy = way2->y - way1->y;

    return sqrtf(dx*dx + dy*dy);
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
float Waypoint_manhattanDistance(const Waypoint* way1, const Waypoint* way2)
{
    float dx = abs(way2->x - way1->x);
    float dy = abs(way2->y - way1->y);

    return dx + dy;
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
float Waypoint_diagshortDistance(const Waypoint* way1, const Waypoint* way2)
{
    float dx = abs(way2->x - way1->x);
    float dy = abs(way2->y - way1->y);

    return fmax(dx, dy) + 0.41421356f*fmin(dx, dy);
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
bool Waypoint_match(const Waypoint* way1, const Waypoint* way2)
{
    return ( (way1->x == way2->x) && (way1->y == way2->y) );
}

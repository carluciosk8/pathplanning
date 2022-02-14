/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __WAYPOINT_H_
#define __WAYPOINT_H_


#include "bool.h"


typedef struct
{
    int x;
    int y;
}
Waypoint;


float  Waypoint_euclideanDistance(const Waypoint* way1, const Waypoint* way2);
float  Waypoint_manhattanDistance(const Waypoint* way1, const Waypoint* way2);
float  Waypoint_diagshortDistance(const Waypoint* way1, const Waypoint* way2);
bool   Waypoint_match            (const Waypoint* way1, const Waypoint* way2);


#endif

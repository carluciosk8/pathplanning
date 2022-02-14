/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __GENMAZE_H_
#define __GENMAZE_H_

#define GO_LEFT    1
#define GO_RIGHT   2
#define GO_UP      4
#define GO_DOWN    8
#define USED       16

#define M(x,y) maze_data[(x)+1+((y)+1)*(c+2)]


void genmaze(int c, int r, int w, int h, int s);


#endif

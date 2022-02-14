/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef _PRIMITIVES_H_
#define _PRIMITIVES_H_

#include <SDL.h>

#define round(a) ((int)(a+0.5))


#define RGB_DARKEN(color) (((color) & 0xff555555))
#define RGB_BRIGHTEN(color) (((~color) & 0xffaaaaaa) | color)


Uint32 getPixel      (SDL_Surface* surface, int x,  int y);
void   putPixel      (SDL_Surface* surface, int x,  int y,Uint32 color);
void   line          (SDL_Surface* surface, int xa, int ya, int xb, int yb, Uint32 color);
void   box           (SDL_Surface* surface, int xa, int ya, int xb, int yb, Uint32 color);
void   fillBox       (SDL_Surface* surface, int xa, int ya, int xb, int yb, Uint32 color);
void   circle        (SDL_Surface* surface, int x,  int y,  int radius, Uint32 color);
void   fillCircle    (SDL_Surface* surface, int x,  int y,  int radius, Uint32 color);
void   ellipse       (SDL_Surface* surface, int x,  int y,  int rx, int ry, Uint32 color);
void   fillEllipse   (SDL_Surface* surface, int x,  int y,  int rx, int ry, Uint32 color);


#endif

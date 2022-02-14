/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "primitives.h"



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void putPixel(SDL_Surface* surface, int x, int y, Uint32 color)
{
    if ( x < surface->w && x >= 0 && y < surface->h && y >= 0 )
    {
        int bpp = surface->format->BytesPerPixel;
        Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

        switch ( bpp )
        {
        case 1:
            *p = color;
            break;

        case 2:
            *(Uint16*)p = color;
            break;

        case 3:
            if ( SDL_BYTEORDER == SDL_BIG_ENDIAN )
            {
                p[0] = (color >> 16) & 0xff;
                p[1] = (color >> 8) & 0xff;
                p[2] = color & 0xff;
            }
            else
            {
                p[0] = color & 0xff;
                p[1] = (color >> 8) & 0xff;
                p[2] = (color >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32*)p = color;
            break;
        }
    }
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void circlePutPixel(SDL_Surface* surface, int cx, int cy, int x, int y, Uint32 color)
{
    putPixel( surface, cx+x, cy+y, color );
    putPixel( surface, cx+y, cy+x, color );
    putPixel( surface, cx+y, cy-x, color );
    putPixel( surface, cx+x, cy-y, color );
    putPixel( surface, cx-x, cy-y, color );
    putPixel( surface, cx-y, cy-x, color );
    putPixel( surface, cx-y, cy+x, color );
    putPixel( surface, cx-x, cy+y, color );
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void ellipsePutPixel(SDL_Surface* surface, int cx, int cy, int x, int y, Uint32 color)
{
    putPixel( surface, cx+x, cy+y, color );
    putPixel( surface, cx+x, cy-y, color );
    putPixel( surface, cx-x, cy-y, color );
    putPixel( surface, cx-x, cy+y, color );
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void fillellipsePutPixel(SDL_Surface* surface, int cx, int cy, int x, int y,Uint32 color)
{
    int i;

    for ( i = 0; i <= y; i++ )
    {
        putPixel( surface, cx+x, cy+i, color );
        putPixel( surface, cx+x, cy-i, color );
        putPixel( surface, cx-x, cy-i, color );
        putPixel( surface, cx-x, cy+i, color );
    }
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
Uint32 getPixel(SDL_Surface* surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp)
    {
    case 1:
        return *p;
    case 2:
        return *(Uint16*)p;
    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
        {
            return p[0] << 16 | p[1] << 8 | p[2];
        }
        else
        {
            return p[0] | p[1] << 8 | p[2] << 16;
        }
    case 4:
        return *(Uint32*)p;
    default:
        return 0;
    }
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void line(SDL_Surface* surface, int xa, int ya, int xb, int yb, Uint32 color)
{
    int x;
    int dx;
    int xinc1;
    int xinc2;
    int y;
    int dy;
    int yinc1;
    int yinc2;
    int d;
    int dinc1;
    int dinc2;
    int i;
    int numpixels;


    if ( xb < xa )
    {
        dx = xa - xb;
    }
    else
    {
        dx = xb - xa;
    }
    if ( yb < ya )
    {
        dy = ya - yb;
    }
    else
    {
        dy = yb - ya;
    }

    if ( dx >= dy )
    {
        numpixels = dx + 1;
        d = 2*dy - dx;
        dinc1 = 2*dy;
        dinc2 = 2*(dy - dx);
        xinc1 = 1;
        xinc2 = 1;
        yinc1 = 0;
        yinc2 = 1;
    }
    else
    {
        numpixels = dy + 1;
        d = 2*dx - dy;
        dinc1 = 2*dx;
        dinc2 = 2*(dx - dy);
        xinc1 = 0;
        xinc2 = 1;
        yinc1 = 1;
        yinc2 = 1;
    }
    if ( xa > xb )
    {
        xinc1 = -xinc1;
        xinc2 = -xinc2;
    }
    if ( ya > yb )
    {
        yinc1 = -yinc1;
        yinc2 = -yinc2;
    }
    x = xa;
    y = ya;


    for ( i = 0; i < numpixels; i++ )
    {
        putPixel( surface, x,y, color);
        if ( d < 0 )
        {
            d += dinc1;
            x += xinc1;
            y += yinc1;
        }
        else
        {
            d += dinc2;
            x += xinc2;
            y += yinc2;
        }
    }
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void box(SDL_Surface* surface, int xa, int ya, int xb, int yb, Uint32 color)
{
    int x;
    int y;
    int t;

    if ( xb < xa )
    {
        t = xa;
        xa = xb;
        xb = t;
    }

    if ( yb < ya )
    {
        t = ya;
        ya = yb;
        yb = t;
    }

    for ( x = xa; x <= xb; x++ )
    {
        putPixel( surface, x,ya, color);
        putPixel( surface, x,yb, color);
    }
    for ( y = ya; y <= yb; y++ )
    {
        putPixel( surface, xa,y, color);
        putPixel( surface, xb,y, color);
    }
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void fillBox(SDL_Surface* surface, int xa, int ya, int xb, int yb, Uint32 color)
{
    int x;
    int y;
    int t;

    if ( xb < xa )
    {
        t = xa;
        xa = xb;
        xb = t;
    }

    if ( yb < ya )
    {
        t = ya;
        ya = yb;
        yb = t;
    }

    for ( x = xa; x <= xb; x++ )
    {
        for ( y = ya; y <= yb; y++ )
        {
            putPixel(surface, x, y, color);
        }
    }
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void circle(SDL_Surface* surface, int cx, int cy, int radius, Uint32 color)
{
    int x = 0;
    int y = radius;
    int d = 1 - radius;
    int dE = 3;
    int dSE = -2*radius + 5;

    circlePutPixel( surface, cx, cy, x, y, color );
    while ( y > x )
    {
        if ( d < 0 )
        {
            d += dE;
            dE += 2;
            dSE += 2;
        }
        else
        {
            d += dSE;
            dE += 2;
            dSE += 4;
            y--;
        }
        x++;
        SDL_Flip( surface );
        circlePutPixel( surface, cx,cy, x,y, color );
    }
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void fillCircle(SDL_Surface* surface, int cx, int cy, int radius, Uint32 color)
{
    int r2 = radius*radius;
    int twor2 = 2*r2;
    int p;
    int x = 0;
    int y = radius;
    int px = 0;
    int py = twor2*y;

    fillellipsePutPixel( surface, cx,cy, x,y, color );
    p = round( r2 - (r2*radius) + (0.25*r2) );
    while ( px < py )
    {
        x++;
        px += twor2;
        if ( p < 0 )
        {
            p += r2 + px;
        }
        else
        {
            y--;
            py -= twor2;
            p += r2 + px - py;
        }
        fillellipsePutPixel( surface, cx,cy, x,y, color );
    }

    p = round( r2*(x + 0.5)*(x + 0.5) + r2*(y - 1)*(y - 1) - r2*r2 );
    while ( y > 0 )
    {
        y--;
        py -= twor2;
        if ( p > 0 )
        {
            p += r2 - py;
        }
        else
        {
            x++;
            px += twor2;
            p += r2 - py + px;
        }
        fillellipsePutPixel( surface, cx,cy, x,y, color );
    }
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void ellipse(SDL_Surface* surface, int cx, int cy, int rx, int ry, Uint32 color)
{
    int rx2 = rx*rx;
    int ry2 = ry*ry;
    int tworx2 = 2*rx2;
    int twory2 = 2*ry2;
    int p;
    int x = 0;
    int y = ry;
    int px = 0;
    int py = tworx2*y;

    ellipsePutPixel( surface, cx,cy, x,y, color );
    p = round( ry2 - (rx2*ry) + (0.25*rx2) );
    while ( px < py )
    {
        x++;
        px += twory2;
        if ( p < 0 )
        {
            p += ry2 + px;
        }
        else
        {
            y--;
            py -= tworx2;
            p += ry2 + px - py;
        }
        ellipsePutPixel( surface, cx,cy, x,y, color );
    }

    p = round( (float)ry2*((float)x+0.5)*((float)x+0.5) + (float)rx2*(y-1)*(y-1) - (float)rx2*ry2 );
    while ( y > 0 )
    {
        y--;
        py -= tworx2;
        if ( p > 0 )
        {
            p += rx2 - py;
        }
        else
        {
            x++;
            px += twory2;
            p += rx2 - py + px;
        }
        ellipsePutPixel( surface, cx,cy, x,y, color );
    }
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void fillEllipse(SDL_Surface* surface, int cx, int cy, int rx, int ry, Uint32 color)
{
    int rx2 = rx*rx;
    int ry2 = ry*ry;
    int tworx2 = 2*rx2;
    int twory2 = 2*ry2;
    int p;
    int x = 0;
    int y = ry;
    int px = 0;
    int py = tworx2*y;

    fillellipsePutPixel( surface, cx,cy, x,y, color );
    p = round( ry2 - (rx2*ry) + (0.25*rx2) );
    while ( px < py )
    {
        x++;
        px += twory2;
        if ( p < 0 )
        {
            p += ry2 + px;
        }
        else
        {
            y--;
            py -= tworx2;
            p += ry2 + px - py;
        }
        fillellipsePutPixel( surface, cx,cy, x,y, color );
    }

    p = round( ry2*(x + 0.5)*(x + 0.5) + rx2*(y - 1)*(y - 1) - rx2*ry2 );
    while ( y > 0 )
    {
        y--;
        py -= tworx2;
        if ( p > 0 )
        {
            p += rx2 - py;
        }
        else
        {
            x++;
            px += twory2;
            p += rx2 - py + px;
        }
        fillellipsePutPixel( surface, cx,cy, x,y, color );
    }
}

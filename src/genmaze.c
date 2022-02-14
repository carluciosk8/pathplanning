/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "genmaze.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
int main(int argc, char** argv)
{
    int c = 10, r = 10, w = 3, h = 2, s = 0, d = 8;

    if (argc > 1) c = atoi(argv[1]);
    if (argc > 2) r = atoi(argv[2]);
    if (argc > 3) w = atoi(argv[3]);
    if (argc > 4) h = atoi(argv[4]);
    if (argc > 5) d = atoi(argv[5]);
    if (argc > 6) s = atoi(argv[6]);

    if (c < 1 || r < 1 || w < 2 || h < 2) return 1;

    if ( s == 0 ) s = (int)time(0);
    if ( d < 8 ) d = 8;

    printf("%d %d\n", c*w+1, r*h+1);
    printf("%d %d\n", d, d);
    genmaze(c, r, w, h, s);

    return 0;
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void genmaze(int c, int r, int w, int h, int s)
{
    char* maze_data;
    int x, y;
    int* xt, * yt;
    int num_left;
    int i;

    if (c < 1 || r < 1 || w < 1 || h < 1) return;

    maze_data = (char*)calloc(r+2, c+2);
    xt = (int*)malloc(c*r*sizeof(int));
    yt = (int*)malloc(c*r*sizeof(int));

    for (x = 0; x < c; x++)
    {
        for (y = 0; y < r; y++)
        {
            xt[x+y*c] = x;
            yt[x+y*c] = y;
        }
    }

    for (x = -1; x <= c; x++)
    {
        M(x, -1) = USED;
        M(x, r) = USED;
    }
    for (y = 0; y <= r; y++)
    {
        M(-1, y) = USED;
        M(c, y) = USED;
    }

    if ( s == 0 ) s = time(0);
    srandom(s);

    for (x = r*c-1; x > 0; x--)
    {
        y = random() % (x+1);
        if (y != x)
        {
            int t = xt[x];
            xt[x] = xt[y];
            xt[y] = t;
            t = yt[x];
            yt[x] = yt[y];
            yt[y] = t;
        }
    }

    M(-1,0) = M(-1,0) | (USED | GO_RIGHT);
    M(0,0) = M(0,0) | (USED | GO_LEFT);

    num_left = r*c-1;
    while (num_left)
    {
        for (x = 0; x < c; x++)
        {
            for (y = 0; y < r; y++)
            {
                while (1)
                {
                    int px = xt[x+y*c];
                    int py = yt[x+y*c];
                    int ways[8], wp;
                    
                    if (!(M(px, py) & USED)) break;

                    while (1)
                    {
                        wp = 0;

                        if (!(M(px,py-1) & USED)) ways[wp++] = GO_UP;
                        if (!(M(px,py-1) & USED)) ways[wp++] = GO_UP;
                        if (!(M(px,py+1) & USED)) ways[wp++] = GO_DOWN;
                        if (!(M(px-1,py) & USED)) ways[wp++] = GO_LEFT;
                        if (!(M(px-1,py) & USED)) ways[wp++] = GO_LEFT;
                        if (!(M(px+1,py) & USED)) ways[wp++] = GO_RIGHT;

                        if (wp == 0) break; // blocked at this point

                        wp = random() % wp;

                        switch (ways[wp])
                        {
                        case GO_LEFT:
                            M(px,py) |= GO_LEFT;
                            px--;
                            M(px,py) |= GO_RIGHT | USED;
                            break;
                        case GO_RIGHT:
                            M(px,py) |= GO_RIGHT;
                            px++;
                            M(px,py) |= GO_LEFT | USED;
                            break;
                        case GO_UP:
                            M(px,py) |= GO_UP;
                            py--;
                            M(px,py) |= GO_DOWN | USED;
                            break;
                        case GO_DOWN:
                            M(px,py) |= GO_DOWN;
                            py++;
                            M(px,py) |= GO_UP | USED;
                            break;
                        }
                        num_left--;
                    }
                    
                    if (px == xt[x+y*c] && py == yt[x+y*c]) break;
                }
            }
        }
    }

    M(c,r-1) |= USED | GO_LEFT;
    M(c-1,r-1) |= USED | GO_RIGHT;

    for (y = 0; y <= r; y++)
    {
        int dy;
        putchar('#');
        for (x = 0; x < c; x++)
        {
            for (i = 1; i < w; i++) putchar( (M(x,y) & GO_UP || x == -1) ? ' ' : '#' );
            printf("#");
        }
        printf("\n");
        if ( y < r )
        {
            for (dy = 1; dy < h; dy++)
            {
                putchar( (M(0,y) & GO_LEFT) ? ' ' : '#' );
                for (x = 0; x < c; x++)
                {
                    for (i = 1; i < w; i++) putchar(' ');
                    putchar( (M(x,y) & GO_RIGHT) ? ' ' : '#' );
                }
                printf("\n");
            }
        }
    }
}

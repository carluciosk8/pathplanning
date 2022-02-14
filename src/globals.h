/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __GLOBALS_H_
#define __GLOBALS_H_

#include "world.h"


World  g_world;

#define wait_key() { SDL_Event __evt; for(;;) if ( SDL_WaitEvent(&__evt) && __evt.type == SDL_KEYDOWN ) break; }


#endif

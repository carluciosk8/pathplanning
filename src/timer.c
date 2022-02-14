/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "timer.h"

#include <stdio.h>



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void Timer_start(Timer* timer)
{
    gettimeofday( &(timer->tval), &(timer->tzone) );
    getrusage( RUSAGE_SELF, &(timer->rusage_start) );
    timer->startsec = timer->tval.tv_sec;
    timer->startusec = timer->tval.tv_usec;
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void Timer_stop(Timer* timer)
{
    gettimeofday( &(timer->tval), &(timer->tzone) );
    timer->endsec = timer->tval.tv_sec;
    timer->endusec = timer->tval.tv_usec;
    getrusage( RUSAGE_SELF, &(timer->rusage_end) );

    timer->sec = timer->endsec - timer->startsec;
    timer->usec = timer->endusec - timer->startusec;
    if ( timer->usec < 0 )
    {
        timer->sec--;
        timer->usec = 1000000 + timer->usec;
    }
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void Timer_print(Timer* timer)
{
    printf("Time Statistics:\n\n");
    printf("   Execution time: %f\n", Timer_execTime(*timer));
    printf("   User time: %f\n", Timer_userTime(*timer));
    printf("   System time: %f\n", Timer_sysTime(*timer));
}

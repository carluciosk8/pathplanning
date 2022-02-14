/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __TIMER_H_
#define __TIMER_H_


#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>



typedef struct
{
    long startsec;
    long startusec;
    long endsec;
    long endusec;
    long sec;
    long usec;

    struct timeval tval;
    struct timezone tzone;
    struct rusage rusage_start;
    struct rusage rusage_end;
} Timer;


#define Timer_execTimeSec(c)  ( (c).sec )
#define Timer_execTimeMsec(c) ( (c).usec )

#define Timer_execTime(c)     (float)( Timer_execTimeSec(c) + (float)(Timer_execTimeMsec(c) / 1000000.0) )
#define Timer_userTime(c)     (float)( Timer_userTimeSec(c) + (float)(Timer_userTimeMsec(c) / 1000000.0) )
#define Timer_sysTime(c)      (float)( Timer_sysTimeSec(c)  + (float)(Timer_sysTimeMsec(c)  / 1000000.0) )

#define Timer_userTimeSec(c)  ( (c).rusage_end.ru_utime.tv_sec  - (c).rusage_start.ru_utime.tv_sec )
#define Timer_userTimeMsec(c) ( (c).rusage_end.ru_utime.tv_usec - (c).rusage_start.ru_utime.tv_usec )
#define Timer_sysTimeSec(c)   ( (c).rusage_end.ru_stime.tv_sec  - (c).rusage_start.ru_stime.tv_sec )
#define Timer_sysTimeMsec(c)  ( (c).rusage_end.ru_stime.tv_usec - (c).rusage_start.ru_stime.tv_usec )

void Timer_start (Timer* timer);
void Timer_stop  (Timer* timer);
void Timer_print (Timer* timer);


#endif

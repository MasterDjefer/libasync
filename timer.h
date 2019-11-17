#ifndef TIMER_H
#define TIMER_H

#include "async.h"


typedef void(*timeout_callback_t)(void);
typedef unsigned int uint;
typedef thread_t timers_t;

struct timeout_data_t
{
    uint timeout;
    timeout_callback_t callback;
};


#ifdef _WIN32
    #define sleep(miliseconds) Sleep(miliseconds);    
#elif __linux__
    #define micro_in_mili 1000
    #define sleep(miliseconds) usleep((miliseconds) * micro_in_mili);
#endif

extern mutex_t mutex_callback_call;

timers_t set_timeout(timeout_callback_t callback, uint timeout);
timers_t set_interval(timeout_callback_t callback, uint timeout);
void clear_timeout(timers_t timer);


#endif // TIMER_H

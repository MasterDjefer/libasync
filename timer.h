#ifndef TIMER_H
#define TIMER_H

#include "async.h"


typedef void(*timeout_callback_t)(void);

struct timeout_data_t
{
    uint timeout;
    timeout_callback_t callback;
};

#define unsigned int uint;

#ifdef _WIN32
    #define sleep(miliseconds) Sleep(miliseconds);
#elif __linux__
    #define micro_in_mili 1000
    #define sleep(miliseconds) usleep((miliseconds) * micro_in_mili);
#endif

extern mutex_t mutex_callback_call;

void set_timeout(timeout_callback_t callback, uint timeout);
void set_interval(timeout_callback_t callback, uint timeout);


#endif // TIMER_H

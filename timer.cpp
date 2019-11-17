#include "timer.h"


declare_thread_func(timeout_interval_func, params)
{
    uint timeout = ((timeout_data_t*)params)->timeout;
    timeout_callback_t callback = ((timeout_data_t*)params)->callback;

    free(params);

    while (true)
    {
        sleep(timeout);

        mutex_lock(&mutex_callback_call);
        callback();
        mutex_unlock(&mutex_callback_call);
    }

    main_counter_dec();
    return 0;
}

declare_thread_func(timeout_func, params)
{
    uint timeout = ((timeout_data_t*)params)->timeout;
    timeout_callback_t callback = ((timeout_data_t*)params)->callback;

    free(params);

    sleep(timeout);


    mutex_lock(&mutex_callback_call);
    callback();
    mutex_unlock(&mutex_callback_call);


    main_counter_dec();
    return 0;
}

timers_t set_timeout(timeout_callback_t callback, uint timeout)
{
    main_counter_inc();

    timeout_data_t *timeout_data = (timeout_data_t*)malloc(sizeof(timeout_data_t));
    memset(timeout_data, 0, sizeof(timeout_data_t));
    timeout_data->timeout = timeout;
    timeout_data->callback = callback;

    thread_t thread;
    thread_create(&thread, timeout_func, timeout_data);

    add_handler(thread);

    return thread;
}

timers_t set_interval(timeout_callback_t callback, uint timeout)
{
    main_counter_inc();

    timeout_data_t *timeout_data = (timeout_data_t*)malloc(sizeof(timeout_data_t));
    memset(timeout_data, 0, sizeof(timeout_data_t));
    timeout_data->timeout = timeout;
    timeout_data->callback = callback;

    thread_t thread;
    thread_create(&thread, timeout_interval_func, timeout_data);

    add_handler(thread);

    return thread;
}

void clear_timeout(timers_t timer)
{
    thread_destroy(&timer);
    main_counter_dec();
    mutex_unlock(&mutex_callback_call);
}

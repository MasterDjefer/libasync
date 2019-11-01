#include "async.h"


static mutex_t mutex_add_handle;

static thread_t handles[100];
static int handles_count;

static int main_counter;


void main_counter_inc()
{
    main_counter++;
}
void main_counter_dec()
{
    main_counter--;
}

void async_clean()
{
    for (int i = 0; i < handles_count; ++i)
    {
        thread_destroy(handles[i]);
    }

    mutex_destroy(&mutex_add_handle);
}
void async_wait()
{
    while (main_counter != 0);
}

void add_handler(thread_t handle)
{
    mutex_lock(&mutex_add_handle);

    handles[handles_count++] = handle;

    mutex_unlock(&mutex_add_handle);
}

void async_init()
{
    mutex_create(&mutex_add_handle);
}

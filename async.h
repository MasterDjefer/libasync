#ifndef ASYNC_H
#define ASYNC_H


#include <stdio.h>
#include <string.h>
#include <functional>

#ifdef _WIN32
    #include <winsock2.h>
    #include <windows.h>
#elif __linux__
    #include <pthread.h>
    #include <unistd.h>
#endif


#ifdef _WIN32
    typedef mutex_t HANDLE;
    typedef thread_t HANDLE;
#elif __linux__
    typedef pthread_mutex_t mutex_t;
    typedef pthread_t thread_t;
#endif




#ifdef _WIN32
    #define mutex_lock(mutex) WaitForSingleObject(*mutex, INFINITE);
    #define mutex_unlock(mutex) ReleaseMutex(*mutex);
    #define mutex_create(mutex) CreateMutex(NULL, FALSE, NULL);
    #define mutex_destroy(mutex) CloseHandle(*mutex);
    #define thread_create(thread, thread_func, args) CreateThread(NULL, 0, thread_func, args, 0, NULL);
    #define thread_destroy(thread) CloseHandle(*thread);
#elif __linux__
    #define mutex_lock(mutex) pthread_mutex_lock(mutex);//pointer
    #define mutex_unlock(mutex) pthread_mutex_unlock(mutex);
    #define mutex_create(mutex) pthread_mutex_init(mutex, NULL);
    #define mutex_destroy(mutex) pthread_mutex_destroy(mutex);
    #define thread_create(thread, thread_func, args) pthread_create(thread, NULL, thread_func, args);
    #define thread_destroy(thread)
#endif


#ifdef _WIN32
    #define declare_thread_func(name, params) DWORD WINAPI name(LPVOID params)
#elif __linux__
    #define declare_thread_func(name, params) void* name(void* params)
#endif


void main_counter_inc();
void main_counter_dec();

void add_handler(thread_t handle);

void async_wait();
void async_init();
void async_clean();


#endif // ASYNC_H

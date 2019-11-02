#ifndef FS_H
#define FS_H

#include "async.h"

typedef void(*read_file_callback_t)(const char*);

struct file_data_t
{
    const char* file_name;
    read_file_callback_t callback;
};

extern mutex_t mutex_callback_call;

void read_file(const char* file_name, read_file_callback_t callback);


#endif // FS_H

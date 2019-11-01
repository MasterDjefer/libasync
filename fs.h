#ifndef FS_H
#define FS_H

#include "async.h"


void read_file(const char* file_name, read_file_callback_t callback);
void fs_init();
void fs_clean();


#endif // FS_H

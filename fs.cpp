#include "fs.h"

#ifdef _WIN32
int getline(char** buffer, size_t* size, FILE* file_name)
{
    *size = 120;
    if (!*buffer)
    {
        *buffer = (char*)malloc(*size);
    }

    memset(*buffer, 0, *size);
    fgets(*buffer, *size, file_name);

    int bytes_read = strlen(*buffer);
    return bytes_read ? bytes_read : -1;
}
#endif


declare_thread_func(read_file_func, params)
{
    const char* file_name = ((file_data_t*)params)->file_name;
    read_file_callback_t callback = ((file_data_t*)params)->callback;

    free(params);

    FILE* file;
    file = fopen(file_name, "r");

    if (!file)
    {
        printf("file not found\n");
        exit(0);
    }

    char buffer[1000];
    int buffer_size = 0;
    memset(buffer, 0, sizeof(buffer));

    char* line = NULL;
    size_t line_size = 0;

    int bytes_read = 0;
    while ((bytes_read = getline(&line, &line_size, file)) != -1)
    {
        memcpy(buffer + buffer_size, line, bytes_read);
        buffer_size += bytes_read;

        //free the line memory! malloc used!
    }
    free(line);

    fclose(file);


    mutex_lock(&mutex_callback_call);
    callback(buffer);
    mutex_unlock(&mutex_callback_call);


    main_counter_dec();
    return 0;
}

void read_file(const char* file_name, read_file_callback_t callback)
{
    main_counter_inc();

    file_data_t *file_data = (file_data_t*)malloc(sizeof(file_data_t));
    memset(file_data, 0, sizeof(file_data_t));
    file_data->file_name = file_name;
    file_data->callback = callback;


    thread_t thread;
    thread_create(&thread, read_file_func, file_data);


    add_handler(thread);
}

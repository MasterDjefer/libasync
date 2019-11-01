#include "async.h"
#include "fs.h"


int main()
{
    async_init();
    fs_init();

    printf("haha\n");

    read_file("file1", [](const char* data) {
        printf("%s", data);

        read_file("file2", [](const char* data) {
            printf("%s", data);
        });
        read_file("file2", [](const char* data) {
            printf("%s", data);

            read_file("file1", [](const char* data) {
                printf("%s", data);
            });
        });
    });

    printf("haha\n");

    async_wait();

    fs_clean();
    async_clean();

    return 0;
}

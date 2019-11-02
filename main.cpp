#include "async.h"
#include "fs.h"
#include "timer.h"


int main()
{
    async_init();

    set_interval([](){
        printf("sdf\n");
    }, 2000);

    set_timeout([](){
        printf("123\n");
    }, 4000);
    set_timeout([](){
        printf("000\n");
    }, 6000);




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

    async_clean();

    return 0;
}

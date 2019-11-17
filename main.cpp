#include "async.h"
#include "fs.h"
#include "timer.h"


int main()
{
    async_init();

    timers_t timer = set_interval([](){
        printf("123\n");
    }, 2000);

//    sleep(5000);
    clear_timeout(timer);


    set_timeout([](){
        printf("123\n");
    }, 1000);
    set_timeout([](){
        printf("000\n");

        set_timeout([](){
            printf("inner\n");
        }, 2000);

    }, 2000);


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

    return 0;
}

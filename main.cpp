#include <iostream>
#include <fstream>
#include <cstring>
#include <functional>

#ifdef _WIN32
    #include <winsock2.h>
    #include <windows.h>
#elif __linux__
    #include <pthread.h>
#endif

using namespace std;

typedef void(*ReadFileCallback)(const char*);

struct FileData
{
    const char* fileName;
    ReadFileCallback callback;
};


#ifdef _WIN32
    typedef mutex_t HANDLE;
    typedef thread_t HANDLE;
#elif __linux__
    typedef pthread_mutex_t mutex_t;
    typedef pthread_t thread_t;
#endif


#ifdef _WIN32
    #define close_handle(handle) CloseHandle(handle);
    #define mutex_lock(mutex) WaitForSingleObject(*mutex, INFINITE);
    #define mutex_unlock(mutex) ReleaseMutex(*mutex);
    #define mutex_create(mutex) CreateMutex(NULL, FALSE, NULL);
    #define thread_create(thread, thread_func, args) CreateThread(NULL, 0, thread_func, args, 0, NULL);
#elif __linux__
    #define close_handle(handle)
    #define mutex_lock(mutex) pthread_mutex_lock(mutex);//pointer
    #define mutex_unlock(mutex) pthread_mutex_unlock(mutex);
    #define mutex_create(mutex) pthread_mutex_init(mutex, NULL);
    #define thread_create(thread, thread_func, args) pthread_create(thread, NULL, thread_func, args);
#endif


#ifdef _WIN32
#define thread_func(name, params) DWORD WINAPI name(LPVOID params)
#elif __linux__
#define thread_func(name, params) void* name(void* params)
#endif



mutex_t mutexSetHandle;
mutex_t mutexCallbackCall;

thread_t handlesAll[100];
int handlesAllCounter = 0;

int mainCounter = 0;

void cleanAll()
{
    for (int i = 0; i < handlesAllCounter; ++i)
    {
        close_handle(handlesAll[i]);
    }

    close_handle(mutexSetHandle);
    close_handle(mutexCallbackCall);
}

void setHandleForWaiting(thread_t handle)
{
    mutex_lock(&mutexSetHandle);
    handlesAll[handlesAllCounter++] = handle;
    mutex_unlock(&mutexSetHandle);
}
bool wait()
{
    return mainCounter != 0;
}

thread_func(read_file_func, params)
{
    const char* fileName = ((FileData*)params)->fileName;
    ReadFileCallback callback = ((FileData*)params)->callback;

    std::ifstream file;
    file.open(fileName);
    if (!file.is_open())
    {
        throw 1;
    }

    std::string dataLine = "";
    std::getline(file, dataLine);
    file.close();

    mutex_lock(&mutexCallbackCall);
    callback(dataLine.c_str());
    mutex_unlock(&mutexCallbackCall);

    free(params);

    mainCounter--;
    return 0;
}

void readFile(const char* fileName, ReadFileCallback callback)
{
    mainCounter++;

    FileData *fileData = (FileData*)malloc(sizeof(FileData));
    memset(fileData, 0, sizeof(fileData));
    fileData->fileName = fileName;
    fileData->callback = callback;


    thread_t thread;
    thread_create(&thread, read_file_func, fileData);


    setHandleForWaiting(thread);
}

//void CALLBACK f(HWND hwnd, UINT uMsg, UINT timerId, DWORD dwTime)
//{
//    printf("Hello\n");
//}


int main()
{
    mutex_create(&mutexSetHandle);
    mutex_create(&mutexCallbackCall);

    printf("haha\n");
    readFile("file1", [](const char* data) {
        printf("%s\n", data);

        readFile("file2", [](const char* data) {
            printf("%s\n", data);
        });
        readFile("file2", [](const char* data) {
            printf("%s\n", data);

            readFile("file1", [](const char* data) {
                printf("%s\n", data);

                readFile("file1", [](const char* data) {
                    printf("%s\n", data);
                });
            });
        });

    });


    printf("haha\n");

    while (wait());

    cleanAll();
    //SetWaitableTimer()
    //    int a = SetTimer(NULL, 0, 1000,(TIMERPROC) f);
    //    WaitForSingleObject((int*)a, INFINITE);

    return 0;

}

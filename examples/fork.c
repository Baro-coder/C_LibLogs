#include <logs.h>

#ifdef _WIN32
    #include <windows.h>
#elif defined(__linux__)
    #include <sys/wait.h>
    #include <sys/types.h>
    #include <pthread.h>
#elif defined(__APPLE__)
    #include <TargetConditionals.h>
    #if TARGET_OS_MAC
        #include <sys/wait.h>
    #endif
#else
    #error "Unsupported OS"
#endif

#define P_COUNT 20

void* thread_body(void* arg) {
    int id = *((int*)arg);
    logs_log_debug("Sub", "P%d -- Init...", id);
    logs_log(LOG_LEVEL_DEBUG, "Sub", "P%d -- End of work.", id);
    free(id);
    return NULL;
}


int main(void) {
    logs_log_info("App", "Initializing...");
    logs_threads_safety_enable("/semTest");
    
#ifdef _WIN32
    HANDLE threads[P_COUNT];
#else
    pthread_t threads[P_COUNT];
#endif

    for(int i = 0; i < P_COUNT; i++) {
        int* id = malloc(sizeof(int));
        if (id == NULL) {
            logs_log_error("App", "Memory allocation failed.");
            return 1;
        }
        *id = i;

#ifdef _WIN32
        threads[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_body, id, 0, NULL);
        if (threads[i] == NULL) {
            logs_log_error("App", "Thread creation failed.");
            return 1;
        }
#else
        if(pthread_create(&threads[i], NULL, thread_body, id) != 0) {
            logs_log_error("App", "Thread creation failed.");
            return 1;
        }
#endif
    }

    logs_log_trace("App", "Waiting...");

    for(int i = 0; i < P_COUNT; i++) {
#ifdef _WIN32
        WaitForSingleObject(threads[i], INFINITE);
        CloseHandle(threads[i]);
#else
        pthread_join(threads[i], NULL);
#endif
    }

    logs_threads_safety_disable("/semTest");
    logs_log_info("App", "Done.");

    return 0;
}
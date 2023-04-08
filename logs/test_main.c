/* *** Includes *** */
// -- Built-in

// -- Logs
#include "logs.h"
#include <sys/wait.h>

#define  TIMESTAMP_BUFF_SIZE 20

void* thread_body(int id) {
    logs_log_info("Sub", "P%d -- Init...", id);

    for(unsigned int i = 0; i < 1000000; i++) {
        for(unsigned int j = 0; j < 1000; j++) {
            ;;
        }
    }
    sleep(1);
    logs_log_info("Sub", "P%d -- End of work.", id);

    return NULL;
}


int main(void) {
    int p_count = 10;

    logs_log_info("App", "Initializing...");
    logs_threads_safety_enable("/semTest");
    
    for(int i = 0; i < p_count; i++) {
        if(fork() == 0) {
            thread_body(i);
            exit(0);
        }
    }

    logs_log_info("App", "Waiting...");
    for(int i = 0; i < p_count; i++) {
        wait(NULL);
    }

    logs_threads_safety_disable("/semTest");
    logs_log_info("App", "Done.");

    return 0;
}
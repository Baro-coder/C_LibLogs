#include <logs.h>
#include <sys/wait.h>

void* thread_body(int id) {
    logs_log_debug("Sub", "P%d -- Init...", id);
    
    logs_log(LOG_LEVEL_DEBUG, "Sub", "P%d -- End of work.", id);

    return NULL;
}


int main(void) {
    int p_count = 10;

    logs_log_info("App", "Initializing...");
    logs_threads_safety_enable("/semTest");
    
    for(int i = 0; i < p_count; i++) {
        thread_body(i);
    }

    logs_log_trace("App", "Waiting...");
    for(int i = 0; i < p_count; i++) {
        wait(NULL);
    }

    logs_threads_safety_disable("/semTest");
    logs_log_info("App", "Done.");

    return 0;
}
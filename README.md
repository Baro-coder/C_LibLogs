# ***C LibLogs***

## **Content:**

1. [Overview](#overview)
2. [Installation](#installation)
3. [Usage](#usage)
4. [Example](#example)
5. [License](#license)

## **Overview**

### **Description**

Simple logging library developed in C, providing structured log messages with timestamps, log levels, process IDs, and formatted messages. The library supports multi-threaded applications by implementing semaphore-based synchronization.

*Example output:*

``` text
[08-04-2023 15:14:22] | [ INFO  ] | [App (19683)] | Initializing...
[08-04-2023 15:14:22] | [ DEBUG ] | [Sub (19685)] | P1 -- Init...
[08-04-2023 15:14:22] | [ DEBUG ] | [Sub (19684)] | P0 -- Init...
[08-04-2023 15:14:22] | [ DEBUG ] | [Sub (19686)] | P2 -- Init...
[08-04-2023 15:14:22] | [ TRACE ] | [App (19683)] | Waiting...
[08-04-2023 15:14:25] | [ DEBUG ] | [Sub (19686)] | P2 -- End of work.
[08-04-2023 15:14:25] | [ DEBUG ] | [Sub (19685)] | P1 -- End of work.
[08-04-2023 15:14:25] | [ DEBUG ] | [Sub (19684)] | P0 -- End of work.
[08-04-2023 15:14:25] | [ INFO  ] | [App (19683)] | Done.
```

*Format:*

``` text
[datetime] | [level] | [owner(PID)] | message
```

### **Interface**

``` c
/* --- LOG WRITERS */
void logs_log(log_level_t level, const char* owner, const char* fmt, ...);
void logs_log_trace(const char* owner, const char* fmt, ...);
void logs_log_debug(const char* owner, const char* fmt, ...);
void logs_log_info(const char* owner, const char* fmt, ...);
void logs_log_warn(const char* owner, const char* fmt, ...);
void logs_log_error(const char* owner, const char* fmt, ...);
void logs_log_fatal(const char* owner, const char* fmt, ...);
```

### **Log Levels**

``` c
enum __log_level_t{
    LOG_LEVEL_TRACE,    // --> 0
    LOG_LEVEL_DEBUG,    // --> 1
    LOG_LEVEL_INFO,     // --> 2
    LOG_LEVEL_WARNING,  // --> 3
    LOG_LEVEL_ERROR,    // --> 4
    LOG_LEVEL_FATAL};   // --> 5

typedef enum __log_level_t log_level_t;
```

### **Other functions**

``` c
/* --- OPTIONS */
int logs_threads_safety_enable(const char* sem_name);
int logs_threads_safety_disable(const char* sem_name);

void logs_set_minimal_log_level(log_level_t level);

void logs_set_output_stream(FILE* stream);
void logs_set_output_stream_default();
```

---

## **Installation**

### **CMake Build System**

To simplify library integration, I provide a `CMakeLists.txt` configuration. You can include the library in your project with:

``` console
mkdir build && cd build
cmake ..
make
```

***Additionally***, if you want the library to be available **globally** (not only in an actual project), you can eventually execute the following command:

``` console
sudo make install
```

***Alternatively***, include the library in your CMake project with:

``` cmake
add_subdirectory(c-liblogs)
target_link_libraries(your_project PRIVATE c-liblogs)
```

### **Manual Installation**

1. Clone repository:

``` console

```

2. Build:

``` console

```

3. Install:

``` console

```

---

## **Usage**

### **Including in project**

``` c
#include <logs.h>
```

### **Compiling with the library**

``` terminal
gcc your_program.c -o your_program -llogs
```

---

## **Example**

### **fork.c**

``` c
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
```

### **Running the example**

``` console
gcc fork.c -o fork.exe -llogs
./fork.exe
```

---

## **License**

Library is full **open-source software**. You can redistribute it and/or modify it under the terms of the **GNU General Public License v3**.

See [*LICENSE*](https://github.com/Baro-coder/C_LibLogs/blob/master/LICENSE) for details.

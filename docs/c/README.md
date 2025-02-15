# ***Logs***

## ***C*** implementation

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

#### **Threads safety mode**

When using the library in multi-threaded or multi-process applications, it is necessary to synchronize access to shared logging resources. This can be done by enabling thread safety mode with a named semaphore:

``` c
// Enable thread safety mode using a named semaphore
logs_threads_safety_enable("/semTest");
```

After enabling thread safety, all log operations will be synchronized, ensuring that multiple threads or processes do not write logs simultaneously in a conflicting manner.

Once logging is no longer needed, disable thread safety to release system resources:

``` c
// Disable thread safety mode
logs_threads_safety_disable("/semTest");
```

Failing to disable thread safety after usage may lead to unnecessary resource consumption or locking issues.

#### **Set minimal log level**

You can control the verbosity of logs using `logs_set_minimal_log_level()`. For example:

``` c
// Only log messages with level INFO and above
logs_set_minimal_log_level(LOG_LEVEL_INFO);
```

#### **Logging to a File**

By default, logs are written to `stderr`. You can redirect logs to a file:

``` c
FILE* logfile = fopen("logs.txt", "w");
if (logfile) {
    logs_set_output_stream(logfile);
}

// -- Program code begin --
// ...
// -- Program code end --

if (logfile) {
    fclose(logfile);
}
```

---

## **Installation**

### **Linux Installation**

#### **Using CMake**

1. Install CMake Build System Tools with:

``` console
sudo apt install -y cmake
```

2. Open a terminal in your project directory and run:

``` console
mkdir build && cd build
cmake ..
make
```

***Additionally***, if you want the library to be available **globally** (not only in an actual project), you can eventually execute the following command:

``` console
sudo make install
```

In this way, the library will be installed in `/usr/local/lib` and `/usr/local/include/`. It can be necessary to update `LD_LIBRARY_PATH` with:

``` sh
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
```

***Alternatively***, include the library in your CMake project with:

``` cmake
add_subdirectory(c-liblogs)
target_link_libraries(your_project PRIVATE c-liblogs)
```

#### **Manual Installation**

1. Clone repository:

``` console
git clone https://github.com/Baro-coder/C_LibLogs
cd C_LibLogs
```

2. Build:

``` console
gcc -c ./src/logs.c -o ./src/log.o -Iinclude
ar crs ./src/liblogs.a ./src/log.o
```

3. Install:

``` console
sudo cp ./include/logs.h /usr/local/include/
sudo cp ./src/liblogs.a /usr/local/lib/
```

To compile your program using the library, use:

``` console
gcc your_program.c -o your_program -llogs
```

### **Windows Installation**

#### **Using CMake**

1. Install [CMake](https://cmake.org/download/) and [MinGW-w64](https://www.mingw-w64.org/) if not already installed.

2. Open a terminal in the project directory and run:

``` console
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
```

***Alternatively***, include the library in your CMake project with:

``` cmake
add_subdirectory(c-liblogs)
target_link_libraries(your_project PRIVATE c-liblogs)
```

#### **Manual Installation**

1. Clone repository:

``` console
git clone https://github.com/Baro-coder/C_LibLogs
cd C_LibLogs
```

2. Build:

``` console
gcc -c ./src/logs.c -o ./src/log.o -Iinclude
ar crs ./src/liblogs.a ./src/log.o
```

3. Copy necessary files to your project directory:

``` console
copy include\logs.h C:\your_project\include
copy src\liblogs.a C:\your_project\lib\
```

***Alternatively***, if you want the library to be available **globally** (not only in an actual project), you can copy the files to MinGW resources with:

``` console
copy include\logs.h C:\MinGW\include
copy src\liblogs.a C:\MinGW\lib\
```

To compile your program using the library, use:

``` console
gcc your_program.c -o your_program.exe -Lyour_project/lib -llogs -Iyour_project/include
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

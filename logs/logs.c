/*
*  Copyright (c) 2023 Baro-coder
* 
*  Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to
*  deal in the Software without restriction, including without limitation the
*  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
*  sell copies of the Software, and to permit persons to whom the Software is
*  furnished to do so, subject to the following conditions:
* 
*  The above copyright notice and this permission notice shall be included in
*  all copies or substantial portions of the Software.
* 
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
*  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
*  IN THE SOFTWARE.
*/

/* ---------------------------------------------------------------------------------------------------- */

/* -- Header -- */
#include "logs.h"


/* -- STATIC VARS -- */

// -- Stream
static FILE* stream = NULL;

// -- Thread-Safety Semaphore
static sem_t* mtx = NULL;

// -- Level min
static log_level_t level_min = trace;

// -- Log level names
const char* __LEVEL_NAMES[] = {
    "  TRACE  ",
    "  DEBUG  ",
    "  INFO   ",
    " WARNING ",
    "  ERROR  ",
    "  FATAL  "
};

/* ---------------------------------------------------------------------------------------------------- */
/* -- PRIVATE FUNCTIONS -- */

// Print timestamp, level name and owner
void __log_preprint(log_level_t level, const char* owner) {
    if (mtx != NULL) {
        sem_wait(mtx);
    }

    time_t *now = (time_t*) malloc(sizeof(time_t));
    time(now);
    struct tm *tm = localtime(now);

    fprintf(stderr, "[%d-%02d-%02d %02d:%02d:%02d] : [%s] : [%s] : ", 
        tm->tm_mday, 
        tm->tm_mon + 1,
        tm->tm_year + 1900, 
        tm->tm_hour, 
        tm->tm_min, 
        tm->tm_sec, 
        __LEVEL_NAMES[level], 
        owner);

    free(now);
}

bool_t __check_level(log_level_t level) {
    return (level >= level_min);
}

/* ---------------------------------------------------------------------------------------------------- */
/* -- PUBLIC FUNCTIONS -- */

// ---- Threads-Safety semaphore init
int logging_threads_safety_enable(const char* sem_name) {
    mtx = sem_open(sem_name, O_CREAT | O_EXCL, 0666, 1);
    if (mtx == SEM_FAILED) {
        return 1;
    }
    return 0;
}
// ---- Threads-Safety semaphore destroy
int logging_threads_safety_disable(const char* sem_name) {
    if (sem_close(mtx) < 0) {
        return 2;
    }
    if (sem_unlink(sem_name) < 0) {
        return 1;
    }
    
    mtx = NULL;

    return 0;
}

// ---- Minimal log level set
void set_minimal_log_level(log_level_t level) {
    level_min = level;
}



// ---- 'SPECIFIED' LOG
void log(log_level_t level, const char* owner, const char* fmt, ...) {
    if(!__check_level(level)) {
        return;
    }

    __log_preprint(level, owner);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    va_end(args);

    if(mtx != NULL) {
        sem_post(mtx);
    }
}
// ---- TRACE LOG
void log_trace(const char* owner, const char* fmt, ...) {
    if(!__check_level(trace)) {
        return;
    }

    __log_preprint(trace, owner);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    va_end(args);

    if(mtx != NULL) {
        sem_post(mtx);
    }
}
// ---- DEBUG LOG
void log_debug(const char* owner, const char* fmt, ...) {
    if(!__check_level(debug)) {
        return;
    }

    __log_preprint(debug, owner);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    va_end(args);

    if(mtx != NULL) {
        sem_post(mtx);
    }
}
// ---- INFO LOG
void log_info(const char* owner, const char* fmt, ...) {
    if(!__check_level(info)) {
        return;
    }

    __log_preprint(info, owner);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    va_end(args);

    if(mtx != NULL) {
        sem_post(mtx);
    }
}
// ---- WARNING LOG
void log_warn(const char* owner, const char* fmt, ...) {
    if(!__check_level(warning)) {
        return;
    }

    __log_preprint(warning, owner);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    va_end(args);

    if(mtx != NULL) {
        sem_post(mtx);
    }
}
// ---- ERROR LOG
void log_error(const char* owner, const char* fmt, ...) {
    if(!__check_level(error)) {
        return;
    }

    __log_preprint(error, owner);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    va_end(args);

    if(mtx != NULL) {
        sem_post(mtx);
    }
}
// ---- FATAL LOG
void log_fatal(const char* owner, const char* fmt, ...) {
    if(!__check_level(fatal)) {
        return;
    }

    __log_preprint(fatal, owner);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    va_end(args);

    if(mtx != NULL) {
        sem_post(mtx);
    }
}

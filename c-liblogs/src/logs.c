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
static FILE *out_stream = NULL;
static bool_t mute_std_streams = 0;

#ifdef _WIN32
// -- Thread-Safety Semaphore
static HANDLE mtx = NULL;

// -- Get username
static char *get_username()
{
    static char username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    GetUserName(username, &username_len);
    if (strlen(username) == 0 || username == NULL || strlen(username) > OWNER_BUFF_SIZE)
    {
        return "unknown";
    }
    return username;
}

#elif __linux__
// -- Thread-Safety Semaphore
static sem_t *mtx = NULL;

// -- Get username
static char *get_username()
{
    struct passwd *pw = getpwuid(getuid());
    if (pw && pw->pw_name)
    {
        if (strlen(pw->pw_name) == 0 || strlen(pw->pw_name) > OWNER_BUFF_SIZE)
        {
            return "unknown";
        }
        return pw->pw_name;
    }
    return "unknown";
}
#endif

// -- Owner default
#define OWNER_DEFAULT get_username()

// -- Level min
static log_level_t level_min = LOG_LEVEL_TRACE;

// -- Log level names
const char *__LEVEL_NAMES[] = {
    "  TRACE  ",
    "  DEBUG  ",
    "  INFO   ",
    " WARNING ",
    "  ERROR  ",
    "  FATAL  "};

/* ---------------------------------------------------------------------------------------------------- */
/* -- PRIVATE FUNCTIONS -- */

// Print log
void __log_print(log_level_t level, const char *owner, const char *fmt, va_list args)
{
    // -- Getting datetime data
    time_t *now = (time_t *)malloc(sizeof(time_t));
    time(now);
    struct tm *tm = localtime(now);

    char *timestamp = (char *)malloc(TIMESTAMP_BUFF_SIZE * sizeof(char));
    sprintf(timestamp, "%02d-%02d-%02d %02d:%02d:%02d",
            tm->tm_mday,
            tm->tm_mon + 1,
            tm->tm_year + 1900,
            tm->tm_hour,
            tm->tm_min,
            tm->tm_sec);

#ifdef _WIN32
    // -- Mutex Lock
    if (mtx != NULL)
    {
        WaitForSingleObject(mtx, INFINITE);
    }
#elif __linux__
    // -- Mutex Lock
    if (mtx != NULL)
    {
        sem_wait(mtx);
    }
#endif

    // -- Log Output
    if (out_stream == NULL)
    {
        if (mute_std_streams == TRUE)
        {
            return;
        }
        // -- Output to stderr
        fprintf(stderr, "[%s] | [%s] | [%s (%d)] | ",
                timestamp,
                __LEVEL_NAMES[level],
                owner,
                getpid());

        vfprintf(stderr, fmt, args);
        va_end(args);
        fputc('\n', stderr);
    }
    else
    {
        if (out_stream != stdout && out_stream != stderr && mute_std_streams == FALSE)
        {
            // -- Copy args
            va_list args_copy;
            va_copy(args_copy, args);

            // -- Output copy to stderr
            fprintf(stderr, "[%s] | [%s] | [%s (%d)] | ",
                    timestamp,
                    __LEVEL_NAMES[level],
                    owner,
                    getpid());
            vfprintf(stderr, fmt, args_copy);
            va_end(args_copy);
            fputc('\n', stderr);
        }

        // -- Output to out_stream
        fprintf(out_stream, "[%s] | [%s] | [%s (%d)] | ",
                timestamp,
                __LEVEL_NAMES[level],
                owner,
                getpid());

        vfprintf(out_stream, fmt, args);
        va_end(args);
        fputc('\n', out_stream);
    }

    // -- Memory free
    free(now);
    free(timestamp);

#ifdef _WIN32
    // -- Mutex Unlock
    if (mtx != NULL)
    {
        ReleaseSemaphore(mtx, 1, NULL);
    }
#elif __linux__
    // -- Mutex Unlock
    if (mtx != NULL)
    {
        sem_post(mtx);
    }
#endif
}

// Check minimal level
static bool_t __check_min_level(log_level_t level)
{
    return (level >= level_min);
}

// Check level and print log
void __log(log_level_t level, const char *owner, const char *fmt, va_list args)
{
    if (!__check_min_level(level))
    {
        return;
    }

    if (owner == NULL || strcmp(owner, "") == 0 || strcmp(owner, " ") == 0 || owner[0] == '\0')
    {
        __log_print(level, OWNER_DEFAULT, fmt, args);
    }
    else
    {
        __log_print(level, owner, fmt, args);
    }
}

/* ---------------------------------------------------------------------------------------------------- */
/* -- PUBLIC FUNCTIONS -- */
#ifdef _WIN32
// ---- Threads-Safety semaphore init
int logs_threads_safety_enable(const char *sem_name)
{
    mtx = CreateSemaphore(NULL, 1, 1, sem_name);
    if (mtx == NULL)
    {
        return 1;
    }
    return 0;
}
// ---- Threads-Safety semaphore destroy
int logs_threads_safety_disable(const char *sem_name)
{
    if (mtx == NULL)
    {
        return 2;
    }

    if (!CloseHandle(mtx))
    {
        return 1;
    }

    mtx = NULL;
    return 0;
}
#elif __linux__
// ---- Threads-Safety semaphore init
int logs_threads_safety_enable(const char *sem_name)
{
    mtx = (sem_t *)malloc(sizeof(sem_t));
    mtx = sem_open(sem_name, O_CREAT | O_EXCL, 0666, 1);
    if (mtx == SEM_FAILED)
    {
        return 1;
    }
    return 0;
}
// ---- Threads-Safety semaphore destroy
int logs_threads_safety_disable(const char *sem_name)
{
    if (sem_close(mtx) < 0)
    {
        return 2;
    }

    mtx = NULL;

    if (sem_unlink(sem_name) < 0)
    {
        return 1;
    }

    return 0;
}
#endif

// ---- Minimal log level set
void logs_set_minimal_log_level(log_level_t level)
{
    level_min = level;
}

// ---- Output stream set `stream`
void logs_set_output_stream(FILE *stream)
{
    out_stream = stream;
}
// ---- Output stream set DEFAULT
void logs_set_output_stream_default()
{
    logs_set_output_stream(NULL);
}
// ---- Mute standard streams
void logs_mute_std_streams(bool_t mute)
{
    mute_std_streams = mute;
}

// ---- 'SPECIFIED' LOG
void logs_log(log_level_t level, const char *owner, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    __log(level, owner, fmt, args);
    va_end(args);
}
// ---- TRACE LOG
void logs_log_trace(const char *owner, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    __log(LOG_LEVEL_TRACE, owner, fmt, args);
    va_end(args);
}
// ---- DEBUG LOG
void logs_log_debug(const char *owner, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    __log(LOG_LEVEL_DEBUG, owner, fmt, args);
    va_end(args);
}
// ---- INFO LOG
void logs_log_info(const char *owner, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    __log(LOG_LEVEL_INFO, owner, fmt, args);
    va_end(args);
}
// ---- WARNING LOG
void logs_log_warn(const char *owner, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    __log(LOG_LEVEL_WARNING, owner, fmt, args);
    va_end(args);
}
// ---- ERROR LOG
void logs_log_error(const char *owner, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    __log(LOG_LEVEL_ERROR, owner, fmt, args);
    va_end(args);
}
// ---- FATAL LOG
void logs_log_fatal(const char *owner, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    __log(LOG_LEVEL_FATAL, owner, fmt, args);
    va_end(args);
}

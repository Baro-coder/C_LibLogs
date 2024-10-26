/*
 *  Copyright (c) 2023 Baro-coder
 *
 *  This library is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU GENERAL PUBLIC LICENSE v3. See `logs.c` for details.
 */

/* *** Info *** */
/*
 *   -- Formats --
 *  Log:          [DATETIME] : [LEVEL] : [OWNER] : MESSAGE
 *  Datetime:     %dd-%MM-%yyyy %hh:%mm:%ss
 *
 *   -- Examples --
 *   [5-04-2023 17:49:41] : [ DEBUG ] : [ App ] : Control mechanisms init...
 *   [5-04-2023 17:52:32] : [ INFO  ] : [ C-4 ] : Init...
 *   [5-04-2023 17:53:16] : [ INFO  ] : [ C-4 ] : Ready.
 */

#ifndef LOGS_H
#define LOGS_H

/* *** INCLUDES *** */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <time.h>

/* *** DEFINITIONS *** */

#define OWNER_BUFF_SIZE 128    // Buffer size for owner name
#define TIMESTAMP_BUFF_SIZE 72 // Buffer size for timestamp

// Custom boolean type
// -- Type for boolean values
typedef int bool_t;

// -- True / False values
#define TRUE 1
#define FALSE 0

// -- Log levels
// Log levels enumeration
typedef enum __log_level_t
{
    LOG_LEVEL_TRACE = 0,   // Trace
    LOG_LEVEL_DEBUG = 1,   // Debug
    LOG_LEVEL_INFO = 2,    // Info
    LOG_LEVEL_WARNING = 3, // Warning
    LOG_LEVEL_ERROR = 4,   // Error
    LOG_LEVEL_FATAL = 5    // Fatal
} log_level_t;

/* *** INTERFACE *** */

/* --- OPTIONS */

#ifdef __linux__
#include <semaphore.h>

/// @brief Creating semaphore with name as `sem_name` to sync threads printing.
/// @attention Remember to call `logs_threads_safety_disable()` to remove created semaphore!
/// @param sem_name semaphore name
/// @return `0` on success; `1` - sem_open error
int logs_threads_safety_enable(const char *sem_name);

/// @brief Removing semaphore with name as `sem_name` to sync threads printing.
/// @param sem_name semaphore name
/// @return `0` on success; `1` - sem_unlink error; `2` - sem_close error
int logs_threads_safety_disable(const char *sem_name);
#endif

/// @brief Set minimal log level to `level`
/// @param level minimal log level
void logs_set_minimal_log_level(log_level_t level);

/// @brief Set log output stream to `stream`
/// @param stream output stream
/// @attention If `stream` is `NULL`, output stream will be set to `stderr`
void logs_set_output_stream(FILE *stream);

/// @brief Set log output stream to `stderr`
void logs_set_output_stream_default();

/// @brief Prevent standard streams from printing logs
/// @param mute `TRUE` (`1`) to mute; `FALSE` (`0`) to unmute
/// @attention If `mute` is `TRUE`, logs will not be printed to standard streams
void logs_mute_std_streams(bool_t mute);

/* --- LOG WRITERS */

// Write `level` log with `owner` and message as formatted output

/// @brief Write log with `level`, `owner` and message as formatted output
/// @param level log level
/// @param owner log owner
/// @param fmt message format
/// @param ... message arguments
/// @attention If `owner` is `NULL`, it will be set to name of user who called the function
void logs_log(log_level_t level, const char *owner, const char *fmt, ...);

/// @brief Write TRACE log with `owner` and message as formatted output
/// @param owner log owner
/// @param fmt message format
/// @param  ... message arguments
/// @attention If `owner` is `NULL`, it will be set to name of user who called the function
void logs_log_trace(const char *owner, const char *fmt, ...);

/// @brief Write TRACE log with `owner` and message as formatted output
/// @param owner log owner
/// @param fmt message format
/// @param  ... message arguments
/// @attention If `owner` is `NULL`, it will be set to name of user who called the function
void logs_log_debug(const char *owner, const char *fmt, ...);

/// @brief Write INFO log with `owner` and message as formatted output
/// @param owner log owner
/// @param fmt message format
/// @param  ... message arguments
/// @attention If `owner` is `NULL`, it will be set to name of user who called the function
void logs_log_info(const char *owner, const char *fmt, ...);

/// @brief Write WARNING log with `owner` and message as formatted output
/// @param owner log owner
/// @param fmt message format
/// @param  ... message arguments
/// @attention If `owner` is `NULL`, it will be set to name of user who called the function
void logs_log_warn(const char *owner, const char *fmt, ...);

/// @brief Write ERROR log with `owner` and message as formatted output
/// @param owner log owner
/// @param fmt message format
/// @param  ... message arguments
/// @attention If `owner` is `NULL`, it will be set to name of user who called the function
void logs_log_error(const char *owner, const char *fmt, ...);

/// @brief Write FATAL log with `owner` and message as formatted output
/// @param owner log owner
/// @param fmt message format
/// @param  ... message arguments
/// @attention If `owner` is `NULL`, it will be set to name of user who called the function
void logs_log_fatal(const char *owner, const char *fmt, ...);

#endif
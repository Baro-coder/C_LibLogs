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

/* *** Includes *** */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>

/* *** Definitions *** */
// -- Log levels
#define LOG_LEVEL_TRACE     0
#define LOG_LEVEL_DEBUG     1
#define LOG_LEVEL_INFO      2
#define LOG_LEVEL_WARNING   3
#define LOG_LEVEL_ERROR     4
#define LOG_LEVEL_FATAL     5

// Custom boolean type
typedef int bool_t;

// Log levels enumeration
enum __log_level_t{trace, debug, info, warning, error, fatal};
typedef enum __log_level_t log_level_t;

/* *** INTERFACE *** */

/* --- OPTIONS */

// Creating semaphore with name as `sem_name` to sync threads printing.
//  `Attention`: Remember to call `logging_threads_safety_disable()` to remove created semaphore!
//  Returns: `0` on success; `1` - sem_open error
int logging_threads_safety_enable(const char* sem_name);

// Removing semaphore with name as `sem_name` to sync threads printing.
//  Returns: `0` on success; `1` - sem_unlink error; `2` - sem_close error
int logging_threads_safety_disable(const char* sem_name);

// Set minimal log level to `level`
void set_minimal_log_level(log_level_t level);



/* --- LOG WRITERS */

// Write `level` log with `owner` and message as formatted output
void log(log_level_t level, const char* owner, const char* fmt, ...);

// Write TRACE log with `owner` and message as formatted output
void log_trace(const char* owner, const char* fmt, ...);

// Write DEBUG log with `owner` and message as formatted output
void log_debug(const char* owner, const char* fmt, ...);

// Write INFO log with `owner` and message as formatted output
void log_info(const char* owner, const char* fmt, ...);

// Write WARNING log with `owner` and message as formatted output
void log_warn(const char* owner, const char* fmt, ...);

// Write ERROR log with `owner` and message as formatted output
void log_error(const char* owner, const char* fmt, ...);

// Write FATAL log with `owner` and message as formatted output
void log_fatal(const char* owner, const char* fmt, ...);

#endif
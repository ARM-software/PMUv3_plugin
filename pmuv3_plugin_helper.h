/*
 * MIT License
 * Copyright (c) [Year] ARM-software
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 */

#ifndef PMUV3_INCLUDES_H
/*
 * Performance Monitoring accessing PMUV3 counters
 * Author: Gayathri  Narayana Yegna Narayanan (gayathrinarayana.yegnanarayanan@arm.com)
 * Description: This plugin supports the initialization of performance monitoring for specific hardware events, reads cycle counts and cleans up the resources after that)
 */
#define PMUV3_INCLUDES_H

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <linux/perf_event.h>
#include <perf/cpumap.h>
#include <perf/threadmap.h>
#include <perf/evsel.h>
#include <internal/tests.h>
#include <sys/mman.h>
#include <time.h>
#include <inttypes.h> // Include inttypes.h for PRIu64 macro
#include <asm/unistd.h>
#include <stdint.h>
#include <linux/kernel.h>
#include <unistd.h> // For sleep functionality in C
//#include "pmuv3.h"

extern struct perf_evsel *global_evsel;
extern struct perf_thread_map *global_threads;
extern struct perf_counts_values global_counts;
extern uint64_t eventnum;
extern int event_names[];
// Function declarations
int test_stat_user_read(int event);
int libperf_print_(enum libperf_print_level level, const char *fmt, va_list ap); //to resolve multiple definition linker error 

#endif // PMUV3_INCLUDES_H

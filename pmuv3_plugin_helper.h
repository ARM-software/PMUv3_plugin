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
#include </home/ubuntu/linux/tools/lib/perf/include/perf/cpumap.h>
#include </home/ubuntu/linux/tools/lib/perf/include/perf/threadmap.h>
#include </home/ubuntu/linux/tools/lib/perf/include/perf/evsel.h>
#include </home/ubuntu/linux/tools/lib/perf/include/internal/tests.h>
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

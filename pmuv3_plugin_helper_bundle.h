#ifndef PMUV3_INCLUDES_H_BUNDLE
/*
 * Performance Monitoring accessing PMUV3 counters
 * Author: Gayathri  Narayana Yegna Narayanan (gayathrinarayana.yegnanarayanan@arm.com)
 * Description: This plugin supports the initialization of performance monitoring for specific hardware events, reads cycle counts and cleans up the resources after that)
 */
#define PMUV3_INCLUDES_H_BUNDLE
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

#define MAX_EVENTS 7

struct PerfData{
    struct perf_evsel *global_evsel[MAX_EVENTS];
};
extern struct PerfData *perf_data;

extern struct perf_thread_map *global_threads;
struct CountData{
    struct perf_counts_values global_count[MAX_EVENTS];
};
extern struct CountData count_data;
#define TOTAL_BUNDLE_NUM 15
extern uint64_t num_events;
extern int num_bundles;
extern int *event_values;
extern char **event_names;
extern struct perf_thread_map *global_threads;
struct PMUv3_Bundle_Data {
    uint64_t start_cnt[MAX_EVENTS];   // Start count
    uint64_t end_cnt[MAX_EVENTS];   // End count
    const char* context;  // Context information
};
extern uint64_t global_index;
extern struct PMUv3_Bundle_Data event_counts[10000];
extern uint64_t eventnum;
int test_stat_user_read(int events[]);
int libperf_print_(enum libperf_print_level level, const char *fmt, va_list ap); //to resolve multiple definition linker error 

// Bundle Groups
typedef const struct{
    const char *name;
    uint8_t event_value;
}bundles; 

extern bundles bundle0[];
extern bundles bundle1[];
extern bundles bundle2[];
extern bundles bundle3[];
extern bundles bundle4[];
extern bundles bundle5[];
extern bundles bundle6[];
extern bundles bundle7[];
extern bundles bundle8[];
extern bundles bundle9[];
extern bundles bundle10[];
extern bundles bundle11[];
extern bundles bundle12[];
extern bundles bundle13[];
extern bundles bundle14[];
#endif // PMUV3_INCLUDES_H_BUNDLE

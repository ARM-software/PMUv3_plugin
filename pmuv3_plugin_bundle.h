/*
 * Performance Monitoring accessing PMUV3 counters
 * Author: Gayathri  Narayana Yegna Narayanan (gayathrinarayana.yegnanarayanan@arm.com)
 * Description: This plugin initializes performance monitoring for specific hardware events, reads cycle counts and cleans up the resources after that. 
 */

#ifndef PMUV3_BUNDLE_H
#define PMUV3_BUNDLE_H
#include "pmuv3_plugin_helper_bundle.h"


void pmuv3_bundle_init(int argc, char *argv[]);
//INITIALIZATION API
//int test_evsel(int argc, char **argv, int event_n);

int test_evsel(int argc, char **argv, int event_vals[]);
//START CYCLE API
//uint64_t get_start_count(struct perf_evsel *global_evsel, struct perf_counts_values *counts);

uint64_t get_start_count(struct PerfData *perf_data, struct CountData *count_data);

//uint64_t get_start_count(struct perf_evsel *global_evsel[], struct perf_counts_values counts[], int index);

//END CYCLE API
//uint64_t get_end_count(struct perf_evsel *global_evsel, struct perf_counts_values *counts);

uint64_t get_end_count(struct PerfData *perf_data, struct CountData *count_data);

//uint64_t get_end_count(struct perf_evsel *global_evsel[], struct perf_counts_values counts[], int index);
//SHUTDOWN API
int shutdown_resources(struct PerfData *perf_data);

//int shutdown_resources(struct perf_evsel *global_evsel[], struct perf_thread_map *global_threads, int num_events);

#endif // PMUV3_BUNDLE_H
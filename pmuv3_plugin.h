/*
 * Performance Monitoring accessing PMUV3 counters
 * Author: Gayathri  Narayana Yegna Narayanan (gayathrinarayana.yegnanarayanan@arm.com)
 * Description: This plugin initializes performance monitoring for specific hardware events, reads cycle counts and cleans up the resources after that. 
 */

#ifndef PMUV3_H
#define PMUV3_H
#include "pmuv3_plugin_helper.h"

//INITIALIZATION API
int test_evsel(int argc, char **argv, int event_n);
int pmuv3_cycle_init();
//START CYCLE API
uint64_t get_start_count(struct perf_evsel *global_evsel, struct perf_counts_values *counts);

//END CYCLE API
uint64_t get_end_count(struct perf_evsel *global_evsel, struct perf_counts_values *counts);

//SHUTDOWN API
int shutdown_resources();

#endif // PMUV3_H



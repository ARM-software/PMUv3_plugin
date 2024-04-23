/*
 * Performance Monitoring accessing PMUV3 counters
 * Author: Gayathri  Narayana Yegna Narayanan (gayathrinarayana.yegnanarayanan@arm.com)
 * Description: This plugin initializes performance monitoring for specific hardware events, reads cycle counts and cleans up the resources after that. 
 */

#ifndef PMUV3_BUNDLE_H
#define PMUV3_BUNDLE_H
#include "pmuv3_plugin_helper_bundle.h"


int pmuv3_bundle_init(int bundle_num);
//INITIALIZATION API
//int test_evsel(int argc, char **argv, int event_n);

int test_evsel(int argc, char **argv, int event_vals[]);

//START CYCLE API
uint64_t process_start_count(struct PerfData *perf_data, struct CountData *count_data);
uint64_t get_start_count(struct PerfData *perf_data, struct CountData *count_data, const char* context, uint64_t index);


//END CYCLE API
uint64_t process_end_count(struct PerfData *perf_data, struct CountData *count_data);
uint64_t get_end_count(struct PerfData *perf_data, struct CountData *count_data, const char* context, uint64_t index);

//SHUTDOWN API
int shutdown_resources(struct PerfData *perf_data);
uint64_t get_next_index();

#endif // PMUV3_BUNDLE_H

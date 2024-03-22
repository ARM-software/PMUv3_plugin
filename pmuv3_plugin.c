/*
 * Performance Monitoring accessing PMUV3 counters
 * Author: Gayathri  Narayana Yegna Narayanan (gayathrinarayana.yegnanarayanan@arm.com)
 * Description: This plugin initializes performance monitoring for specific hardware events, reads cycle counts and cleans up the resources after that. 
 */

#include "pmuv3_plugin.h"

int event_names[] = { PERF_COUNT_HW_CPU_CYCLES, PERF_COUNT_HW_INSTRUCTIONS, PERF_COUNT_HW_CACHE_L1D, PERF_COUNT_HW_CACHE_REFERENCES, PERF_COUNT_HW_CACHE_MISSES, PERF_COUNT_HW_BUS_CYCLES, PERF_COUNT_HW_BRANCH_MISSES, PERF_COUNT_HW_STALLED_CYCLES_FRONTEND, PERF_COUNT_HW_STALLED_CYCLES_BACKEND};

#define NUM_EVENTS sizeof(event_names)/sizeof(int)

uint64_t eventnum = 0;
volatile int arr_rand[0xFFFF]; // Array declaration
volatile int iter = 1;
int tests_failed;
int tests_verbose;

struct perf_evsel *global_evsel;
struct perf_thread_map *global_threads;
struct perf_counts_values global_counts;

#if 1
int libperf_print_(enum libperf_print_level level,
			 const char *fmt, va_list ap)
{
	//return 0;
	return vfprintf(stderr, fmt, ap);
}   
#endif

int pmuv3_cycle_init(){
 __T("test evsel", !test_evsel(0, NULL, event_names[eventnum]));
 return 0;
}
/*
void pmuv3_cycle_init(){
 __T("test evsel", !test_evsel(0, NULL, event_names[eventnum]));
}*/

int test_stat_user_read(int event)
{
	struct perf_counts_values counts = { .val = 0 };
	struct perf_thread_map *threads;
	struct perf_evsel *evsel;
	struct perf_event_mmap_page *pc;
	struct perf_event_attr attr = {
		.type	= PERF_TYPE_HARDWARE,
		.config	= event,

		.config1 = 0x2,		// Request user access 
		/*#ifdef __aarch64__
		  .config1 = 0x2,		// Request user access 
#endif */
	};
	int err, i;

	__u64 start, end, total = 0;

	threads = perf_thread_map__new_dummy();
	__T("failed to create threads", threads);

	perf_thread_map__set_pid(threads, 0, 0);

	evsel = perf_evsel__new(&attr);
	__T("failed to create evsel", evsel);

	err = perf_evsel__open(evsel, NULL, threads);
	__T("failed to open evsel", err == 0);

	err = perf_evsel__mmap(evsel, 0);
	__T("failed to mmap evsel", err == 0);

	pc = perf_evsel__mmap_base(evsel, 0, 0);
	__T("failed to get mmapped address", pc);
	global_evsel = evsel;
	global_threads = threads;
	global_counts = counts;
        global_counts.val = counts.val;
	return 0;

}

// INIT FUNCTION 
int test_evsel(int argc, char **argv, int event_n)
{
	
        __T_START;
	libperf_init(libperf_print_);
        test_stat_user_read(event_n);
        __T_END;
        return tests_failed == 0 ? 0 : -1; 

}

//START CYCLE

uint64_t get_start_count(struct perf_evsel *global_evsel, struct perf_counts_values *counts) {
    // Perform perf_evsel__read operation to get start count
    perf_evsel__read(global_evsel, 0, 0, counts);
    return counts->val;
}

//END CYCLE 

uint64_t get_end_count(struct perf_evsel *global_evsel, struct perf_counts_values *counts) {
    // Perform perf_evsel__read operation to get end count
    perf_evsel__read(global_evsel, 0, 0, counts);
    return counts->val;
}

//SHUTDOWN API

int shutdown_resources() {
	if (global_evsel != NULL) {
		perf_evsel__munmap(global_evsel);
		perf_evsel__close(global_evsel);
		perf_evsel__delete(global_evsel);
		global_evsel = NULL;
	}

	if (global_threads != NULL) {
		// Cleanup operations for threads
		perf_thread_map__put(global_threads);
		global_threads = NULL;
	}
return 0;
}


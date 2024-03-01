/*
 * Performance Monitoring accessing PMUV3 counters
 * Author: Gayathri  Narayana Yegna Narayanan (gayathrinarayana.yegnanarayanan@arm.com)
 * Description: This plugin initializes performance monitoring for specific hardware events, reads cycle counts and cleans up the resources after that. 
 */

#include "pmuv3_plugin_bundle.h"

int tests_failed;
int tests_verbose;
//struct perf_evsel *global_evsel;

/*
struct perf_evsel *global_evsel_0;
struct perf_evsel *global_evsel_1;
struct perf_evsel *global_evsel_2;
struct perf_thread_map *global_threads;
struct perf_counts_values global_count_0;
struct perf_counts_values global_count_1;
struct perf_counts_values global_count_2;*/
struct PerfData *perf_data;
struct CountData count_data;
struct perf_thread_map *global_threads;

uint64_t num_events; 
int *event_values = NULL;
char **event_names=NULL;
uint64_t start_0,start_1,start_2,start_3,start_4,start_5,start_6,start_7,start_8;
uint64_t end_0,end_1,end_2,end_3,end_4,end_5,end_6,end_7,end_8;
/*
#define MAX_EVENTS 3// Define the maximum number of events
struct perf_evsel *global_evsel[MAX_EVENTS];
struct perf_thread_map *global_threads;
struct perf_counts_values global_counts[MAX_EVENTS];
*/


#if 1
int libperf_print_(enum libperf_print_level level,
			 const char *fmt, va_list ap)
{
	//return 0;
	return vfprintf(stderr, fmt, ap);
}   
#endif

/*
int test_stat_user_read(int event)
{
	struct perf_counts_values counts = { .val = 0 };
	struct perf_thread_map *threads;
	struct perf_evsel *evsel;
	struct perf_event_mmap_page *pc;
	struct perf_event_attr attr = {
		.type	= PERF_TYPE_RAW,
		.config	= event,

		.config1 = 0x2,		// Request user access 
		/*#ifdef __aarch64__
		  .config1 = 0x2,		// Request user access 
#endif 
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
*/




int test_stat_user_read(int events[]) {
    //struct perf_counts_values counts[MAX_EVENTS] = {0}; // Array to store counts for each event
    struct perf_counts_values count_0 =  { .val = 0 };
    struct perf_counts_values count_1 =  { .val = 0 };
    struct perf_counts_values count_2 =  { .val = 0 };
    struct perf_counts_values count_3 =  { .val = 0 };
    struct perf_counts_values count_4 =  { .val = 0 };
    struct perf_counts_values count_5 =  { .val = 0 };
    struct perf_counts_values count_6 =  { .val = 0 };
    struct perf_counts_values count_7 =  { .val = 0 };
    struct perf_counts_values count_8 =  { .val = 0 };
    struct perf_thread_map *threads;
    struct perf_evsel *evsel_0;
    struct perf_evsel *evsel_1;
    struct perf_evsel *evsel_2;
    struct perf_evsel *evsel_3;
    struct perf_evsel *evsel_4;
    struct perf_evsel *evsel_5;
    struct perf_evsel *evsel_6;
    struct perf_evsel *evsel_7;
    struct perf_evsel *evsel_8;
    struct perf_event_mmap_page *pc_0;
    struct perf_event_mmap_page *pc_1;
    struct perf_event_mmap_page *pc_2;
    struct perf_event_mmap_page *pc_3;
    struct perf_event_mmap_page *pc_4;
    struct perf_event_mmap_page *pc_5;
    struct perf_event_mmap_page *pc_6;
    struct perf_event_mmap_page *pc_7;
    struct perf_event_mmap_page *pc_8;
  //  struct perf_event_attr attr_0;
  //  struct perf_event_attr attr_1;
  //  struct perf_event_attr attr_2;
    struct perf_event_attr attr_2;
    struct perf_event_attr attr_3;
    struct perf_event_attr attr_4;
    struct perf_event_attr attr_5;
    struct perf_event_attr attr_6;
    struct perf_event_attr attr_7;
    struct perf_event_attr attr_8;
    int err_0,err_1,err_2,err_3,err_4,err_5,err_6,err_7,err_8;
    
    perf_data = (struct PerfData *)malloc(sizeof(struct PerfData));    
    

    // Initialize thread map
    threads = perf_thread_map__new_dummy();
    if (!threads) {
        // Handle error
        return -1;
    }

    perf_thread_map__set_pid(threads, 0, 0);
 
  
    //  global_evsel = malloc(sizeof(struct perf_evsel));

 //   perf_data->global_evsel_0 =malloc(sizeof(struct perf_evsel));
  //  perf_data->global_evsel_1 =malloc(sizeof(struct perf_evsel));
    struct perf_event_attr attr_0 = {
		.type	= PERF_TYPE_RAW,
		.config	= events[0],

		.config1 = 0x2,		// Request user access 
		/*#ifdef __aarch64__
		  .config1 = 0x2,		// Request user access 
#endif */ 
	};

    struct perf_event_attr attr_1 = {
		.type	= PERF_TYPE_RAW,
		.config	= events[1],

		.config1 = 0x2,		// Request user access 
		/*#ifdef __aarch64__
		  .config1 = 0x2,		// Request user access 
#endif */ 
	};


        // Create new event selector
    evsel_0 = perf_evsel__new(&attr_0);
    if (!evsel_0) {
        // Handle error
        return -1;
    }

    evsel_1 = perf_evsel__new(&attr_1);
    if (!evsel_1) {
        // Handle error
        return -1;
    }
// Open event selector
    err_0 = perf_evsel__open(evsel_0, NULL, threads);
    if (err_0) {
        // Handle error
        return -1;
    }
    err_1 = perf_evsel__open(evsel_1, NULL, threads);
    if (err_1) {
        // Handle error
        return -1;
    }
    // Memory map event selector
    err_0 = perf_evsel__mmap(evsel_0, 0);
    if (err_0) {
        // Handle error
        return -1;
    }
    err_1 = perf_evsel__mmap(evsel_1, 0);
    if (err_1) {
        // Handle error
        return -1;
    }
    // Get mapped address
    pc_0 = perf_evsel__mmap_base(evsel_0, 0, 0);
    if (!pc_0) {
        // Handle error
        return -1;
    }
    pc_1 = perf_evsel__mmap_base(evsel_1, 0, 0);
    if (!pc_1) {
        // Handle error
        return -1;
    }
    // Set global variables
    perf_data->global_evsel_0 = evsel_0;
    perf_data->global_evsel_1 = evsel_1;
    
    global_threads = threads;
    count_data.global_count_0 = count_0;
    count_data.global_count_1 = count_1;
    
    count_data.global_count_0.val = count_0.val;
    count_data.global_count_1.val = count_1.val;

if(num_events >= 3) {
    struct perf_event_attr attr_2 = {
		.type	= PERF_TYPE_RAW,
		.config	= events[2],

		.config1 = 0x2,		// Request user access 
		/*#ifdef __aarch64__
		  .config1 = 0x2,		// Request user access 
#endif */ 
	};
    evsel_2 = perf_evsel__new(&attr_2);
    if (!evsel_2) {
        // Handle error
        return -1;
    }

    err_2 = perf_evsel__open(evsel_2, NULL, threads);
    if (err_2) {
        // Handle error
        return -1;
    }
    
    err_2 = perf_evsel__mmap(evsel_2, 0);
    if (err_2) {
        // Handle error
        return -1;
    }

    pc_2 = perf_evsel__mmap_base(evsel_2, 0, 0);
    if (!pc_2) {
        // Handle error
        return -1;
    }

    perf_data->global_evsel_2 = evsel_2;
    count_data.global_count_2 = count_2;
    count_data.global_count_2.val = count_2.val;
} 
if(num_events >= 4) {
    struct perf_event_attr attr_3 = {
		.type	= PERF_TYPE_RAW,
		.config	= events[3],

		.config1 = 0x2,		// Request user access 
		/*#ifdef __aarch64__
		  .config1 = 0x2,		// Request user access 
#endif */ 
	};
    evsel_3 = perf_evsel__new(&attr_3);
    if (!evsel_3) {
        // Handle error
        return -1;
    }

    err_3 = perf_evsel__open(evsel_3, NULL, threads);
    if (err_3) {
        // Handle error
        return -1;
    }
    
    err_3 = perf_evsel__mmap(evsel_3, 0);
    if (err_3) {
        // Handle error
        return -1;
    }

    pc_3 = perf_evsel__mmap_base(evsel_3, 0, 0);
    if (!pc_3) {
        // Handle error
        return -1;
    }

    perf_data->global_evsel_3 = evsel_3;
    count_data.global_count_3 = count_3;
    count_data.global_count_3.val = count_3.val;
} 
if(num_events >= 5) {
    struct perf_event_attr attr_4 = {
		.type	= PERF_TYPE_RAW,
		.config	= events[4],

		.config1 = 0x2,		// Request user access 
		/*#ifdef __aarch64__
		  .config1 = 0x2,		// Request user access 
#endif */ 
	};
    evsel_4 = perf_evsel__new(&attr_4);
    if (!evsel_4) {
        // Handle error
        return -1;
    }

    err_4 = perf_evsel__open(evsel_4, NULL, threads);
    if (err_4) {
        // Handle error
        return -1;
    }
    
    err_4 = perf_evsel__mmap(evsel_4, 0);
    if (err_4) {
        // Handle error
        return -1;
    }

    pc_4 = perf_evsel__mmap_base(evsel_4, 0, 0);
    if (!pc_4) {
        // Handle error
        return -1;
    }

    perf_data->global_evsel_4 = evsel_4;
    count_data.global_count_4 = count_4;
    count_data.global_count_4.val = count_4.val;
} 
if(num_events >= 6) {
    struct perf_event_attr attr_5 = {
		.type	= PERF_TYPE_RAW,
		.config	= events[5],

		.config1 = 0x2,		// Request user access 
		/*#ifdef __aarch64__
		  .config1 = 0x2,		// Request user access 
#endif */ 
	};
    evsel_5 = perf_evsel__new(&attr_5);
    if (!evsel_5) {
        // Handle error
        return -1;
    }

    err_5 = perf_evsel__open(evsel_5, NULL, threads);
    if (err_5) {
        // Handle error
        return -1;
    }
    
    err_5 = perf_evsel__mmap(evsel_5, 0);
    if (err_5) {
        // Handle error
        return -1;
    }

    pc_5 = perf_evsel__mmap_base(evsel_5, 0, 0);
    if (!pc_5) {
        // Handle error
        return -1;
    }

    perf_data->global_evsel_5 = evsel_5;
    count_data.global_count_5 = count_5;
    count_data.global_count_5.val = count_5.val;
} 
if(num_events >= 7) {
    struct perf_event_attr attr_6 = {
		.type	= PERF_TYPE_RAW,
		.config	= events[6],

		.config1 = 0x2,		// Request user access 
		/*#ifdef __aarch64__
		  .config1 = 0x2,		// Request user access 
#endif */ 
	};
    evsel_6 = perf_evsel__new(&attr_6);
    if (!evsel_6) {
        // Handle error
        return -1;
    }

    err_6 = perf_evsel__open(evsel_6, NULL, threads);
    if (err_6) {
        // Handle error
        return -1;
    }
    
    err_6 = perf_evsel__mmap(evsel_6, 0);
    if (err_6) {
        // Handle error
        return -1;
    }

    pc_6 = perf_evsel__mmap_base(evsel_6, 0, 0);
    if (!pc_6) {
        // Handle error
        return -1;
    }

    perf_data->global_evsel_6 = evsel_6;
    count_data.global_count_6 = count_6;
    count_data.global_count_6.val = count_6.val;
} 
if(num_events >= 8) {
    struct perf_event_attr attr_7 = {
		.type	= PERF_TYPE_RAW,
		.config	= events[7],

		.config1 = 0x2,		// Request user access 
		/*#ifdef __aarch64__
		  .config1 = 0x2,		// Request user access 
#endif */ 
	};
    evsel_7 = perf_evsel__new(&attr_7);
    if (!evsel_7) {
        // Handle error
        return -1;
    }

    err_7 = perf_evsel__open(evsel_7, NULL, threads);
    if (err_7) {
        // Handle error
        return -1;
    }
    
    err_7 = perf_evsel__mmap(evsel_7, 0);
    if (err_7) {
        // Handle error
        return -1;
    }

    pc_7 = perf_evsel__mmap_base(evsel_7, 0, 0);
    if (!pc_7) {
        // Handle error
        return -1;
    }

    perf_data->global_evsel_7 = evsel_7;
    count_data.global_count_7 = count_7;
    count_data.global_count_7.val = count_7.val;
} 

if(num_events >= 9) {
    struct perf_event_attr attr_8 = {
		.type	= PERF_TYPE_RAW,
		.config	= events[8],

		.config1 = 0x2,		// Request user access 
		/*#ifdef __aarch64__
		  .config1 = 0x2,		// Request user access 
#endif */ 
	};
    evsel_8 = perf_evsel__new(&attr_8);
    if (!evsel_8) {
        // Handle error
        return -1;
    }

    err_8 = perf_evsel__open(evsel_8, NULL, threads);
    if (err_8) {
        // Handle error
        return -1;
    }
    
    err_8 = perf_evsel__mmap(evsel_8, 0);
    if (err_8) {
        // Handle error
        return -1;
    }

    pc_8 = perf_evsel__mmap_base(evsel_8, 0, 0);
    if (!pc_8) {
        // Handle error
        return -1;
    }

    perf_data->global_evsel_8 = evsel_8;
    count_data.global_count_8 = count_8;
    count_data.global_count_8.val = count_8.val;
} 
    
    return 0;
}



// INIT FUNCTION
int test_evsel(int argc, char **argv, int event_vals[]) {
    __T_START;
    libperf_init(libperf_print_);
    if (test_stat_user_read(event_vals) != 0) {
        // Handle error
        return -1;
    }
    __T_END;
    return tests_failed == 0 ? 0 : -1;
}

// START CYCLE
uint64_t get_start_count(struct PerfData *perf_data, struct CountData *count_data) {
 
    if (perf_data != NULL && count_data != NULL) {
    // Check if perf_data->global_evsel_0 is not NULL
    if (perf_data->global_evsel_0 != NULL) {
        // Accessing perf_data->global_evsel_0 is safe
        perf_evsel__read(perf_data->global_evsel_0, 0, 0, &count_data->global_count_0);
    } else {
        // Handle the case where perf_data->global_evsel_0 is NULL
        // This might indicate an error in your program
        // You can print an error message or take appropriate action
        printf("Error: perf_data->global_evsel_0 is NULL\n");
    }

    // Check if count_data->global_count_0 is not NULL
 //   if (count_data->global_count_0 != NULL) {
        // Accessing count_data->global_count_0 is safe
        // Perform necessary operations here
   // } else {
        // Handle the case where count_data->global_count_0 is NULL
        // This might indicate an error in your program
        // You can print an error message or take appropriate action
     //   printf("Error: count_data->global_count_0 is NULL\n");
 }

    // Perform perf_evsel__read operation to get start count for the event at the given index
    perf_evsel__read(perf_data->global_evsel_0, 0, 0, &count_data->global_count_0);
    perf_evsel__read(perf_data->global_evsel_1, 0, 0, &count_data->global_count_1);
    
    start_0=count_data->global_count_0.val;
    start_1=count_data->global_count_1.val;

    if(num_events >= 3) {
        perf_evsel__read(perf_data->global_evsel_2, 0, 0, &count_data->global_count_2);
        start_2=count_data->global_count_2.val;
    }
    if(num_events >= 4) {
        perf_evsel__read(perf_data->global_evsel_3, 0, 0, &count_data->global_count_3);
        start_3=count_data->global_count_3.val;
    }
    if(num_events >= 5) {
        perf_evsel__read(perf_data->global_evsel_4, 0, 0, &count_data->global_count_4);
        start_4=count_data->global_count_4.val;
    }
    if(num_events >= 6) {
        perf_evsel__read(perf_data->global_evsel_5, 0, 0, &count_data->global_count_5);
        start_5=count_data->global_count_5.val;
    }
    if(num_events >= 7) {
        perf_evsel__read(perf_data->global_evsel_6, 0, 0, &count_data->global_count_6);
        start_6=count_data->global_count_6.val;
    }
    if(num_events >= 8) {
        perf_evsel__read(perf_data->global_evsel_7, 0, 0, &count_data->global_count_7);
        start_7=count_data->global_count_7.val;
    }
    if(num_events >= 9) {
        perf_evsel__read(perf_data->global_evsel_8, 0, 0, &count_data->global_count_8);
        start_8=count_data->global_count_8.val;
    }
    return 0;
}

// END CYCLE 
uint64_t get_end_count(struct PerfData *perf_data, struct CountData *count_data) {
    // Perform perf_evsel__read operation to get end count for the event at the given index
    perf_evsel__read(perf_data->global_evsel_0, 0, 0, &count_data->global_count_0);
    perf_evsel__read(perf_data->global_evsel_1, 0, 0, &count_data->global_count_1);
    end_0=count_data->global_count_0.val;
    end_1=count_data->global_count_1.val;
    if(num_events >= 3) {
        perf_evsel__read(perf_data->global_evsel_2, 0, 0, &count_data->global_count_2);
        end_2=count_data->global_count_2.val;
    }
    if(num_events >= 4) {
        perf_evsel__read(perf_data->global_evsel_3, 0, 0, &count_data->global_count_3);
        end_3=count_data->global_count_3.val;
    }
    if(num_events >= 5) {
        perf_evsel__read(perf_data->global_evsel_4, 0, 0, &count_data->global_count_4);
        end_4=count_data->global_count_4.val;
    }
    if(num_events >= 6) {
        perf_evsel__read(perf_data->global_evsel_5, 0, 0, &count_data->global_count_5);
        end_5=count_data->global_count_5.val;
    }
    if(num_events >= 7) {
        perf_evsel__read(perf_data->global_evsel_6, 0, 0, &count_data->global_count_6);
        end_6=count_data->global_count_6.val;
    }
    if(num_events >= 8) {
        perf_evsel__read(perf_data->global_evsel_7, 0, 0, &count_data->global_count_7);
        end_7=count_data->global_count_7.val;
    }
    if(num_events >= 9) {
        perf_evsel__read(perf_data->global_evsel_8, 0, 0, &count_data->global_count_8);
        end_8=count_data->global_count_8.val;
    }
    return 0;
}

// SHUTDOWN API
int shutdown_resources(struct PerfData *perf_data) {

    printf("Entering shutdown_resources\n");

    if (perf_data == NULL) {
        printf("perf_data is NULL\n");
        return -1; // Return an error code if perf_data is NULL
    }

    // Debugging statements for global_evsel_0
    printf("Cleaning up global_evsel_0\n");

        if (perf_data->global_evsel_0 != NULL) {
            printf("munmap, close, and delete global_evsel_0\n");
            printf("Before munmap: global_evsel_0 = %p\n", (void*)perf_data->global_evsel_0);
            perf_evsel__munmap(perf_data->global_evsel_0);
            printf("After munmap: global_evsel_0 = %p\n", (void*)perf_data->global_evsel_0);
            printf("Before close: global_evsel_0 = %p\n", (void*)perf_data->global_evsel_0);
            perf_evsel__close(perf_data->global_evsel_0);
            printf("After close: global_evsel_0 = %p\n", (void*)perf_data->global_evsel_0);
            printf("Before delete: global_evsel_0 = %p\n", (void*)perf_data->global_evsel_0);
            perf_evsel__delete(perf_data->global_evsel_0);
            printf("After delete: global_evsel_0 = %p\n", (void*)perf_data->global_evsel_0);
            printf("Before setting perf_data->global_evsel_0 to NULL: perf_data->global_evsel_0 = %p\n", (void*)perf_data->global_evsel_0);
            perf_data->global_evsel_0 = NULL;
            printf("After NULL: global_evsel_0 = %p\n", (void*)perf_data->global_evsel_0);
        }else {
        printf("global_evsel_0 is already NULL\n");
    }
            
        if (perf_data->global_evsel_1 != NULL) {
            printf("Before munmap: global_evsel_1 = %p\n", (void*)perf_data->global_evsel_1);
            perf_evsel__munmap(perf_data->global_evsel_1);
            printf("After munmap: global_evsel_1 = %p\n", (void*)perf_data->global_evsel_1);
            printf("Before close: global_evsel_1 = %p\n", (void*)perf_data->global_evsel_1);
            perf_evsel__close(perf_data->global_evsel_1);
            printf("After close: global_evsel_1 = %p\n", (void*)perf_data->global_evsel_1);
            perf_evsel__delete(perf_data->global_evsel_1);
            printf("After delete: global_evsel_1 = %p\n", (void*)perf_data->global_evsel_1);
            perf_data->global_evsel_1 = NULL;
            printf("After perf data: global_evsel_1 = %p\n", (void*)perf_data->global_evsel_1);
        }
        if(num_events >= 3){
        if (perf_data->global_evsel_2 != NULL) {
            printf("Before munmap: global_evsel_2 = %p\n", (void*)perf_data->global_evsel_2);
            perf_evsel__munmap(perf_data->global_evsel_2);
            printf("Before munmap: global_evsel_2 = %p\n", (void*)perf_data->global_evsel_2);
            perf_evsel__close(perf_data->global_evsel_2);
            printf("Before munmap: global_evsel_2 = %p\n", (void*)perf_data->global_evsel_2);
            perf_evsel__delete(perf_data->global_evsel_2);
            perf_data->global_evsel_2 = NULL;
            printf("After perf data: global_evsel_2 = %p\n", (void*)perf_data->global_evsel_2);
        }}
        if(num_events >= 4){
        if (perf_data->global_evsel_3 != NULL) {
            perf_evsel__munmap(perf_data->global_evsel_3);
            perf_evsel__close(perf_data->global_evsel_3);
            perf_evsel__delete(perf_data->global_evsel_3);
            perf_data->global_evsel_3 = NULL;
        }}
        if(num_events >= 5){
        if (perf_data->global_evsel_4 != NULL) {
            perf_evsel__munmap(perf_data->global_evsel_4);
            perf_evsel__close(perf_data->global_evsel_4);
            perf_evsel__delete(perf_data->global_evsel_4);
            perf_data->global_evsel_4 = NULL;
        }}
        if(num_events >= 6){
        if (perf_data->global_evsel_5 != NULL) {
            perf_evsel__munmap(perf_data->global_evsel_5);
            perf_evsel__close(perf_data->global_evsel_5);
            perf_evsel__delete(perf_data->global_evsel_5);
            perf_data->global_evsel_5 = NULL;
        }}
        if(num_events >= 7){
        if (perf_data->global_evsel_6 != NULL) {
            perf_evsel__munmap(perf_data->global_evsel_6);
            perf_evsel__close(perf_data->global_evsel_6);
            perf_evsel__delete(perf_data->global_evsel_6);
            perf_data->global_evsel_6 = NULL;
        }}
        if(num_events >= 8){
        if (perf_data->global_evsel_7 != NULL) {
            perf_evsel__munmap(perf_data->global_evsel_7);
            perf_evsel__close(perf_data->global_evsel_7);
            perf_evsel__delete(perf_data->global_evsel_7);
            perf_data->global_evsel_7 = NULL;
        }}
        if(num_events >= 9){
        if (perf_data->global_evsel_8 != NULL) {
            perf_evsel__munmap(perf_data->global_evsel_8);
            perf_evsel__close(perf_data->global_evsel_8);
            perf_evsel__delete(perf_data->global_evsel_8);
            perf_data->global_evsel_8 = NULL;
        }}
    if (global_threads != NULL) {
        perf_thread_map__put(global_threads);
        global_threads = NULL;
    }

    return 0;
}

void pmuv3_bundle_init(int argc, char *argv[]){
if (argc != 2) {
        printf("Usage: %s <arg>\n", argv[0]);
        exit(1);
    }

    int arg = atoi(argv[1]);
   
    switch (arg) {
        case 0:
            num_events = sizeof(bundle0) / sizeof(bundle0[0]);
            event_values = malloc(num_events * sizeof(int));
            event_names = (char **)malloc(num_events * sizeof(char*));
            // Extract event values from the events array
            for (size_t i = 0; i < num_events; i++) {
                event_values[i] = bundle0[i].event_value;
                event_names[i] = (char *)malloc(strlen(bundle0[i].name) * sizeof(char));
                strcpy(event_names[i],bundle0[i].name);
            }
            break;

        case 1:
            num_events = sizeof(bundle1) / sizeof(bundle1[0]);
            event_values = malloc(num_events * sizeof(int));
            event_names = (char **)malloc(num_events * sizeof(char*));

            // Extract event values from the events array
            for (size_t i = 0; i < num_events; i++) {
                event_values[i] = bundle1[i].event_value;
                event_names[i] = (char *)malloc(strlen(bundle1[i].name) * sizeof(char));
                strcpy(event_names[i],bundle1[i].name);
            }
            break;
        case 2:
            num_events = sizeof(bundle2) / sizeof(bundle2[0]);
            event_values = malloc(num_events * sizeof(int));
            event_names = (char **)malloc(num_events * sizeof(char*));

            // Extract event values from the events array
            for (size_t i = 0; i < num_events; i++) {
                event_values[i] = bundle2[i].event_value;
                event_names[i] = (char *)malloc(strlen(bundle2[i].name) * sizeof(char));
                strcpy(event_names[i],bundle2[i].name);
            }
            break;

        case 3:
            num_events = sizeof(bundle3) / sizeof(bundle3[0]);
            event_values = malloc(num_events * sizeof(int));
            event_names = (char **)malloc(num_events * sizeof(char*));

            // Extract event values from the events array
            for (size_t i = 0; i < num_events; i++) {
                event_values[i] = bundle3[i].event_value;
                event_names[i] = (char *)malloc(strlen(bundle3[i].name) * sizeof(char));
                strcpy(event_names[i],bundle3[i].name);
            }
            break;

        case 4:
            num_events = sizeof(bundle4) / sizeof(bundle4[0]);
            event_values = malloc(num_events * sizeof(int));
            event_names = (char **)malloc(num_events * sizeof(char*));

            // Extract event values from the events array
            for (size_t i = 0; i < num_events; i++) {
                event_values[i] = bundle4[i].event_value;
                event_names[i] = (char *)malloc(strlen(bundle4[i].name) * sizeof(char));
                strcpy(event_names[i],bundle4[i].name);
            }
            break;
        case 5:
            num_events = sizeof(bundle5) / sizeof(bundle5[0]);
            event_values = malloc(num_events * sizeof(int));
            event_names = (char **)malloc(num_events * sizeof(char*));

            // Extract event values from the events array
            for (size_t i = 0; i < num_events; i++) {
                event_values[i] = bundle5[i].event_value;
                event_names[i] = (char *)malloc(strlen(bundle5[i].name) * sizeof(char));
                strcpy(event_names[i],bundle5[i].name);
            }
            break;
        case 6:
            num_events = sizeof(bundle6) / sizeof(bundle6[0]);
            event_values = malloc(num_events * sizeof(int));
            event_names = (char **)malloc(num_events * sizeof(char*));

            // Extract event values from the events array
            for (size_t i = 0; i < num_events; i++) {
                event_values[i] = bundle6[i].event_value;
                event_names[i] = (char *)malloc(strlen(bundle6[i].name) * sizeof(char));
                strcpy(event_names[i],bundle6[i].name);
            }
            break;
        case 7:
            num_events = sizeof(bundle7) / sizeof(bundle7[0]);
            event_values = malloc(num_events * sizeof(int));
            event_names = (char **)malloc(num_events * sizeof(char*));

            // Extract event values from the events array
            for (size_t i = 0; i < num_events; i++) {
                event_values[i] = bundle7[i].event_value;
                event_names[i] = (char *)malloc(strlen(bundle7[i].name) * sizeof(char));
                strcpy(event_names[i],bundle7[i].name);
            }
            break;
         
        case 8:
            num_events = sizeof(bundle8) / sizeof(bundle8[0]);
            event_values = malloc(num_events * sizeof(int));
            event_names = (char **)malloc(num_events * sizeof(char*));

            // Extract event values from the events array
            for (size_t i = 0; i < num_events; i++) {
                event_values[i] = bundle8[i].event_value;
                event_names[i] = (char *)malloc(strlen(bundle8[i].name) * sizeof(char));
                strcpy(event_names[i],bundle8[i].name);
            }
            break;
        
        case 9:
            num_events = sizeof(bundle9) / sizeof(bundle9[0]);
            event_values = malloc(num_events * sizeof(int));
            event_names = (char **)malloc(num_events * sizeof(char*));

            // Extract event values from the events array
            for (size_t i = 0; i < num_events; i++) {
                event_values[i] = bundle9[i].event_value;
                event_names[i] = (char *)malloc(strlen(bundle9[i].name) * sizeof(char));
                strcpy(event_names[i],bundle9[i].name);
            }
            break;
        
        case 10:
            num_events = sizeof(bundle10) / sizeof(bundle10[0]);
            event_values = malloc(num_events * sizeof(int));
            event_names = (char **)malloc(num_events * sizeof(char*));

            // Extract event values from the events array
            for (size_t i = 0; i < num_events; i++) {
                event_values[i] = bundle10[i].event_value;
                event_names[i] = (char *)malloc(strlen(bundle10[i].name) * sizeof(char));
                strcpy(event_names[i],bundle10[i].name);
            }
            break;

        default:
            printf("Argument should be one of these in the interval [0,10] \n");
            exit(1);
    }
}
/*
int main(int argc, char *argv[]){
    pmuv3_bundle_init(argc, argv);
    
    __T("test evsel",!test_evsel(0, NULL, event_values));
        int k = 0;
        get_start_count(perf_data, &count_data);

        //sleep(10); // Sleep for 10 seconds
        while(k < 500000000) {
             k++;
        }
        // After sleep, retrieve end counts for each event
       // struct perf_counts_values endCounts[num_events];
       // get_end_count(global_evsel, endCounts, num_events);

        get_end_count(perf_data, &count_data);
        // Print the count values for each event
        for (size_t i = 0; i < num_events; i++) {
            if(i==0)
                printf("%s Count: %lu\n", event_names[i], end_0 - start_0);
            else if (i==1)
                    printf("%s Count: %lu\n",  event_names[i], end_1 - start_1);
            else if (i==2)
                    printf("%s Count: %lu\n",  event_names[i], end_2 - start_2);
            else if (i==3)
                    printf("%s Count: %lu\n",  event_names[i], end_3 - start_3);
            else if (i==4)
                    printf("%s Count: %lu\n",  event_names[i], end_4 - start_4);
            else if (i==5)
                    printf("%s Count: %lu\n",  event_names[i], end_5 - start_5);
            else if (i==6)
                    printf("%s Count: %lu\n",  event_names[i], end_6 - start_6);
            else if (i==7)
                printf("%s Count: %lu\n",  event_names[i], end_7 - start_7);
            else
                printf("%s Count: %lu\n",  event_names[i], end_8 - start_8);
        }

        // Shutdown resources
        shutdown_resources(perf_data);

}
*/

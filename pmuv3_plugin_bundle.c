/*
 * Performance Monitoring accessing PMUV3 counters
 * Author: Gayathri  Narayana Yegna Narayanan (gayathrinarayana.yegnanarayanan@arm.com)
 * Description: This plugin initializes performance monitoring for specific hardware events, reads cycle counts and cleans up the resources after that. 
 */

#include "pmuv3_plugin_bundle.h"

int tests_failed;
int tests_verbose;
struct PerfData *perf_data;
struct CountData count_data;
struct perf_thread_map *global_threads;
int num_bundles;
uint64_t num_events; 
int *event_values = NULL;
char **event_names=NULL;

uint64_t start_0,start_1,start_2,start_3,start_4,start_5,start_6,start_7;
uint64_t end_0,end_1,end_2,end_3,end_4,end_5,end_6,end_7;

struct PMUv3_Bundle_Data event_counts[10000];
uint64_t global_index = 0;
uint64_t get_next_index() {
    return global_index++;
}

#if 1
int libperf_print_(enum libperf_print_level level,
			 const char *fmt, va_list ap)
{
	//return 0;
	return vfprintf(stderr, fmt, ap);
}   
#endif


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
    perf_data->global_evsel[0] = evsel_0;
    perf_data->global_evsel[1] = evsel_1;
    
    global_threads = threads;
    count_data.global_count[0] = count_0;
    count_data.global_count[1] = count_1;
    
    count_data.global_count[0].val = count_0.val;
    count_data.global_count[1].val = count_1.val;

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

    perf_data->global_evsel[2] = evsel_2;
    count_data.global_count[2] = count_2;
    count_data.global_count[2].val = count_2.val;
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

    perf_data->global_evsel[3] = evsel_3;
    count_data.global_count[3] = count_3;
    count_data.global_count[3].val = count_3.val;
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

    perf_data->global_evsel[4] = evsel_4;
    count_data.global_count[4] = count_4;
    count_data.global_count[4].val = count_4.val;
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

    perf_data->global_evsel[5] = evsel_5;
    count_data.global_count[5] = count_5;
    count_data.global_count[5].val = count_5.val;
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

    perf_data->global_evsel[6] = evsel_6;
    count_data.global_count[6] = count_6;
    count_data.global_count[6].val = count_6.val;
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

    perf_data->global_evsel[7] = evsel_7;
    count_data.global_count[7] = count_7;
    count_data.global_count[7].val = count_7.val;
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

    perf_data->global_evsel[8] = evsel_8;
    count_data.global_count[8] = count_8;
    count_data.global_count[8].val = count_8.val;
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

//Instrumentation without local variable

// START CYCLE
uint64_t process_start_count(struct PerfData *perf_data, struct CountData *count_data) {                                           
        
    if (perf_data != NULL && count_data != NULL) {
    // Check if perf_data->global_evsel_0 is not NULL
    if (perf_data->global_evsel[0] != NULL) {
        // Accessing perf_data->global_evsel_0 is safe                                                                         
        perf_evsel__read(perf_data->global_evsel[0], 0, 0, &count_data->global_count[0]);                                        
    } else {
        // Handle the case where perf_data->global_evsel_0 is NULL                                                             
        // This might indicate an error in your program                                                                        
        // You can print an error message or take appropriate action                                                           
        printf("Error: perf_data->global_evsel_0 is NULL\n");                                                                  
    }   
        
 }  
                                                                                                                               
   for(uint64_t i =0; i < num_events; i++) {
        perf_evsel__read(perf_data->global_evsel[i], 0, 0, &count_data->global_count[i]);
        event_counts[0].start_cnt[i] = count_data->global_count[i].val;
}    
    return 0;
}   




// START CYCLE
//uint64_t get_start_count(struct PerfData *perf_data, struct CountData *count_data) {
uint64_t get_start_count(struct PerfData *perf_data, struct CountData *count_data, const char* context, uint64_t index) { 
    if (perf_data != NULL && count_data != NULL) {
    // Check if perf_data->global_evsel_0 is not NULL
    if (perf_data->global_evsel[0] != NULL) {
        // Accessing perf_data->global_evsel_0 is safe
        perf_evsel__read(perf_data->global_evsel[0], 0, 0, &count_data->global_count[0]);
    } else {
        // Handle the case where perf_data->global_evsel_0 is NULL
        // This might indicate an error in your program
        // You can print an error message or take appropriate action
        printf("Error: perf_data->global_evsel_0 is NULL\n");
    }

 }
    event_counts[index].context = context;
    for(uint64_t i =0; i < num_events; i++) {
        perf_evsel__read(perf_data->global_evsel[i], 0, 0, &count_data->global_count[i]);
        event_counts[index].start_cnt[i] = count_data->global_count[i].val;
    }
    

    return 0;
}

//Instrumenting w/o local variable
// END CYCLE 
uint64_t process_end_count(struct PerfData *perf_data, struct CountData *count_data) {
    // Perform perf_evsel__read operation to get end count for the event at the given index
    for(uint64_t i =0; i < num_events; i++) {
        perf_evsel__read(perf_data->global_evsel[i], 0, 0, &count_data->global_count[i]);
        event_counts[0].end_cnt[i] = count_data->global_count[i].val;
    }
    
    return 0;
}

//Instrumenting with local variable in multiple functions.
uint64_t get_end_count(struct PerfData *perf_data, struct CountData *count_data, const char* context, uint64_t index) {

    for(uint64_t i =0; i < num_events; i++) {
        perf_evsel__read(perf_data->global_evsel[i], 0, 0, &count_data->global_count[i]);
        event_counts[index].end_cnt[i] = count_data->global_count[i].val;
    }
    //array_index--;
    event_counts[index].context = context;
    //end_index++;
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
    for(uint64_t i =0; i < num_events; i++){
        if (perf_data->global_evsel[i] != NULL) {
            perf_evsel__munmap(perf_data->global_evsel[i]);
            perf_evsel__close(perf_data->global_evsel[i]);
            perf_evsel__delete(perf_data->global_evsel[i]);
            perf_data->global_evsel[i] = NULL;
        }
    }
    if (global_threads != NULL) {
        perf_thread_map__put(global_threads);
        global_threads = NULL;
    }
} 

int pmuv3_bundle_init(int num_bundles){
    if(num_bundles < 0 || num_bundles >= TOTAL_BUNDLE_NUM){
        printf("Error: Invalid bundle number %d\n", num_bundles);
        exit(1);
    } 
    switch (num_bundles) {
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

        case 11:
            num_events = sizeof(bundle11) / sizeof(bundle11[0]);
            event_values = malloc(num_events * sizeof(int));
            event_names = (char **)malloc(num_events * sizeof(char*));

            // Extract event values from the events array
            for (size_t i = 0; i < num_events; i++) {
                event_values[i] = bundle11[i].event_value;
                event_names[i] = (char *)malloc(strlen(bundle11[i].name) * sizeof(char));
                strcpy(event_names[i],bundle11[i].name);
            }
            break;

        case 12:
            num_events = sizeof(bundle12) / sizeof(bundle12[0]);
            event_values = malloc(num_events * sizeof(int));
            event_names = (char **)malloc(num_events * sizeof(char*));

            // Extract event values from the events array
            for (size_t i = 0; i < num_events; i++) {
                event_values[i] = bundle12[i].event_value;
                event_names[i] = (char *)malloc(strlen(bundle12[i].name) * sizeof(char));
                strcpy(event_names[i],bundle12[i].name);
            }
            break;

        case 13:
            num_events = sizeof(bundle13) / sizeof(bundle13[0]);
            event_values = malloc(num_events * sizeof(int));
            event_names = (char **)malloc(num_events * sizeof(char*));

            // Extract event values from the events array
            for (size_t i = 0; i < num_events; i++) {
                event_values[i] = bundle13[i].event_value;
                event_names[i] = (char *)malloc(strlen(bundle13[i].name) * sizeof(char));
                strcpy(event_names[i],bundle13[i].name);
            }
            break;

        case 14:
            num_events = sizeof(bundle14) / sizeof(bundle14[0]);
            event_values = malloc(num_events * sizeof(int));
            event_names = (char **)malloc(num_events * sizeof(char*));

            // Extract event values from the events array
            for (size_t i = 0; i < num_events; i++) {
                event_values[i] = bundle14[i].event_value;
                event_names[i] = (char *)malloc(strlen(bundle14[i].name) * sizeof(char));
                strcpy(event_names[i],bundle14[i].name);
            }
            break;
        default:
            printf("Argument should be one of these in the interval [0,14] \n");
            exit(1);
    }
    __T("test evsel",!test_evsel(0, NULL, event_values));
}
/*
int main(int argc, char *argv[]){
    num_bundles = atoi(argv[1]);
    pmuv3_bundle_init(num_bundles);
    
   // __T("test evsel",!test_evsel(0, NULL, event_values));
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

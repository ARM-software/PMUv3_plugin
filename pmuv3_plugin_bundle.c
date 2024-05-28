/*
 * Performance Monitoring accessing PMUV3 counters
 * Author: Gayathri  Narayana Yegna Narayanan (gayathrinarayana.yegnanarayanan@arm.com)
 * Description: This plugin initializes performance monitoring for specific hardware events grouped into 15 bundles, reads cycle counts and cleans up the resources after that. 
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

bundles bundle0[] = {
    {"CPU_CYCLES", 0x11},
    {"L1D_TLB_REFILL", 0x05},
    {"L1D_TLB", 0x25},
    {"L2D_TLB_REFILL", 0x2D},
    {"L2D_TLB", 0x2F},
    {"DTLB_WALK", 0x34}
};

bundles bundle1[] = {
    {"CPU_CYCLES", 0x11},
    {"L2D_TLB_REFILL_RD", 0x5C},
    {"L2D_TLB_REFILL_WR", 0x5D},
    {"L2D_TLB_RD", 0x5E},
    {"L2D_TLB_WR", 0x5F}
};

bundles bundle2[] = {
    {"CPU_CYCLES", 0x11},
    {"MEM_ACCESS", 0x13},
    {"BUS_ACCESS", 0x19},
    {"MEMORY_ERROR", 0x1A}
};

bundles bundle3[] = {
    {"CPU_CYCLES", 0x11},
    {"BR_MIS_PRED", 0x10},
    {"BR_PRED", 0x12},
    {"BR_RETIRED", 0x21},
    {"BR_MIS_PRED_RETIRED", 0x22},
    {"BR_IMMED_SPEC", 0x78},
    //{"BR_RETURN_SPEC", 0x79},
    {"BR_INDIRECT_SPEC", 0x7A}
};

bundles bundle4[] = {
    {"CPU_CYCLES", 0x11},
    {"STALL_FRONTEND", 0x23},
    {"STALL_BACKEND", 0x24}
};

bundles bundle5[] = {
    {"CPU_CYCLES", 0x11},
    {"L1I_CACHE_REFILL", 0x01},
    {"L1I_CACHE", 0x14}
};

bundles bundle6[] = {
    {"CPU_CYCLES", 0x11},
    {"L1D_CACHE_REFILL", 0x03},
    {"L1D_CACHE", 0x04},
    {"L2D_CACHE", 0x16},
    {"L2D_CACHE_REFILL", 0x17},
    {"L3D_CACHE_REFILL", 0x2A},
    {"L3D_CACHE", 0x2B},
};

bundles bundle7[] = {
    {"CPU_CYCLES", 0x11},
    {"L1I_TLB_REFILL", 0x02},
    {"L1I_TLB", 0x26},
    {"ITLB_WALK", 0x35}
};

bundles bundle8[] = {
    {"CPU_CYCLES", 0x11},
    {"INST_RETIRED", 0x08},
    {"INST_SPEC", 0x1B},
    {"EXC_TAKEN", 0x09},
    {"ST_SPEC", 0x71},
    {"ASE_SPEC", 0x74},
    {"PC_WRITE_SPEC", 0x76}
};

bundles bundle9[] = {
    {"CPU_CYCLES", 0x11},
    {"BR_RETURN_SPEC", 0x79},
    {"BR_IMMED_SPEC", 0x78},
    {"BR_INDIRECT_SPEC", 0x7A},
    {"INST_SPEC", 0x1B},
    {"LD_SPEC", 0x70},
    {"DSB_SPEC", 0x7D},
};

bundles bundle10[] = {
    {"CPU_CYCLES", 0x11},
    {"L1D_TLB_REFILL_RD", 0x4C},
    {"L1D_TLB_REFILL_WR", 0x4D},
    {"L1D_TLB_RD", 0x4E},
    {"L1D_TLB_WR", 0x4F}
};

bundles bundle11[] = {
    {"CPU_CYCLES", 0x11},
    {"INST_RETIRED", 0x08},
    {"LL_CACHE_MISS_RD", 0x37},
    {"L1D_CACHE_REFILL", 0x03},
    {"ITLB_WALK", 0x35},
    {"L1I_CACHE_REFILL", 0x01},
};

bundles bundle12[] = {
    {"CPU_CYCLES", 0x11},
    {"INST_RETIRED", 0x08},
    {"DTLB_WALK", 0x34},
    {"BR_MIS_PRED_RETIRED", 0x22},
    {"L2D_CACHE_REFILL", 0x17}
};

bundles bundle13[] = {
    {"CPU_CYCLES", 0x11},
    {"L1D_CACHE_REFILL_OUTER", 0x45},
    {"L1D_CACHE_REFILL", 0x03},
    {"L1D_CACHE_REFILL_RD", 0x42},
    {"L1D_CACHE_RD", 0x40},
    {"L1D_CACHE_REFILL_WR", 0x43},
    {"L1D_CACHE_WR", 0x41}
};

bundles bundle14[] = {
    {"CPU_CYCLES", 0x11},
    {"CRYPTO_SPEC", 0x77},
    {"ISB_SPEC", 0x7C},
    {"DP_SPEC", 0x73},
    {"DMB_SPEC", 0x7E},
    {"VFP_SPEC", 0x75},
    {"INST_SPEC", 0x1B}
};
struct PMUv3_Bundle_Data event_counts[10000];
uint64_t global_index = 0;
uint64_t get_next_index(void) {
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
    struct perf_counts_values counts[MAX_EVENTS] = {0};
    struct perf_thread_map *threads;
    struct perf_evsel *evsels[MAX_EVENTS];
    struct perf_event_mmap_page *pcs[MAX_EVENTS];
    struct perf_event_attr attr;
    int errs[MAX_EVENTS];

    perf_data = (struct PerfData *)malloc(sizeof(struct PerfData));


    // Initialize thread map
    threads = perf_thread_map__new_dummy();
    if (!threads) {
        // Handle error
        return -1;
    }

    perf_thread_map__set_pid(threads, 0, 0);
    global_threads = threads;

    // Loop through events and initialize attributes, evsels, and counts
    for (int i = 0; i < num_events; ++i) {
        struct perf_event_attr attr = {
            .type       = PERF_TYPE_RAW,
            .config     = events[i],
            .config1    = 0x2 // Request user access
        };

        evsels[i] = perf_evsel__new(&attr);
        if (!evsels[i]) {
            // Handle error
            return -1;
        }

        errs[i] = perf_evsel__open(evsels[i], NULL, threads);
        if (errs[i]) {
            // Handle error
            return -1;
        }

        errs[i] = perf_evsel__mmap(evsels[i], 0);
        if (errs[i]) {
            // Handle error
            return -1;
        }

        pcs[i] = perf_evsel__mmap_base(evsels[i], 0, 0);
        if (!pcs[i]) {
            // Handle error
            return -1;
        }

        perf_data->global_evsel[i] = evsels[i];
        count_data.global_count[i] = counts[i];
        count_data.global_count[i].val = counts[i].val;
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
uint64_t process_start_count(struct CountData *count_data) {
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
uint64_t get_start_count(struct CountData *count_data, const char* context, uint64_t index) { 
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
uint64_t process_end_count(struct CountData *count_data) {
    // Perform perf_evsel__read operation to get end count for the event at the given index
    for(uint64_t i =0; i < num_events; i++) {
        perf_evsel__read(perf_data->global_evsel[i], 0, 0, &count_data->global_count[i]);
        event_counts[0].end_cnt[i] = count_data->global_count[i].val;
    }
    return 0;
}

//Instrumenting with local variable in multiple functions.
uint64_t get_end_count(struct CountData *count_data, const char* context, uint64_t index) {

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
int shutdown_resources() {
    //printf("Entering shutdown_resources\n");
    if (perf_data == NULL) {
        printf("perf_data is NULL\n");
        return -1; // Return an error code if perf_data is NULL
    }
    // Debugging statements for global_evsel_0
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
    free_bundle_memory();
}
// Function to initialize a bundle
void init_bundle(bundles* bundle) {
    event_values = malloc(num_events * sizeof(int));
    event_names = (char**)malloc(num_events * sizeof(char*));

    for (size_t i = 0; i < num_events; i++) {
        event_values[i] = bundle[i].event_value;
        event_names[i] = (char*)malloc((strlen(bundle[i].name) + 1) * sizeof(char));
        strcpy(event_names[i], bundle[i].name);
    }
}

// Function to free allocated memory
void free_bundle_memory() {
    if (event_names != NULL) {
        for (size_t i = 0; i < num_events; i++) {
            free(event_names[i]);
        }
        free(event_names);
        event_names = NULL;
    }
    if (event_values != NULL) {
        free(event_values);
        event_values = NULL;
    }
}

int pmuv3_bundle_init(int num_bundles) {
    if (num_bundles < 0 || num_bundles >= TOTAL_BUNDLE_NUM) {
        printf("Error: Invalid bundle number %d\n", num_bundles);
        exit(1);
    }

    switch (num_bundles) {
        case 0: num_events = sizeof(bundle0) / sizeof(bundle0[0]); init_bundle(bundle0); break;
        case 1: num_events = sizeof(bundle1) / sizeof(bundle1[0]);init_bundle(bundle1); break;
        case 2: num_events = sizeof(bundle2) / sizeof(bundle2[0]);init_bundle(bundle2); break;
        case 3: num_events = sizeof(bundle3) / sizeof(bundle3[0]);init_bundle(bundle3); break;
        case 4: num_events = sizeof(bundle4) / sizeof(bundle4[0]);init_bundle(bundle4); break;
        case 5: num_events = sizeof(bundle5) / sizeof(bundle5[0]);init_bundle(bundle5); break;
        case 6: num_events = sizeof(bundle6) / sizeof(bundle6[0]);init_bundle(bundle6); break;
        case 7: num_events = sizeof(bundle7) / sizeof(bundle7[0]);init_bundle(bundle7); break;
        case 8: num_events = sizeof(bundle8) / sizeof(bundle8[0]);init_bundle(bundle8); break;
        case 9: num_events = sizeof(bundle9) / sizeof(bundle9[0]);init_bundle(bundle9); break;
        case 10: num_events = sizeof(bundle10) / sizeof(bundle10[0]);init_bundle(bundle10); break;
        case 11: num_events = sizeof(bundle11) / sizeof(bundle11[0]);init_bundle(bundle11); break;
        case 12: num_events = sizeof(bundle12) / sizeof(bundle12[0]);init_bundle(bundle12); break;
        case 13: num_events = sizeof(bundle13) / sizeof(bundle13[0]);init_bundle(bundle13); break;
        case 14: num_events = sizeof(bundle14) / sizeof(bundle14[0]);init_bundle(bundle14); break;
        default:
            printf("Argument should be one of these in the interval [0,14] \n");
            exit(1);
    }

    __T("test evsel", !test_evsel(0, NULL, event_values));
}

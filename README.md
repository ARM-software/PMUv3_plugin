About PMUv3 Plugins

There is a way to measure profiling of the target application as a whole box, but sometime we need to add the insturumentation into the code itself to get more fine-grained and precise measurement of the funtions handling a specific task we are interested to know its performance. For that intention, we developed the PMUv3 Plugin to allow the users to do so.

We have supported the simplest way of measuring CPU Cycle counts as well as measuring many diffent bundles of events in one shot (like Cache misses et al along with CPU cycles)

Requirements:
-------------

We require libperf.a and libapi.a from Linux standard kernel as following 
Recommendation: Please compile and replace it with your compiled liperf.a and libapi.a as it is platform dependent. 

STEP 1: Clone this linux source repository.
        git clone git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git

STEP 2: Go to /linux/tools/lib/perf 
        make 
        
STEP 3: Go to /linux/perf/tools/lib/api
        make 

STEP 4: Copy /linux/tools/lib/perf/libperf.a and /linux/perf/tools/lib/api/libapi.a to the PMUv3_plugin directory 

Compile PMUv3 Plugin Modules:
-----------------------------

We assume the directory structure like:

        /ut_integration
           /linux (git clone from Linux kernel)
           /PMUv3_plugin 
	   /YourApplication

To Generate Object file
        gcc -c pmuv3_plugin_bundle.c -I/home/ubuntu/ut_integration/linux/tools/lib/perf/include -o pmuv3_plugin_bundle.o
        gcc -c processing.cpp -I/home/ubuntu/ut_integration/linux/tools/lib/perf/include -o processing.o
        gcc —c pmuv3_plugin.c -I/home/ubuntu/ut_integration/linux/tools/lib/perf/include -o pmuv3_plugin.o

To Generate static libraries (libpmuv3_plugin.a for CPU CYCLE and libpmuv3_plugin_bundle.a for event bundles)
        ar rcs libpmuv3_plugin_bundle.a pmuv3_plugin_bundle.o processing.o
	ar rcs libpmuv3_plugin.a pmuv3_plugin.o

We provide a script build.sh to run above in one command

How to Use the APIs:
--------------------

For PMUV3_CPU_CYCLES:
---------------------

In your application's Makefile or CMakeLists files, make sure to add the includ of this -lpmuv3_plugin.a static library with -L to point its location.

In your application source code where the PMUv3 instrument will be embedded, you need to include header this way. 

#ifdef PMUV3_CPU_CYCLES
extern "C" {
    #ifdef PARENT_DIR
        #include "pmuv3_plugin.h"
    #endif
}
#endif

1) Initialize the PMUv3 Cycle instrument:
void pmuv3_cycle_init()

2) Start Cycle Instrument:
uint64_t get_start_count(struct perf_evsel *global_evsel, struct perf_counts_values *counts);

3) End Cycle Instrument:
uint64_t get_end_count(struct perf_evsel *global_evsel, struct perf_counts_values *counts);

4) Shutdown Cycle Instrument
int shutdown_resources();

For PMUV3_CPU_BUNDLES:
---------------------

In your application's Makefile or CMakeLists files, make sure to add the includ of this -lpmuv3_plugin_bundle.a static library with -L to point its location

In your application source code where the PMUv3 instrument will be embedded, you need to include header this way. 

#include "processing.hpp" 

#ifdef PMUV3_CPU_BUNDLES
extern "C" {
    #ifdef PARENT_DIR
        #include "pmuv3_plugin_bundle.h"
    #endif
}
#endif

Initialize the PMUv3 Event Bundle
1) void pmuv3_bundle_init(int)

In testcases, in main function, we need to pass the arugment for which bundle to choose:
int main(int argc, char** argv)
{
    if (argc != 2) {
        printf("Usage: %s <arg>\n", argv[0]);
        exit(1);
    }

    int cur_bundles_no = atoi(argv[1]);

    // then call initialization once:
    pmuv3_bundle_init(cur_bundles_no);
}

2) Instrument for Getting different measurement of code

local_index is a unique variable specific to every piece of instrumentation. It will be used to map the end_count to corresponding start_count and helps in post processing to calculate the cycle difference.
The get_next_index() API will help to increment the local_index by 1 at every call.

uint64_t local_index = get_next_index(); 

NOTE : This local_index variable that you define should be unique everytime. You call this before calling the get_start_count() API and every single time give unique variable name like local1, local2, local3 etc instead of using local_index everytime. This uniqueness will be useful when there are multiple functions of the same level within a function. Eg: When f2(), f3() are present within f1() and f2(), f3() are of same level, not nested. 

3) Start Event Bundle
uint64_t get_start_count(struct PerfData *perf_data, struct CountData *count_data, const char* context, uint64_t index);

For example:
  get_start_count(perf_data, &count_data, "DU_HIGH1", local_index); 

NOTE: The third variable is a context. NOTE: Whatever context (3rd parameter) and index (4th parameter) one passes in get_start_count() should be passed to corresponding get_end_count() 

4) End Event Bundle
uint64_t get_end_count(struct PerfData *perf_data, struct CountData *count_data, const char* context, uint64_t index);

For example:
   get_end_count(perf_data, &count_data, "DU_HIGH1", local_index);

# TODO :  We need to include its definition in the header!
5) Define this in a place after all instrumentation is done. 
process_data(); 

6) Shutdown and release resource for Event Bundle Instrument
int shutdown_resources(struct PerfData *perf_data);




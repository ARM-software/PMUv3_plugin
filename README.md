NOTE TO THE READERS: 

These were created and compiled on Target (ARM Neoverse CPUs) 
You can use the compiled binaries and static libraries .a files if you are running on the same target (Neoverse CPU)
Else, please follow the below steps and replace the .a and .o files with your own binaries after you clone.  

**********************************************************************************************************************************************************************************************
PMUV3_PLUGIN_BUNDLE STEPS

These are the compilation steps for using pmuv3_plugin (pmuv3_plugin_bundle.c, pmuv3_plugin_bundle.h, pmuv3_plugin_helper_bundle.h) 

To Generate Object file (Assuming linux is within ut_integration) 
        gcc -c pmuv3_plugin_bundle.c -I/home/ubuntu/ut_integration/linux/tools/lib/perf/include -o pmuv3_plugin_bundle.o

To Generate static library

        ar rcs libpmuv3_plugin_bundle.a pmuv3_plugin_bundle.o

**********************************************************************************************************************************************************************************************
5G TESTCASE / UT MODIFICATIONS

Once this is generated, in the 5G workloads (srsRAN, Radisys, etc.) include the  libpmuv3_plugin_bundle.a static library in Makefile or CMakelists.txt.

In every testcase file, on the top, you need to include header this way. 

extern "C" {
#ifdef PARENT_DIR
#include "pmuv3_plugin_bundle.h"
#endif
}

Also, in every testcase file, add the 4 APIs as follows. NOTE: Remember to comment out the API calls that were used in the PMUV3 non-bundle version. 

In testcases, in main function, we need to make below changes
int main(int argc, char** argv)
{
 if (argc != 2) {
    printf("Usage: %s <arg>\n", argv[0]);
    exit(1);
 }

 num_bundles = atoi(argv[1]);
 }

pmuv3_bundle_init(num_bundles);

get_start_count(perf_data, &count_data);

///////////CODE CHUNK OF INTEREST. EXAMPLE FROM LDPC SRSRAN - encoder->encode(codeblock, data, cfg_enc);////////////////////

get_end_count(perf_data, &count_data);

shutdown_resources(perf_data);

**********************************************************************************************************************************************************************************************
PMUV3 PLUGIN (WITHOUT BUNDLE)
these are the compilation steps for using pmuv3_plugin (pmuv3_plugin.c, pmuv3_plugin.h, pmuv3_plugin_helper.h) 

To Generate Object file

        gcc —c pmuv3_plugin.c -I/home/ubuntu/ut_integration/linux/tools/lib/perf/include -o pmuv3_plugin.o

To Generate static library

	ar rcs libpmuv3_plugin.a pmuv3_plugin.o

How libperf.a and libapi.a were compiled? 
Recommendation: Please compile and replace it with your compiled liperf.a and libapi.a as it is platform dependent. 

STEP 1: Clone this linux source repository.

        git clone git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git

STEP 2: Go to /linux/tools/lib/perf 
        
        make 
        
STEP 3: Go to /linux/perf/tools/lib/api
        
        make 

STEP 4: Copy /linux/tools/lib/perf/libperf.a and /linux/perf/tools/lib/api/libapi.a to the PMUv3_plugin directory 



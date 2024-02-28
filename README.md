NOTE TO THE READERS: 

These were created and compiled on Target (ARM Neoverse CPUs) 
You can use the compiled binaries and static libraries .a files if you are running on the same target (Neoverse CPU)
Else, please follow the below steps and replace the .a and .o files with your own binaries after you clone.  

these are the compilation steps 

To Generate Object file

        gcc —c pmuv3_plugin.c -I/home/ubuntu/linux/tools/lib/perf/include -o pmuv3_plugin.o

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



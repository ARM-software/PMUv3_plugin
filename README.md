You can find the detailed documentation [here](Documentation-PMUV3_plugin.pdf).

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
FIRST STEP BEFORE YOU BEGIN INSTRUMENTING:

To do the static library compilation, run ./build.sh from /home/ubuntu/ut_integration/PMUv3_plugin/ directory. 

We provide a script build.sh to run above in one command

So just run ./build.sh if you are going to instrument around a C++ codebase. If it is a C codebase, then comment line 19 of build.sh and uncomment line 20 and run ./build.sh

EXPLANATION OF BUILD.SH (READ BELOW FOR UNDERSTANDING, NO NEED TO EXECUTE, build.sh will do the same.)

To Generate Object file
        gcc -c pmuv3_plugin_bundle.c -I/home/ubuntu/ut_integration/linux/tools/lib/perf/include -o pmuv3_plugin_bundle.o
        gcc -c processing.cpp -I/home/ubuntu/ut_integration/linux/tools/lib/perf/include -o processing.o
        gcc —c pmuv3_plugin.c -I/home/ubuntu/ut_integration/linux/tools/lib/perf/include -o pmuv3_plugin.o

To Generate static libraries (libpmuv3_plugin.a for CPU CYCLE and libpmuv3_plugin_bundle.a for event bundles)
        ar rcs libpmuv3_plugin_bundle.a pmuv3_plugin_bundle.o processing.o
	ar rcs libpmuv3_plugin.a pmuv3_plugin.o





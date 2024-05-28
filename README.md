You can find the detailed usage information and features of the PMUv3_plugin documentation [here](Documentation-PMUV3_plugin.pdf).

**OVERVIEW**

- There is a way to measure profiling of the target application as a whole box, but sometimes we need to add the instrumentation into the code itself to get more fine-grained and precise measurement of the functions handling a specific task we are interested to know its performance. 
- For that intention, we developed the PMUv3 Plugin to allow the users to do so. This performance monitoring plugin helps to do performance analysis based on the Hardware events available in PMUV3. 
- To access the user space registers directly, we employ the mmap() system call on the perf event file descriptor. This action prompts the kernel to enable user space access and furnish us with a handle to read the raw counter registers. We have supported the simplest way of measuring CPU Cycle counts as well as measuring many different bundles of events in one shot (like Cache misses et al along with CPU cycles). The PMU events have been grouped into 15 meaningful groups/bundles and KPIs like Rate per instructions, MPKI, Stall rate, Cache Miss Rate, IPC etc. can be calculated. 
- The PMUv3_plugin not only records values of raw counter registers but also provides support to visualize the results in a CSV format by providing a post processing code. The PMUv3 source file is written in C language. Hence one can call the APIs within a C codebase by including the header file and in the case of a C++ codebase, one can include the headers within extern.
   




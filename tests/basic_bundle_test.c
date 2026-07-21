/*
 * MIT License
 * Copyright (c) [Year] ARM-software
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 */

#include "pmuv3_plugin_bundle.h"
#include "pmuv3_plugin_helper_bundle.h"

int main(int argc, char **argv){
  if (argc != 2) {
        printf("Usage: %s <arg>\n", argv[0]);
	exit(1);
  }
    num_bundles = atoi(argv[1]);
    //int cur_bundle_no = 8;
    pmuv3_bundle_init(num_bundles);
    
        volatile int k = 0;
        process_start_count(&count_data);
        while(k < 500000000) {
             k++;
        }
        process_end_count(&count_data);

        // Shutdown resources
        shutdown_resources();
        // for(int k = 0; k < num_events; ++k) {
        uint64_t diff = event_counts[0].end_cnt[0] - event_counts[0].start_cnt[0];
        //uint64_t diff = event_counts[0].end_cnt[k] - event_counts[0].start_cnt[k];
        //printf("End is %ld, Start is %ld, diff is %ld\n", event_counts[0].end_cnt[k], event_counts[0].start_cnt[k], diff);
        printf("End is %" PRIu64 ", Start is %" PRIu64 ", CPU_CYCLES is %" PRIu64 "\n",
               event_counts[0].end_cnt[0], event_counts[0].start_cnt[0], diff);
//}
}

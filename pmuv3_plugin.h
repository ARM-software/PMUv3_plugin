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

/*
 * Performance Monitoring accessing PMUV3 counters
 * Author: Gayathri  Narayana Yegna Narayanan (gayathrinarayana.yegnanarayanan@arm.com)
 * Description: This plugin initializes performance monitoring for specific hardware events, reads cycle counts and cleans up the resources after that. 
 */

#ifndef PMUV3_H
#define PMUV3_H
#include "pmuv3_plugin_helper.h"

//INITIALIZATION API
int init_api(int argc, char **argv, int event_n);
int pmuv3_cycle_init();
//START CYCLE API
uint64_t get_start_count(struct perf_evsel *global_evsel, struct perf_counts_values *counts);

//END CYCLE API
uint64_t get_end_count(struct perf_evsel *global_evsel, struct perf_counts_values *counts);

//SHUTDOWN API
int shutdown_resources();

#endif // PMUV3_H



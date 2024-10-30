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

#ifndef PMUV3_BUNDLE_H
#define PMUV3_BUNDLE_H
#include "pmuv3_plugin_helper_bundle.h"


int pmuv3_bundle_init(int bundle_num);
void init_bundle(bundles* bundle);
void free_bundle_memory();
//INITIALIZATION API

int init_api(int argc, char **argv, int event_vals[]);

//START CYCLE API
uint64_t process_start_count(struct CountData *count_data);
uint64_t get_start_count(struct CountData *count_data, const char* context, uint64_t index);

//END CYCLE API
uint64_t process_end_count(struct CountData *count_data);
uint64_t get_end_count(struct CountData *count_data, const char* context, uint64_t index);

//SHUTDOWN API
int shutdown_resources();
//uint64_t get_next_index();
uint64_t get_next_index(void);

#endif // PMUV3_BUNDLE_H

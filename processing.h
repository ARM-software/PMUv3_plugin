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

#ifndef PROCESSING_H
#define PROCESSING_H

void pushback(uint64_t **arr, uint64_t *arr_size, uint64_t value);
FILE* open_csv_file(int bundle_num);
void process_data(int bundle_num);
void post_process(int bundle_num);
void write_column_names_to_csv(int bundle_num, FILE* outFile);
void add_column_names_to_csv(int bundle_num, FILE* outFile);
void add_values_to_csv(int bundle_num, FILE* outFile);
void write_to_csv(int bundle_num, FILE* outFile);
void cycle_diff(int num_events);
void generate_cycle_diff(int num_events);

#endif // PROCESSING_H

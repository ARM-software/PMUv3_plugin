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

#ifndef PROCESSING_HPP
#define PROCESSING_HPP

#include <vector>
#include <stack>
// Declare functions
//extern "C" {
void generate_cycle_diff(int num_events);
void cycle_diff(int num_events);
void add_values_to_csv(int bundle_num, std::ofstream& outFile);
void write_to_csv(int bundle_num, std::ofstream& outFile);
void write_column_names_to_csv(int bundle_num, std::ofstream& outFile);
void add_column_names_to_csv(int bundle_num, std::ofstream& outFile);
//void process_data(int bundle_num);
void append_to_csv(int bundle_num, std::ofstream& outFile);
void process_data(int bundle_num);
void process_single_chunk(int bundle_num);
void post_process(int bundle_num);
// Declare variables
#ifdef __cplusplus
//extern "C" {
#endif
extern std::vector<const char*> context_arr;
extern std::vector<int64_t> cd_arr0;
extern std::vector<int64_t> cd_arr1;
extern std::vector<int64_t> cd_arr2;
extern std::vector<int64_t> cd_arr3;
extern std::vector<int64_t> cd_arr4;
extern std::vector<int64_t> cd_arr5;
extern std::vector<int64_t> cd_arr6;
extern std::vector<int64_t> cd_arr_e0;
extern std::vector<int64_t> cd_arr_e1;
extern std::vector<int64_t> cd_arr_e2;
extern std::vector<int64_t> cd_arr_e3;
extern std::vector<int64_t> cd_arr_e4;
extern std::vector<int64_t> cd_arr_e5;
extern std::vector<int64_t> cd_arr_e6;
#ifdef __cplusplus
//}
#endif

#endif // PROCESSING_HPP


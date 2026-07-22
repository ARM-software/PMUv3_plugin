/*
 * MIT License
 * Copyright (c) [Year] ARM-software
 */

#include "pmuv3_processing.hpp"

#include <array>
#include <iostream>
#include <string>

extern "C" {
#include "pmuv3_plugin_bundle.h"
}

std::vector<int64_t> cd_arr0;
std::vector<int64_t> cd_arr1;
std::vector<int64_t> cd_arr2;
std::vector<int64_t> cd_arr3;
std::vector<int64_t> cd_arr4;
std::vector<int64_t> cd_arr5;
std::vector<int64_t> cd_arr6;
std::vector<int64_t> cd_arr_e0;
std::vector<int64_t> cd_arr_e1;
std::vector<int64_t> cd_arr_e2;
std::vector<int64_t> cd_arr_e3;
std::vector<int64_t> cd_arr_e4;
std::vector<int64_t> cd_arr_e5;
std::vector<int64_t> cd_arr_e6;
std::vector<const char *> context_arr;

static std::array<std::vector<int64_t> *, MAX_EVENTS> multi_values = {
    &cd_arr0, &cd_arr1, &cd_arr2, &cd_arr3, &cd_arr4, &cd_arr5, &cd_arr6,
};
static std::array<std::vector<int64_t> *, MAX_EVENTS> single_values = {
    &cd_arr_e0, &cd_arr_e1, &cd_arr_e2, &cd_arr_e3,
    &cd_arr_e4, &cd_arr_e5, &cd_arr_e6,
};

static std::ofstream open_csv_file(int bundle_num)
{
    std::ofstream output;
    if (bundle_num < 0 || bundle_num >= TOTAL_BUNDLE_NUM) {
        std::cerr << "PMUv3: invalid bundle number " << bundle_num << '\n';
        return output;
    }
    output.open("bundle" + std::to_string(bundle_num) + ".csv");
    return output;
}

static void clear_results()
{
    for (auto *values : multi_values)
        values->clear();
    for (auto *values : single_values)
        values->clear();
    context_arr.clear();
}

static void write_header(std::ofstream &output, bool with_context)
{
    if (with_context)
        output << "CONTEXT";
    for (uint64_t i = 0; i < num_events; i++) {
        if (with_context || i > 0)
            output << ',';
        output << event_names[i];
    }
    output << '\n';
}

void add_column_names_to_csv(int bundle_num, std::ofstream &output)
{
    (void)bundle_num;
    write_header(output, false);
}

void write_column_names_to_csv(int bundle_num, std::ofstream &output)
{
    (void)bundle_num;
    write_header(output, true);
}

void append_to_csv(int bundle_num, std::ofstream &output)
{
    (void)bundle_num;
    std::size_t rows = num_events > 0 ? single_values[0]->size() : 0;
    for (std::size_t row = 0; row < rows; row++) {
        for (uint64_t event = 0; event < num_events; event++) {
            if (event > 0)
                output << ',';
            output << (*single_values[event])[row];
        }
        output << '\n';
    }
}

void add_values_to_csv(int bundle_num, std::ofstream &output)
{
    append_to_csv(bundle_num, output);
}

void write_to_csv(int bundle_num, std::ofstream &output)
{
    (void)bundle_num;
    std::size_t rows = num_events > 0 ? multi_values[0]->size() : 0;
    for (std::size_t row = 0; row < rows; row++) {
        output << (context_arr[row] != nullptr ? context_arr[row] : "unnamed_region");
        for (uint64_t event = 0; event < num_events; event++)
            output << ',' << (*multi_values[event])[row];
        output << '\n';
    }
}

void cycle_diff(int event_count)
{
    for (int event = 0; event < event_count; event++) {
        uint64_t start = event_counts[0].start_cnt[event];
        uint64_t end = event_counts[0].end_cnt[event];
        if (end < start)
            std::cerr << "PMUv3: counter wrapped for event " << event << '\n';
        single_values[event]->push_back(static_cast<int64_t>(end - start));
    }
}

void generate_cycle_diff(int event_count)
{
    for (uint64_t row = 0; row < global_index; row++) {
        context_arr.push_back(event_counts[row].context);
        for (int event = 0; event < event_count; event++) {
            uint64_t start = event_counts[row].start_cnt[event];
            uint64_t end = event_counts[row].end_cnt[event];
            if (end < start)
                std::cerr << "PMUv3: counter wrapped for row " << row
                          << " event " << event << '\n';
            multi_values[event]->push_back(static_cast<int64_t>(end - start));
        }
    }
}

void process_data(int bundle_num)
{
    generate_cycle_diff(static_cast<int>(num_events));
    std::ofstream output = open_csv_file(bundle_num);
    if (output.is_open()) {
        write_column_names_to_csv(bundle_num, output);
        write_to_csv(bundle_num, output);
    }
    clear_results();
}

void post_process(int bundle_num)
{
    cycle_diff(static_cast<int>(num_events));
    std::ofstream output = open_csv_file(bundle_num);
    if (output.is_open()) {
        add_column_names_to_csv(bundle_num, output);
        add_values_to_csv(bundle_num, output);
    }
    clear_results();
}

void process_single_chunk(int bundle_num)
{
    post_process(bundle_num);
}

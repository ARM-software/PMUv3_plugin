/*
 * MIT License
 * Copyright (c) [Year] ARM-software
 */

#include "pmuv3_plugin_bundle.h"
#include "pmuv3_processing.h"

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *multi_values[MAX_EVENTS];
static uint64_t *single_values[MAX_EVENTS];
static uint64_t multi_sizes[MAX_EVENTS];
static uint64_t single_sizes[MAX_EVENTS];
static char *contexts[PMUV3_MAX_REGIONS];
static uint64_t context_count;

void pushback(uint64_t **array, uint64_t *size, uint64_t value)
{
    uint64_t *updated = realloc(*array, (*size + 1) * sizeof(**array));
    if (updated == NULL) {
        fprintf(stderr, "PMUv3: processing allocation failed\n");
        exit(EXIT_FAILURE);
    }
    updated[(*size)++] = value;
    *array = updated;
}

static void push_context(const char *context)
{
    if (context_count >= PMUV3_MAX_REGIONS)
        return;
    contexts[context_count] = strdup(context != NULL ? context : "unnamed_region");
    if (contexts[context_count] == NULL) {
        fprintf(stderr, "PMUv3: context allocation failed\n");
        exit(EXIT_FAILURE);
    }
    context_count++;
}

static void reset_processing_data(void)
{
    for (uint64_t i = 0; i < MAX_EVENTS; i++) {
        free(multi_values[i]);
        free(single_values[i]);
        multi_values[i] = NULL;
        single_values[i] = NULL;
        multi_sizes[i] = 0;
        single_sizes[i] = 0;
    }
    for (uint64_t i = 0; i < context_count; i++) {
        free(contexts[i]);
        contexts[i] = NULL;
    }
    context_count = 0;
}

FILE *open_csv_file(int bundle_num)
{
    if (bundle_num < 0 || bundle_num >= TOTAL_BUNDLE_NUM) {
        fprintf(stderr, "PMUv3: invalid bundle number %d\n", bundle_num);
        return NULL;
    }

    char filename[32];
    snprintf(filename, sizeof(filename), "bundle%d.csv", bundle_num);
    FILE *output = fopen(filename, "w");
    if (output == NULL)
        perror("PMUv3: opening CSV");
    return output;
}

static void write_event_header(FILE *output, int with_context)
{
    if (with_context)
        fputs("CONTEXT", output);
    for (uint64_t i = 0; i < num_events; i++)
        fprintf(output, "%s%s", (with_context || i > 0) ? "," : "", event_names[i]);
    fputc('\n', output);
}

void add_column_names_to_csv(int bundle_num, FILE *output)
{
    (void)bundle_num;
    write_event_header(output, 0);
}

void write_column_names_to_csv(int bundle_num, FILE *output)
{
    (void)bundle_num;
    write_event_header(output, 1);
}

void add_values_to_csv(int bundle_num, FILE *output)
{
    (void)bundle_num;
    uint64_t rows = num_events > 0 ? single_sizes[0] : 0;
    for (uint64_t row = 0; row < rows; row++) {
        for (uint64_t event = 0; event < num_events; event++)
            fprintf(output, "%s%" PRIu64, event > 0 ? "," : "",
                    single_values[event][row]);
        fputc('\n', output);
    }
}

void write_to_csv(int bundle_num, FILE *output)
{
    (void)bundle_num;
    uint64_t rows = num_events > 0 ? multi_sizes[0] : 0;
    for (uint64_t row = 0; row < rows; row++) {
        fprintf(output, "%s", contexts[row]);
        for (uint64_t event = 0; event < num_events; event++)
            fprintf(output, ",%" PRIu64, multi_values[event][row]);
        fputc('\n', output);
    }
}

void cycle_diff(int event_count)
{
    for (int event = 0; event < event_count; event++) {
        uint64_t start = event_counts[0].start_cnt[event];
        uint64_t end = event_counts[0].end_cnt[event];
        if (end < start)
            fprintf(stderr,
                    "PMUv3: counter wrapped for event %d: end=%" PRIu64
                    " start=%" PRIu64 "\n", event, end, start);
        pushback(&single_values[event], &single_sizes[event], end - start);
    }
}

void generate_cycle_diff(int event_count)
{
    for (uint64_t row = 0; row < global_index; row++) {
        push_context(event_counts[row].context);
        for (int event = 0; event < event_count; event++) {
            uint64_t start = event_counts[row].start_cnt[event];
            uint64_t end = event_counts[row].end_cnt[event];
            if (end < start)
                fprintf(stderr,
                        "PMUv3: counter wrapped for row %" PRIu64
                        " event %d: end=%" PRIu64 " start=%" PRIu64 "\n",
                        row, event, end, start);
            pushback(&multi_values[event], &multi_sizes[event], end - start);
        }
    }
}

void process_data(int bundle_num)
{
    generate_cycle_diff((int)num_events);
    FILE *output = open_csv_file(bundle_num);
    if (output != NULL) {
        write_column_names_to_csv(bundle_num, output);
        write_to_csv(bundle_num, output);
        fclose(output);
    }
    reset_processing_data();
}

void post_process(int bundle_num)
{
    cycle_diff((int)num_events);
    FILE *output = open_csv_file(bundle_num);
    if (output != NULL) {
        add_column_names_to_csv(bundle_num, output);
        add_values_to_csv(bundle_num, output);
        fclose(output);
    }
    reset_processing_data();
}

void process_single_chunk(int bundle_num)
{
    post_process(bundle_num);
}

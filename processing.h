#ifndef PROCESSING_H
#define PROCESSING_H

void pushback(uint64_t **arr, uint64_t *arr_size, uint64_t value);
FILE* open_csv_file(int bundle_num);
void process_data(int bundle_num, uint64_t index_test);
void post_process(int bundle_num);
void write_column_names_to_csv(int bundle_num, FILE* outFile);
void add_column_names_to_csv(int bundle_num, FILE* outFile);
void add_values_to_csv(int bundle_num, FILE* outFile);
void write_to_csv(int bundle_num, FILE* outFile);
void cycle_diff(int num_events);
void generate_cycle_diff(int num_events);

#endif // PROCESSING_H

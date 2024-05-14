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
void process_data(int bundle_num, uint64_t index_test);
void process_single_chunk(int bundle_num);
void post_process(int bundle_num);
//void push_to_start_stack(uint64_t num_events); 
//}
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


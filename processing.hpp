#ifndef PROCESSING_HPP
#define PROCESSING_HPP

#include <vector>
#include <stack>
// Declare functions
//extern "C" {
void generate_cycle_diff(int num_bundles);
void write_column_names_to_csv(int bundle_num);
void push_to_start_stack(uint64_t num_events); 
//}
// Declare variables
#ifdef __cplusplus
//extern "C" {
#endif
extern std::vector<const char*> context_arr;
extern std::vector<int> cd_arr0;
extern std::vector<int> cd_arr1;
extern std::vector<int> cd_arr2;
extern std::vector<int> cd_arr3;
extern std::vector<int> cd_arr4;
extern std::vector<int> cd_arr5;
extern std::vector<int> cd_arr6;
extern std::ofstream outFile0; //global
extern std::ofstream outFile1; //global
extern std::ofstream outFile2; //global
extern std::ofstream outFile3; //global
extern std::ofstream outFile4; //global
extern std::ofstream outFile5; //global
extern std::ofstream outFile6; //global
extern std::ofstream outFile7; //global
extern std::ofstream outFile8; //global
extern std::ofstream outFile9; //global
extern std::ofstream outFile10; //global
extern std::ofstream outFile11; //global
extern std::ofstream outFile12; //global
extern std::ofstream outFile13; //global
extern std::ofstream outFile14; //global
//extern std::stack<uint64_t> start_stack;
#ifdef __cplusplus
//}
#endif
// Declare other variables if necessary

#endif // PROCESSING_HPP


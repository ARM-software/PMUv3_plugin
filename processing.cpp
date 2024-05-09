#include <getopt.h>
#include <random>
#include <iostream>
#include <cstdint>
#include <climits>
#include <chrono>
#include <vector>
#include <algorithm>
#include <fstream> 
#include <iomanip>
#include "processing.hpp"
extern "C"{
//#include "pmuv3_plugin.h"
#include "pmuv3_plugin_bundle.h"
}


std::vector<int64_t> cd_arr0;
std::vector<int64_t> cd_arr1;
std::vector<int64_t> cd_arr2;
std::vector<int64_t> cd_arr3;
std::vector<int64_t> cd_arr4;
std::vector<int64_t> cd_arr5;
std::vector<int64_t> cd_arr6;

std::vector<int64_t>cd_arr_e0;
std::vector<int64_t>cd_arr_e1;
std::vector<int64_t>cd_arr_e2;
std::vector<int64_t>cd_arr_e3;
std::vector<int64_t>cd_arr_e4;
std::vector<int64_t>cd_arr_e5;
std::vector<int64_t>cd_arr_e6;

uint64_t cycle_diff_0,cycle_diff_1,cycle_diff_2,cycle_diff_3,cycle_diff_4,cycle_diff_5,cycle_diff_6;


// Function to open the CSV file corresponding to the bundle number
std::ofstream open_csv_file(int bundle_num) {
    std::ofstream outFile;
    if (bundle_num >= 0 && bundle_num <= 14) {
        std::cout << "printing within open_csv" << std::endl;
        std::string filename = "bundle" + std::to_string(bundle_num) + ".csv";
        outFile.open(filename);

        if (!outFile.is_open()) {
            std::cerr << "Error opening CSV file: " << filename << std::endl;
        }
    } 
    else {
        std::cerr << "Invalid bundle number: " << bundle_num << std::endl;
    }
    return outFile;
}

void process_data(int bundle_num, uint64_t index_test){
    std::cout << "In process_data() bundle_num, index_test, num_events " << bundle_num << "," << index_test << "," << num_events << std::endl;
    generate_cycle_diff(num_events);
    std::ofstream outFile = open_csv_file(bundle_num);
    write_column_names_to_csv(bundle_num, outFile);
    write_to_csv(bundle_num, outFile);
    outFile.close();
}

void post_process(int bundle_num){
    //generate_cycle_diff(num_events);
    std::ofstream outFile = open_csv_file(bundle_num);
    add_column_names_to_csv(bundle_num, outFile);
    add_values_to_csv(bundle_num, outFile);
    outFile.close();
}

void add_column_names_to_csv(int bundle_num, std::ofstream& outFile){
    if(bundle_num == 0) {
        outFile << "CPU_CYCLES,L1D_TLB_REFILL,L1D_TLB,L2D_TLB_REFILL,L2D_TLB,DTLB_WALK\n";
    }
    else if(bundle_num == 1) {
        outFile << "CPU_CYCLES,L2D_TLB_REFILL_RD,L2D_TLB_REFILL_WR,L2D_TLB_RD,L2D_TLB_WR\n";
    }
    else if(bundle_num == 2) {
        outFile << "CPU_CYCLES,MEM_ACCESS,BUS_ACCESS,MEMORY_ERROR\n";
    }
    else if(bundle_num == 3) {
        outFile << "CPU_CYCLES,BR_MIS_PRED,BR_PRED,BR_RETIRED,BR_MIS_PRED_RETIRED,BR_IMMED_SPEC,BR_INDIRECT_SPEC\n";
    }
    else if(bundle_num == 4) {
        outFile << "CPU_CYCLES,STALL_FRONTEND,STALL_BACKEND\n";
    }
    else if(bundle_num == 5) {
        outFile << "CPU_CYCLES,L1I_CACHE_REFILL,L1I_CACHE\n";
    }
    else if(bundle_num == 6){
        outFile << "CPU_CYCLES,L1D_CACHE_REFILL,L1D_CACHE,L2D_CACHE,L2D_CACHE_REFILL,L3D_CACHE_REFILL,L3D_CACHE\n";
    }
    else if(bundle_num == 7){
        outFile << "CPU_CYCLES,L1I_TLB_REFILL,L1I_TLB,ITLB_WALK\n";
    }
    else if(bundle_num == 8){
        outFile << "CPU_CYCLES,INST_RETIRED,INST_SPEC,EXC_TAKEN,ST_SPEC,ASE_SPEC,PC_WRITE_SPEC\n";
    }
    else if(bundle_num == 9){
        outFile << "CPU_CYCLES,BR_RETURN_SPEC,BR_IMMED_SPEC,BR_INDIRECT_SPEC,INST_SPEC,LD_SPEC,DSB_SPEC\n";
    }
    else if(bundle_num == 10){
        outFile << "CPU_CYCLES,L1D_TLB_REFILL_RD,L1D_TLB_REFILL_WR,L1D_TLB_RD,L1D_TLB_WR\n";
    }
    else if(bundle_num == 11){
        outFile << "CPU_CYCLES,INST_RETIRED,LL_CACHE_MISS_RD,L1D_CACHE_REFILL,ITLB_WALK,L1I_CACHE_REFILL\n";
    }
    else if(bundle_num == 12){
        outFile << "CPU_CYCLES,INST_RETIRED,DTLB_WALK,BR_MIS_PRED_RETIRED,L2D_CACHE_REFILL\n";
    }
    else if(bundle_num == 13){
        outFile << "CPU_CYCLES,L1D_CACHE_REFILL_OUTER,L1D_CACHE_REFILL,L1D_CACHE_REFILL_RD,L1D_CACHE_RD,L1D_CACHE_REFILL_WR,L1D_CACHE_WR\n";
    }
    else if(bundle_num == 14){
        outFile << "CPU_CYCLES,CRYPTO_SPEC,ISB_SPEC,DP_SPEC,DMB_SPEC,VFP_SPEC,INST_SPEC\n";
    }
}
void add_values_to_csv(int bundle_num, std::ofstream& outFile){
    std::cout << "cd_arr_e0 size is " << cd_arr_e0.size() << std::endl;
    for(size_t i = 0; i< cd_arr_e0.size(); ++i){
        std::cout << "printing within for cd_arr_e0" << cd_arr_e0[i] << i << std::endl;
        if(bundle_num == 0) {
            std::cout << "printing cd_arr_e0" << cd_arr_e0[i] << i << std::endl;
            outFile << cd_arr_e0[i] << ","  << cd_arr_e1[i] << "," << cd_arr_e2[i] << ","  << cd_arr_e3[i] << "," << cd_arr_e4[i] << "," << cd_arr_e5[i] << "\n";
        }
        else if(bundle_num == 1) {
            outFile << cd_arr_e0[i] << ","  << cd_arr_e1[i] << "," << cd_arr_e2[i] << ","  << cd_arr_e3[i] << "," << cd_arr_e4[i] << "\n";
        }
        else if(bundle_num == 2) {
            outFile << cd_arr_e0[i] << ","  << cd_arr_e1[i] << "," << cd_arr_e2[i] << ","  << cd_arr_e3[i] << "\n";
        }
        else if(bundle_num == 3) {
            outFile << cd_arr_e0[i] << ","  << cd_arr_e1[i] << "," << cd_arr_e2[i] << ","  << cd_arr_e3[i] << "," << cd_arr_e4[i] << ","  << cd_arr_e5[i] << ","  << cd_arr_e6[i] << "\n";
        }
        else if(bundle_num == 4) {
            outFile << cd_arr_e0[i] << ","  << cd_arr_e1[i] << "," << cd_arr_e2[i] << "\n";
        }
        else if(bundle_num == 5) {
            outFile << cd_arr_e0[i] << ","  << cd_arr_e1[i] << "," << cd_arr_e2[i] << "\n";
        }
        else if(bundle_num == 6){
            outFile << cd_arr_e0[i] << ","  << cd_arr_e1[i] << "," << cd_arr_e2[i] << ","  << cd_arr_e3[i] << "," << cd_arr_e4[i] << ","  << cd_arr_e5[i] << ","  << cd_arr_e6[i] << "\n";
        }
        else if(bundle_num == 7){
            outFile << cd_arr_e0[i] << ","  << cd_arr_e1[i] << "," << cd_arr_e2[i] << ","  << cd_arr_e3[i] << "\n";
        }
        else if(bundle_num == 8){
            outFile << cd_arr_e0[i] << ","  << cd_arr_e1[i] << "," << cd_arr_e2[i] << ","  << cd_arr_e3[i] << "," << cd_arr_e4[i] << ","  << cd_arr_e5[i] << ","  << cd_arr_e6[i] << "\n";
        }
        else if(bundle_num == 9){
            outFile << cd_arr_e0[i] << ","  << cd_arr_e1[i] << "," << cd_arr_e2[i] << ","  << cd_arr_e3[i] << "," << cd_arr_e4[i] << ","  << cd_arr_e5[i] << ","  << cd_arr_e6[i] << "\n";
        }
        else if(bundle_num == 10){
            outFile << cd_arr_e0[i] << ","  << cd_arr_e1[i] << "," << cd_arr_e2[i] << "," << cd_arr_e3[i] << "," << cd_arr_e4[i] << "\n";
        }
        else if(bundle_num == 11){
            outFile << cd_arr_e0[i] << ","  << cd_arr_e1[i] << "," << cd_arr_e2[i] << ","  << cd_arr_e3[i]  << "," << cd_arr_e4[i] << "," << cd_arr_e5[i] << "\n";
        }
        else if(bundle_num == 12){
            outFile << cd_arr_e0[i] << ","  << cd_arr_e1[i] << "," << cd_arr_e2[i] << ","  << cd_arr_e3[i]  << "," << cd_arr_e4[i] << "\n";
        }
        else if(bundle_num == 13){
            outFile << cd_arr_e0[i] << ","  << cd_arr_e1[i] << "," << cd_arr_e2[i] << ","  << cd_arr_e3[i]  << "," << cd_arr_e4[i] << "," << cd_arr_e5[i] << "," << cd_arr_e6[i] << "\n";
        }
        else if(bundle_num == 14){
            outFile << cd_arr_e0[i] << ","  << cd_arr_e1[i] << "," << cd_arr_e2[i] << ","  << cd_arr_e3[i]  << "," << cd_arr_e4[i] << "," << cd_arr_e5[i] << "," << cd_arr_e6[i] << "\n";
        }
    }
}
void write_column_names_to_csv(int bundle_num, std::ofstream& outFile){
    if(bundle_num == 0) {
        outFile << "CONTEXT,CPU_CYCLES,L1D_TLB_REFILL,L1D_TLB,L2D_TLB_REFILL,L2D_TLB,DTLB_WALK\n";
    }
    else if(bundle_num == 1) {
        outFile << "CONTEXT,CPU_CYCLES,L2D_TLB_REFILL_RD,L2D_TLB_REFILL_WR,L2D_TLB_RD,L2D_TLB_WR\n";
    }
    else if(bundle_num == 2) {
        outFile << "CONTEXT,CPU_CYCLES,MEM_ACCESS,BUS_ACCESS,MEMORY_ERROR\n";
    }
    else if(bundle_num == 3) {
        outFile << "CONTEXT,CPU_CYCLES,BR_MIS_PRED,BR_PRED,BR_RETIRED,BR_MIS_PRED_RETIRED,BR_IMMED_SPEC,BR_INDIRECT_SPEC\n";
    }
    else if(bundle_num == 4) {
        outFile << "CONTEXT,CPU_CYCLES,STALL_FRONTEND,STALL_BACKEND\n";
    }
    else if(bundle_num == 5) {
        outFile << "CONTEXT,CPU_CYCLES,L1I_CACHE_REFILL,L1I_CACHE\n";
    }
    else if(bundle_num == 6){
        outFile << "CONTEXT,CPU_CYCLES,L1D_CACHE_REFILL,L1D_CACHE,L2D_CACHE,L2D_CACHE_REFILL,L3D_CACHE_REFILL,L3D_CACHE\n";
    }
    else if(bundle_num == 7){
        outFile << "CONTEXT,CPU_CYCLES,L1I_TLB_REFILL,L1I_TLB,ITLB_WALK\n";
    }
    else if(bundle_num == 8){
        outFile << "CONTEXT,CPU_CYCLES,INST_RETIRED,INST_SPEC,EXC_TAKEN,ST_SPEC,ASE_SPEC,PC_WRITE_SPEC\n";
    }
    else if(bundle_num == 9){
        outFile << "CONTEXT,CPU_CYCLES,BR_RETURN_SPEC,BR_IMMED_SPEC,BR_INDIRECT_SPEC,INST_SPEC,LD_SPEC,DSB_SPEC\n";
    }
    else if(bundle_num == 10){
        outFile << "CONTEXT,CPU_CYCLES,L1D_TLB_REFILL_RD,L1D_TLB_REFILL_WR,L1D_TLB_RD,L1D_TLB_WR\n";
    }
    else if(bundle_num == 11){
        outFile << "CONTEXT,CPU_CYCLES,INST_RETIRED,LL_CACHE_MISS_RD,L1D_CACHE_REFILL,ITLB_WALK,L1I_CACHE_REFILL\n";
    }
    else if(bundle_num == 12){
        outFile << "CONTEXT,CPU_CYCLES,INST_RETIRED,DTLB_WALK,BR_MIS_PRED_RETIRED,L2D_CACHE_REFILL\n";
    }
    else if(bundle_num == 13){
        outFile << "CONTEXT,CPU_CYCLES,L1D_CACHE_REFILL_OUTER,L1D_CACHE_REFILL,L1D_CACHE_REFILL_RD,L1D_CACHE_RD,L1D_CACHE_REFILL_WR,L1D_CACHE_WR\n";
    }
    else if(bundle_num == 14){
        outFile << "CONTEXT,CPU_CYCLES,CRYPTO_SPEC,ISB_SPEC,DP_SPEC,DMB_SPEC,VFP_SPEC,INST_SPEC\n";
    }
}
void write_to_csv(int bundle_num, std::ofstream& outFile){
    std::cout << "cd_arr0.size in write_to_csv " << cd_arr0.size() << std::endl;
    for(size_t i = 0; i< cd_arr0.size(); ++i){
        if(bundle_num == 0) {
            outFile << context_arr[i] << "," << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << ","  << cd_arr3[i] << "," << cd_arr4[i] << "," << cd_arr5[i] << "\n";
        }
        else if(bundle_num == 1) {
            outFile << context_arr[i] << "," << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << ","  << cd_arr3[i] << "," << cd_arr4[i] << "\n";
        }
        else if(bundle_num == 2) {
            outFile << context_arr[i] << "," << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << ","  << cd_arr3[i] << "\n";
        }
        else if(bundle_num == 3) {
            outFile << context_arr[i] << "," << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << ","  << cd_arr3[i] << "," << cd_arr4[i] << ","  << cd_arr5[i] << ","  << cd_arr6[i] << "\n";
        }
        else if(bundle_num == 4) {
            outFile << context_arr[i] << "," << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << "\n";
        }
        else if(bundle_num == 5) {
            outFile << context_arr[i] << "," << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << "\n";
        }
        else if(bundle_num == 6){
            //std::cout << "in write_to_csv bundle 6 " << context_arr[i] << "," << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << ","  << cd_arr3[i] << "," << cd_arr4[i] << ","  << cd_arr5[i] << ","  << cd_arr6[i] << std::endl;
            outFile << context_arr[i] << "," << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << ","  << cd_arr3[i] << "," << cd_arr4[i] << ","  << cd_arr5[i] << ","  << cd_arr6[i] << "\n";
        }
        else if(bundle_num == 7){
            outFile << context_arr[i] << "," << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << ","  << cd_arr3[i] << "\n";
        }
        else if(bundle_num == 8){
            outFile << context_arr[i] << "," << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << ","  << cd_arr3[i] << "," << cd_arr4[i] << ","  << cd_arr5[i] << ","  << cd_arr6[i] << "\n";
        }
        else if(bundle_num == 9){
            outFile << context_arr[i] << "," << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << ","  << cd_arr3[i] << "," << cd_arr4[i] << ","  << cd_arr5[i] << ","  << cd_arr6[i] << "\n";
        }
        else if(bundle_num == 10){
            outFile << context_arr[i] << "," << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << "," << cd_arr3[i] << "," << cd_arr4[i] << "\n";
        }
        else if(bundle_num == 11){
            outFile << context_arr[i] << "," << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << ","  << cd_arr3[i]  << "," << cd_arr4[i] << "," << cd_arr5[i] << "\n";
        }
        else if(bundle_num == 12){
            outFile << context_arr[i] << "," << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << ","  << cd_arr3[i]  << "," << cd_arr4[i] << "\n";
        }
        else if(bundle_num == 13){
            outFile << context_arr[i] << "," << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << ","  << cd_arr3[i]  << "," << cd_arr4[i] << "," << cd_arr5[i] << "," << cd_arr6[i] << "\n";
        }
        else if(bundle_num == 14){
            outFile << context_arr[i] << "," << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << ","  << cd_arr3[i]  << "," << cd_arr4[i] << "," << cd_arr5[i] << "," << cd_arr6[i] << "\n";
        }
    }
}

std::vector<const char*> context_arr;

void generate_cycle_diff(int num_events) {
    for (int i = 0; i < global_index; ++i) {
    //for (int i = 0; i < index_test; ++i) {
        context_arr.push_back(event_counts[i].context);
        for(int k = 0; k < num_events; ++k) {
            int64_t diff = event_counts[i].end_cnt[k] - event_counts[i].start_cnt[k];
            if (diff < 0){
                std::cout << "End is " << event_counts[i].end_cnt[k] << "Start is " << event_counts[i].start_cnt[k] << std::endl;
            }
            switch (k) {
                case 0:
                    cd_arr0.push_back(diff);
                    break;
                case 1:
                    cd_arr1.push_back(diff);
                    break;
                case 2:
                    cd_arr2.push_back(diff);
                    break;
                case 3:
                    cd_arr3.push_back(diff);
                    break;
                case 4:
                    cd_arr4.push_back(diff);
                    break;
                case 5:
                    cd_arr5.push_back(diff);
                    break;
                case 6:
                    cd_arr6.push_back(diff);
                    break;
                default:
                    std::cerr << "Invalid index: " << k << std::endl;
                    break;
            }
        }
    }
    //std::cout << " cd_arr SIZE is " << cd_arr0.size() << std::endl;
}

void cycle_diff(int num_events) {
    for(int k = 0; k < num_events; ++k) {
        int64_t diff = event_counts[0].end_cnt[k] - event_counts[0].start_cnt[k];
        if (diff < 0){ 
            std::cout << "End is " << event_counts[0].end_cnt[k] << "Start is " << event_counts[0].start_cnt[k] << std::endl;
        }
        switch (k) {
            case 0:
                cd_arr0.push_back(diff);
                break;
            case 1:
                cd_arr1.push_back(diff);
                break;
            case 2:
                cd_arr2.push_back(diff);
                break;
            case 3:
                cd_arr3.push_back(diff);
                break;
            case 4:
                cd_arr4.push_back(diff);
                break;
            case 5:
                cd_arr5.push_back(diff);
                break;
            case 6:
                cd_arr6.push_back(diff);
                break;
            default:
                std::cerr << "Invalid index: " << k << std::endl;
                break;
        }
    }
    //std::cout << " cd_arr SIZE is " << cd_arr0.size() << std::endl;
}


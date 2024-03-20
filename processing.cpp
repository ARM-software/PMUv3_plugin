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

//extern "C" {

//uint64_t start_0,start_1,start_2,start_3,start_4,start_5,start_6,start_7,start_8;
//uint64_t end_0,end_1,end_2,end_3,end_4,end_5,end_6,end_7,end_8;
std::stack<uint64_t> start_stack;
std::ofstream outFile0("bundle0.csv"); //global
std::ofstream outFile1("bundle1.csv"); //global
std::ofstream outFile2("bundle2.csv"); //global
std::ofstream outFile3("bundle3.csv"); //global
std::ofstream outFile4("bundle4.csv"); //global
std::ofstream outFile5("bundle5.csv"); //global
std::ofstream outFile6("bundle6.csv"); //global
std::ofstream outFile7("bundle7.csv"); //global
std::ofstream outFile8("bundle8.csv"); //global
std::ofstream outFile9("bundle9.csv"); //global
std::ofstream outFile10("bundle10.csv"); //global
std::ofstream outFile11("bundle11.csv"); //global
std::ofstream outFile12("bundle12.csv"); //global
std::ofstream outFile13("bundle13.csv"); //global
std::ofstream outFile14("bundle14.csv"); //global
std::vector<int> cd_arr0;
std::vector<int> cd_arr1;
std::vector<int> cd_arr2;
std::vector<int> cd_arr3;
std::vector<int> cd_arr4;
std::vector<int> cd_arr5;
std::vector<int> cd_arr6;
/*
std::vector<int>cd_arr_e0;
std::vector<int>cd_arr_e1;
std::vector<int>cd_arr_e2;
std::vector<int>cd_arr_e3;
std::vector<int>cd_arr_e4;
std::vector<int>cd_arr_e5;
std::vector<int>cd_arr_e6;
*/
//int lowerIteration = 0, upperIteration = 0;
uint64_t cycle_diff_0,cycle_diff_1,cycle_diff_2,cycle_diff_3,cycle_diff_4,cycle_diff_5,cycle_diff_6;

//std::ofstream outFile("percentiles_du_high.csv");


void write_column_names_to_csv(int bundle_num){
  if(bundle_num == 0) {
    outFile0 << "CONTEXT,CPU_CYCLES,L1D_TLB_REFILL,L1D_TLB,L2D_TLB_REFILL,L2D_TLB,DTLB_WALK\n";
  }
  else if(bundle_num == 1) {
    outFile1 << "CONTEXT,CPU_CYCLES,L2D_TLB_REFILL_RD,L2D_TLB_REFILL_WR,L2D_TLB_RD,L2D_TLB_WR\n";
  }
  else if(bundle_num == 2) {
    outFile2 << "CONTEXT,CPU_CYCLES,MEM_ACCESS,BUS_ACCESS,MEMORY_ERROR\n";
  }
  else if(bundle_num == 3) {
    outFile3 << "CONTEXT,CPU_CYCLES,BR_MIS_PRED,BR_PRED,BR_RETIRED,BR_MIS_PRED_RETIRED,BR_IMMED_SPEC,BR_INDIRECT_SPEC\n";
  }
  else if(bundle_num == 4) {
    outFile4 << "CONTEXT,CPU_CYCLES,STALL_FRONTEND,STALL_BACKEND\n";
  }
  else if(bundle_num == 5) {
    outFile5 << "CONTEXT,CPU_CYCLES,L1I_CACHE_REFILL,L1I_CACHE\n";
  }
  else if(bundle_num == 6){
    outFile6 << "CONTEXT,CPU_CYCLES,L1D_CACHE_REFILL,L1D_CACHE,L2D_CACHE,L2D_CACHE_REFILL,L3D_CACHE_REFILL,L3D_CACHE\n";
  }
  else if(bundle_num == 7){
    outFile7 << "CONTEXT,CPU_CYCLES,L1I_TLB_REFILL,L1I_TLB,ITLB_WALK\n";
  }
  else if(bundle_num == 8){
    outFile8 << "CONTEXT,CPU_CYCLES,INST_RETIRED,INST_SPEC,EXC_TAKEN,ST_SPEC,ASE_SPEC,PC_WRITE_SPEC\n";
  }
  else if(bundle_num == 9){
    outFile9 << "CONTEXT,CPU_CYCLES,BR_RETURN_SPEC,BR_IMMED_SPEC,BR_INDIRECT_SPEC,INST_SPEC,LD_SPEC,DSB_SPEC\n";
  }
  else if(bundle_num == 10){
    outFile10 << "CONTEXT,CPU_CYCLES,L1D_TLB_REFILL_RD,L1D_TLB_REFILL_WR,L1D_TLB_RD,L1D_TLB_WR\n";
  }
  else if(bundle_num == 11){
    outFile11 << "CONTEXT,CPU_CYCLES,INST_RETIRED,LL_CACHE_MISS_RD,L1D_CACHE_REFILL,ITLB_WALK,L1I_CACHE_REFILL\n";
  }
  else if(bundle_num == 12){
    outFile12 << "CONTEXT,CPU_CYCLES,INST_RETIRED,DTLB_WALK,BR_MIS_PRED_RETIRED,L2D_CACHE_REFILL\n";
  }
  else if(bundle_num == 13){
    outFile13 << "CONTEXT,CPU_CYCLES,L1D_CACHE_REFILL_OUTER,L1D_CACHE_REFILL,L1D_CACHE_REFILL_RD,L1D_CACHE_RD,L1D_CACHE_REFILL_WR,L1D_CACHE_WR\n";
  }
  else if(bundle_num == 14){
    outFile14 << "CONTEXT,CPU_CYCLES,CRYPTO_SPEC,ISB_SPEC,DP_SPEC,DMB_SPEC,VFP_SPEC,INST_SPEC\n";
  }
}

/*
void write_to_csv(int bundle_num){
  for(size_t i = 0; i< cd_arr0.size(); ++i){
    if(bundle_num == 0) {
      outFile0 << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << ","  << cd_arr3[i] << "," << cd_arr4[i] << "," << cd_arr5[i] << "\n";
    }
    else if(bundle_num == 1) {
      outFile1 << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << ","  << cd_arr3[i] << "," << cd_arr4[i] << "\n";
    }
    else if(bundle_num == 2) {
      outFile2 << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << ","  << cd_arr3[i] << "\n";
    }
    else if(bundle_num == 3) {
      outFile3 << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << ","  << cd_arr3[i] << "," << cd_arr4[i] << ","  << cd_arr5[i] << ","  << cd_arr6[i] << "\n";
    }
    else if(bundle_num == 4) {
      outFile4 << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << "\n";
    }
    else if(bundle_num == 5) {
      outFile5 << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << "\n";
    }
    else if(bundle_num == 6){
      outFile6 << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << ","  << cd_arr3[i] << "," << cd_arr4[i] << ","  << cd_arr5[i] << ","  << cd_arr6[i] << "\n";
    }
    else if(bundle_num == 7){
      outFile7 << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << ","  << cd_arr3[i] << "\n";
    }
    else if(bundle_num == 8){
      outFile8 << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << ","  << cd_arr3[i] << "," << cd_arr4[i] << ","  << cd_arr5[i] << ","  << cd_arr6[i] << "\n";
    }
    else if(bundle_num == 9){
      outFile9 << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << ","  << cd_arr3[i] << "," << cd_arr4[i] << ","  << cd_arr5[i] << ","  << cd_arr6[i] << "\n";
    }
    else if(bundle_num == 10){
      outFile10 << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << "," << cd_arr3[i] << "," << cd_arr4[i] << "\n";
    }
    else if(bundle_num == 11){
      outFile11 << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << ","  << cd_arr3[i]  << "," << cd_arr4[i] << "," << cd_arr5[i] << "\n";
    }
    else if(bundle_num == 12){
      outFile12 << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << ","  << cd_arr3[i]  << "," << cd_arr4[i] << "\n";
    }
    else if(bundle_num == 13){
      outFile13 << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << ","  << cd_arr3[i]  << "," << cd_arr4[i] << "," << cd_arr5[i] << "," << cd_arr6[i] << "\n";
    }
    else if(bundle_num == 14){
      outFile14 << cd_arr0[i] << ","  << cd_arr1[i] << "," << cd_arr2[i] << ","  << cd_arr3[i]  << "," << cd_arr4[i] << "," << cd_arr5[i] << "," << cd_arr6[i] << "\n";
    }
  }
  cd_arr0.clear();
  cd_arr1.clear();
  cd_arr2.clear();
  cd_arr3.clear();
  cd_arr4.clear();
  cd_arr5.clear();
  cd_arr6.clear();

  outFile0.close();
  outFile1.close();
  outFile2.close();
  outFile3.close();
  outFile4.close();
  outFile5.close();
  outFile6.close();
  outFile7.close();
  outFile8.close();
  outFile9.close();
  outFile10.close();
  outFile11.close();
  outFile12.close();
  outFile13.close();
  outFile14.close();
  
}
*/
/*
void generate_cycle_diff(int num_bundles) {
    if(num_bundles == 4 || num_bundles == 5) {
        cycle_diff_2 = end_2 - start_stack.top();
	start_stack.pop();
        cycle_diff_1 = end_1 - start_stack.top();
	start_stack.pop();
        cycle_diff_0 = end_0 - start_stack.top();
	start_stack.pop();
        cd_arr0.push_back(cycle_diff_0);
        cd_arr1.push_back(cycle_diff_1);
        cd_arr2.push_back(cycle_diff_2);
    
    }
    if(num_bundles == 2 || num_bundles == 7) {
        cycle_diff_3 = end_3 - start_stack.top();
	start_stack.pop();
        cycle_diff_2 = end_2 - start_stack.top();
	start_stack.pop();
        cycle_diff_1 = end_1 - start_stack.top();
	start_stack.pop();
        cycle_diff_0 = end_0 - start_stack.top();
	start_stack.pop();
        cd_arr0.push_back(cycle_diff_0);
        cd_arr1.push_back(cycle_diff_1);
        cd_arr2.push_back(cycle_diff_2);
        cd_arr3.push_back(cycle_diff_3);
    }
    if(num_bundles == 1 || num_bundles == 10 || num_bundles == 12) {
        cycle_diff_4 = end_4 - start_stack.top();
	start_stack.pop();
        cycle_diff_3 = end_3 - start_stack.top();
	start_stack.pop();
        cycle_diff_2 = end_2 - start_stack.top();
	start_stack.pop();
        cycle_diff_1 = end_1 - start_stack.top();
	start_stack.pop();
        cycle_diff_0 = end_0 - start_stack.top();
	start_stack.pop();
        cd_arr0.push_back(cycle_diff_0);
        cd_arr1.push_back(cycle_diff_1);
        cd_arr2.push_back(cycle_diff_2);
        cd_arr3.push_back(cycle_diff_3);
        cd_arr4.push_back(cycle_diff_4);
    }
    if(num_bundles == 0 || num_bundles == 11) {
        cycle_diff_5 = end_5 - start_stack.top();
	start_stack.pop();
        cycle_diff_4 = end_4 - start_stack.top();
	start_stack.pop();
        cycle_diff_3 = end_3 - start_stack.top();
	start_stack.pop();
        cycle_diff_2 = end_2 - start_stack.top();
	start_stack.pop();
        cycle_diff_1 = end_1 - start_stack.top();
	start_stack.pop();
        cycle_diff_0 = end_0 - start_stack.top();
	start_stack.pop();
        cd_arr0.push_back(cycle_diff_0);
        cd_arr1.push_back(cycle_diff_1);
        cd_arr2.push_back(cycle_diff_2);
        cd_arr3.push_back(cycle_diff_3);
        cd_arr4.push_back(cycle_diff_4);
        cd_arr5.push_back(cycle_diff_5);
    }
    if(num_bundles == 3 || num_bundles == 6 || num_bundles == 13 || num_bundles == 8 || num_bundles == 9 || num_bundles == 14) {
	cycle_diff_6 = end_6 - start_stack.top();
	start_stack.pop();
        cycle_diff_5 = end_5 - start_stack.top();
	start_stack.pop();
        cycle_diff_4 = end_4 - start_stack.top();
	start_stack.pop();
        cycle_diff_3 = end_3 - start_stack.top();
	start_stack.pop();
        cycle_diff_2 = end_2 - start_stack.top();
	start_stack.pop();
        cycle_diff_1 = end_1 - start_stack.top();
	start_stack.pop();
        cycle_diff_0 = end_0 - start_stack.top();
	start_stack.pop();
        cd_arr0.push_back(cycle_diff_0);
        cd_arr1.push_back(cycle_diff_1);
        cd_arr2.push_back(cycle_diff_2);
        cd_arr3.push_back(cycle_diff_3);
        cd_arr4.push_back(cycle_diff_4);
        cd_arr5.push_back(cycle_diff_5);
        cd_arr6.push_back(cycle_diff_6);
    }
}
//} 
*/
std::vector<const char*> context_arr;

void generate_cycle_diff(int num_events) {
    for (int i = 0; i < global_index; ++i) {
        context_arr.push_back(event_counts[i].context);
        for(int k = 0; k < num_events; ++k) {
            int diff = event_counts[i].end_cnt[k] - event_counts[i].start_cnt[k];
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
}
/*void generate_cycle_diff(int num_events) {

  for (int i = 0; i < start_index; ++i) {
        for (int j = 0; j < end_index; ++j) {
            // Check if the context (name) matches
            if (event_scounts[i].context == event_ecounts[j].context) {
                // Push the name to cd_name only once
                //if (context_arr.empty() || context_arr.back() != event_scounts[i].context) {
                    context_arr.push_back(event_scounts[i].context);
               // }
                // Calculate the difference of values in array a when context matches
                for (int k = 0; k < num_events; ++k) {
                    int diff = event_ecounts[j].cycle_cnt[k] - event_scounts[i].cycle_cnt[k];
                    // Push the difference into the corresponding cd_arr based on index k
                    std::cout << "Diff for " << event_scounts[i].context << "at index " << k << "is : " << diff << std::endl;
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
                break;  // Move to the next element in event_scounts
            }
        }
    }
}
*/
/*  
    
void push_to_start_stack(uint64_t num_events) { 
    start_stack.push(start_0);
    start_stack.push(start_1);

    if(num_events >= 3) {
        start_stack.push(start_2);
    }
    if(num_events >= 4) {
        start_stack.push(start_3);
    }
    if(num_events >= 5) {
        start_stack.push(start_4);
    }
    if(num_events >= 6) {
        start_stack.push(start_5);
    }
    if(num_events >= 7) {
        start_stack.push(start_6);
    }
    if(num_events >= 8) {
        start_stack.push(start_7);
    }
}
*/
 





/*
void write_to_csv(int bundle_num){
  for(size_t i = 0; i< cd_arr_e0.size(); ++i){
    if(bundle_num == 0) {
      outFile0 << cd_arr_e0[i] << ","  << cd_arr_e1[i] << "," << cd_arr_e2[i] << ","  << cd_arr_e3[i] << "," << cd_arr_e4[i] << "," << cd_arr_e5[i] << "\n";
    }
    else if(bundle_num == 1) {
      outFile1 << cd_arr_e0[i] << ","  << cd_arr_e1[i] << "," << cd_arr_e2[i] << ","  << cd_arr_e3[i] << "," << cd_arr_e4[i] << "\n";
    }
    else if(bundle_num == 2) {
      outFile2 << cd_arr_e0[i] << ","  << cd_arr_e1[i] << "," << cd_arr_e2[i] << ","  << cd_arr_e3[i] << "\n";
    }
    else if(bundle_num == 3) {
      outFile3 << cd_arr_e0[i] << ","  << cd_arr_e1[i] << "," << cd_arr_e2[i] << ","  << cd_arr_e3[i] << "," << cd_arr_e4[i] << ","  << cd_arr_e5[i] << ","  << cd_arr_e6[i] << "\n";
    }
    else if(bundle_num == 4) {
      outFile4 << cd_arr_e0[i] << ","  << cd_arr_e1[i] << "," << cd_arr_e2[i] << "\n";
    }
    else if(bundle_num == 5) {
      outFile5 << cd_arr_e0[i] << ","  << cd_arr_e1[i] << "," << cd_arr_e2[i] << "\n";
    }
    else if(bundle_num == 6){
      outFile6 << cd_arr_e0[i] << ","  << cd_arr_e1[i] << "," << cd_arr_e2[i] << ","  << cd_arr_e3[i] << "," << cd_arr_e4[i] << ","  << cd_arr_e5[i] << ","  << cd_arr_e6[i] << "\n";
    }
    else if(bundle_num == 7){
      outFile7 << cd_arr_e0[i] << ","  << cd_arr_e1[i] << "," << cd_arr_e2[i] << ","  << cd_arr_e3[i] << "\n";
    }
    else if(bundle_num == 8){
      outFile8 << cd_arr_e0[i] << ","  << cd_arr_e1[i] << "," << cd_arr_e2[i] << ","  << cd_arr_e3[i] << "," << cd_arr_e4[i] << ","  << cd_arr_e5[i] << ","  << cd_arr_e6[i] << "\n";
    }
    else if(bundle_num == 9){
      outFile9 << cd_arr_e0[i] << ","  << cd_arr_e1[i] << "," << cd_arr_e2[i] << ","  << cd_arr_e3[i] << "," << cd_arr_e4[i] << ","  << cd_arr_e5[i] << ","  << cd_arr_e6[i] << "\n";
    }
    else if(bundle_num == 10){
      outFile10 << cd_arr_e0[i] << ","  << cd_arr_e1[i] << "," << cd_arr_e2[i] << "," << cd_arr_e3[i] << "," << cd_arr_e4[i] << "\n";
    }
    else if(bundle_num == 11){
      outFile11 << cd_arr_e0[i] << ","  << cd_arr_e1[i] << "," << cd_arr_e2[i] << ","  << cd_arr_e3[i]  << "," << cd_arr_e4[i] << "," << cd_arr_e5[i] << "\n";
    }
    else if(bundle_num == 12){
      outFile12 << cd_arr_e0[i] << ","  << cd_arr_e1[i] << "," << cd_arr_e2[i] << ","  << cd_arr_e3[i]  << "," << cd_arr_e4[i] << "\n";
    }
    else if(bundle_num == 13){
      outFile13 << cd_arr_e0[i] << ","  << cd_arr_e1[i] << "," << cd_arr_e2[i] << ","  << cd_arr_e3[i]  << "," << cd_arr_e4[i] << "," << cd_arr_e5[i] << "," << cd_arr_e6[i] << "\n";
    }
    else if(bundle_num == 14){
      outFile14 << cd_arr_e0[i] << ","  << cd_arr_e1[i] << "," << cd_arr_e2[i] << ","  << cd_arr_e3[i]  << "," << cd_arr_e4[i] << "," << cd_arr_e5[i] << "," << cd_arr_e6[i] << "\n";
    }
  }

  cd_arr0.clear();
  cd_arr1.clear();
  cd_arr2.clear();
  cd_arr3.clear();
  cd_arr4.clear();
  cd_arr5.clear();
  cd_arr6.clear();

  outFile0.close();
  outFile1.close();
  outFile2.close();
  outFile3.close();
  outFile4.close();
  outFile5.close();
  outFile6.close();
  outFile7.close();
  outFile8.close();
  outFile9.close();
  outFile10.close();
  outFile11.close();
  outFile12.close();
  outFile13.close();
  outFile14.close();

  
}*/

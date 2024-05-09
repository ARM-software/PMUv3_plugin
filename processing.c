#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include "pmuv3_plugin_bundle.h"
#include "processing.h"

#define MAX_SIZE 10000

uint64_t *cd_arr0 = NULL;
uint64_t *cd_arr1 = NULL;
uint64_t *cd_arr2 = NULL;
uint64_t *cd_arr3 = NULL;
uint64_t *cd_arr4 = NULL;
uint64_t *cd_arr5 = NULL;
uint64_t *cd_arr6 = NULL;

uint64_t *cd_arr_e0 = NULL;
uint64_t *cd_arr_e1 = NULL;
uint64_t *cd_arr_e2 = NULL;
uint64_t *cd_arr_e3 = NULL;
uint64_t *cd_arr_e4 = NULL;
uint64_t *cd_arr_e5 = NULL;
uint64_t *cd_arr_e6 = NULL;
const char *context_arr[MAX_SIZE];

uint64_t arr_size0 = 0;
uint64_t arr_size1 = 0;
uint64_t arr_size2 = 0;
uint64_t arr_size3 = 0;
uint64_t arr_size4 = 0;
uint64_t arr_size5 = 0;
uint64_t arr_size6 = 0;
uint64_t context_count = 0;
uint64_t cycle_diff_0,cycle_diff_1,cycle_diff_2,cycle_diff_3,cycle_diff_4,cycle_diff_5,cycle_diff_6;

// Function to add an element to the end of a specific array
void pushback(uint64_t **arr, uint64_t *arr_size, uint64_t value) {
    if (*arr_size == 0) {
        *arr = (uint64_t *)malloc(sizeof(uint64_t));
    } else {
        *arr = (uint64_t *)realloc(*arr, (*arr_size + 1) * sizeof(uint64_t));
    }

    // Check if memory allocation succeeded
    if (*arr == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    // Append the value to the array
    (*arr)[(*arr_size)++] = value;
}

// Function to push a context string into the array
void push_context(const char *context) {
    if (context_count < MAX_SIZE) {
        // Allocate memory for the context string
        context_arr[context_count] = malloc(strlen(context) + 1);
        if (context_arr[context_count] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }

        // Copy the context string into the allocated memory
        strcpy((char *)context_arr[context_count], context);

        // Increment the context count
        context_count++;
    } else {
        fprintf(stderr, "Context array is full\n");
    }
}

// Function to open the CSV file corresponding to the bundle number
FILE* open_csv_file(int bundle_num) {
    FILE* outFile = NULL;
    if (bundle_num >= 0 && bundle_num <= 14) {
        char filename[20]; // Assuming maximum filename length is 20 characters
        sprintf(filename, "bundle%d.csv", bundle_num);
        outFile = fopen(filename, "w");
        if (outFile == NULL) {
            perror("Error opening CSV file");
        }
    } else {
        fprintf(stderr, "Invalid bundle number: %d\n", bundle_num);
    }
    return outFile;
}

void process_data(int bundle_num, uint64_t index_test) {
    printf("In process_data() bundle_num, index_test: %d, %ld\n", bundle_num, index_test);
    generate_cycle_diff(num_events);
    FILE *outFile = open_csv_file(bundle_num);
    write_column_names_to_csv(bundle_num, outFile);
    write_to_csv(bundle_num, outFile);
    fclose(outFile);
    free(cd_arr0);
    free(cd_arr1);
    free(cd_arr2);
    free(cd_arr3);
    free(cd_arr4);
    free(cd_arr5);
    free(cd_arr6);
    free(cd_arr_e0);
    free(cd_arr_e1);
    free(cd_arr_e2);
    free(cd_arr_e3);
    free(cd_arr_e4);
    free(cd_arr_e5);
    free(cd_arr_e6);
}

void post_process(int bundle_num) {
    cycle_diff(num_events);
    FILE *outFile = open_csv_file(bundle_num);
    add_column_names_to_csv(bundle_num, outFile);
    add_values_to_csv(bundle_num, outFile);
    fclose(outFile);
    free(cd_arr0);
    free(cd_arr1);
    free(cd_arr2);
    free(cd_arr3);
    free(cd_arr4);
    free(cd_arr5);
    free(cd_arr6);
    free(cd_arr_e0);
    free(cd_arr_e1);
    free(cd_arr_e2);
    free(cd_arr_e3);
    free(cd_arr_e4);
    free(cd_arr_e5);
    free(cd_arr_e6);
}

void add_column_names_to_csv(int bundle_num, FILE* outFile) {
    if (bundle_num == 0) {
        fprintf(outFile, "CPU_CYCLES,L1D_TLB_REFILL,L1D_TLB,L2D_TLB_REFILL,L2D_TLB,DTLB_WALK\n");
    } else if (bundle_num == 1) {
        fprintf(outFile, "CPU_CYCLES,L2D_TLB_REFILL_RD,L2D_TLB_REFILL_WR,L2D_TLB_RD,L2D_TLB_WR\n");
    } else if (bundle_num == 2) {
        fprintf(outFile, "CPU_CYCLES,MEM_ACCESS,BUS_ACCESS,MEMORY_ERROR\n");
    } else if (bundle_num == 3) {
        fprintf(outFile, "CPU_CYCLES,BR_MIS_PRED,BR_PRED,BR_RETIRED,BR_MIS_PRED_RETIRED,BR_IMMED_SPEC,BR_INDIRECT_SPEC\n");
    } else if (bundle_num == 4) {
        fprintf(outFile, "CPU_CYCLES,STALL_FRONTEND,STALL_BACKEND\n");
    } else if (bundle_num == 5) {
        fprintf(outFile, "CPU_CYCLES,L1I_CACHE_REFILL,L1I_CACHE\n");
    } else if (bundle_num == 6) {
        fprintf(outFile, "CPU_CYCLES,L1D_CACHE_REFILL,L1D_CACHE,L2D_CACHE,L2D_CACHE_REFILL,L3D_CACHE_REFILL,L3D_CACHE\n");
    } else if (bundle_num == 7) {
        fprintf(outFile, "CPU_CYCLES,L1I_TLB_REFILL,L1I_TLB,ITLB_WALK\n");
    } else if (bundle_num == 8) {
        fprintf(outFile, "CPU_CYCLES,INST_RETIRED,INST_SPEC,EXC_TAKEN,ST_SPEC,ASE_SPEC,PC_WRITE_SPEC\n");
    } else if (bundle_num == 9) {
        fprintf(outFile, "CPU_CYCLES,BR_RETURN_SPEC,BR_IMMED_SPEC,BR_INDIRECT_SPEC,INST_SPEC,LD_SPEC,DSB_SPEC\n");
    } else if (bundle_num == 10) {
        fprintf(outFile, "CPU_CYCLES,L1D_TLB_REFILL_RD,L1D_TLB_REFILL_WR,L1D_TLB_RD,L1D_TLB_WR\n");
    } else if (bundle_num == 11) {
        fprintf(outFile, "CPU_CYCLES,INST_RETIRED,LL_CACHE_MISS_RD,L1D_CACHE_REFILL,ITLB_WALK,L1I_CACHE_REFILL\n");
    } else if (bundle_num == 12) {
        fprintf(outFile, "CPU_CYCLES,INST_RETIRED,DTLB_WALK,BR_MIS_PRED_RETIRED,L2D_CACHE_REFILL\n");
    } else if (bundle_num == 13) {
        fprintf(outFile, "CPU_CYCLES,L1D_CACHE_REFILL_OUTER,L1D_CACHE_REFILL,L1D_CACHE_REFILL_RD,L1D_CACHE_RD,L1D_CACHE_REFILL_WR,L1D_CACHE_WR\n");
    } else if (bundle_num == 14) {
        fprintf(outFile, "CPU_CYCLES,CRYPTO_SPEC,ISB_SPEC,DP_SPEC,DMB_SPEC,VFP_SPEC,INST_SPEC\n");
    }
}
void write_column_names_to_csv(int bundle_num, FILE *outFile) {
    if(bundle_num == 0) {
        fprintf(outFile, "CONTEXT,CPU_CYCLES,L1D_TLB_REFILL,L1D_TLB,L2D_TLB_REFILL,L2D_TLB,DTLB_WALK\n");
    }
    else if(bundle_num == 1) {
        fprintf(outFile, "CONTEXT,CPU_CYCLES,L2D_TLB_REFILL_RD,L2D_TLB_REFILL_WR,L2D_TLB_RD,L2D_TLB_WR\n");
    }
    else if(bundle_num == 2) {
        fprintf(outFile, "CONTEXT,CPU_CYCLES,MEM_ACCESS,BUS_ACCESS,MEMORY_ERROR\n");
    }
    else if(bundle_num == 3) {
        fprintf(outFile, "CONTEXT,CPU_CYCLES,BR_MIS_PRED,BR_PRED,BR_RETIRED,BR_MIS_PRED_RETIRED,BR_IMMED_SPEC,BR_INDIRECT_SPEC\n");
    }
    else if(bundle_num == 4) {
        fprintf(outFile, "CONTEXT,CPU_CYCLES,STALL_FRONTEND,STALL_BACKEND\n");
    }
    else if(bundle_num == 5) {
        fprintf(outFile, "CONTEXT,CPU_CYCLES,L1I_CACHE_REFILL,L1I_CACHE\n");
    }
    else if(bundle_num == 6){
        fprintf(outFile, "CONTEXT,CPU_CYCLES,L1D_CACHE_REFILL,L1D_CACHE,L2D_CACHE,L2D_CACHE_REFILL,L3D_CACHE_REFILL,L3D_CACHE\n");
    }
    else if(bundle_num == 7){
        fprintf(outFile, "CONTEXT,CPU_CYCLES,L1I_TLB_REFILL,L1I_TLB,ITLB_WALK\n");
    }
    else if(bundle_num == 8){
        fprintf(outFile, "CONTEXT,CPU_CYCLES,INST_RETIRED,INST_SPEC,EXC_TAKEN,ST_SPEC,ASE_SPEC,PC_WRITE_SPEC\n");
    }
    else if(bundle_num == 9){
        fprintf(outFile, "CONTEXT,CPU_CYCLES,BR_RETURN_SPEC,BR_IMMED_SPEC,BR_INDIRECT_SPEC,INST_SPEC,LD_SPEC,DSB_SPEC\n");
    }
    else if(bundle_num == 10){
        fprintf(outFile, "CONTEXT,CPU_CYCLES,L1D_TLB_REFILL_RD,L1D_TLB_REFILL_WR,L1D_TLB_RD,L1D_TLB_WR\n");
    }
    else if(bundle_num == 11){
        fprintf(outFile, "CONTEXT,CPU_CYCLES,INST_RETIRED,LL_CACHE_MISS_RD,L1D_CACHE_REFILL,ITLB_WALK,L1I_CACHE_REFILL\n");
    }
    else if(bundle_num == 12){
        fprintf(outFile, "CONTEXT,CPU_CYCLES,INST_RETIRED,DTLB_WALK,BR_MIS_PRED_RETIRED,L2D_CACHE_REFILL\n");
    }
    else if(bundle_num == 13){
        fprintf(outFile, "CONTEXT,CPU_CYCLES,L1D_CACHE_REFILL_OUTER,L1D_CACHE_REFILL,L1D_CACHE_REFILL_RD,L1D_CACHE_RD,L1D_CACHE_REFILL_WR,L1D_CACHE_WR\n");
    }
    else if(bundle_num == 14){
        fprintf(outFile, "CONTEXT,CPU_CYCLES,CRYPTO_SPEC,ISB_SPEC,DP_SPEC,DMB_SPEC,VFP_SPEC,INST_SPEC\n");
    }
}

void add_values_to_csv(int bundle_num, FILE* outFile) {
    for (size_t i = 0; i < arr_size0; ++i) {
        if (bundle_num == 0) {
            fprintf(outFile, "%ld,%ld,%ld,%ld,%ld,%ld\n",
                    (long)cd_arr_e0[i], (long)cd_arr_e1[i], (long)cd_arr_e2[i],
                    (long)cd_arr_e3[i], (long)cd_arr_e4[i], (long)cd_arr_e5[i]);
        } else if (bundle_num == 1) {
            fprintf(outFile, "%ld,%ld,%ld,%ld,%ld\n",
                    (long)cd_arr_e0[i], (long)cd_arr_e1[i], (long)cd_arr_e2[i],
                    (long)cd_arr_e3[i], (long)cd_arr_e4[i]);
        } else if (bundle_num == 2) {
            fprintf(outFile, "%ld,%ld,%ld,%ld\n",
                    (long)cd_arr_e0[i], (long)cd_arr_e1[i], (long)cd_arr_e2[i], (long)cd_arr_e3[i]);
        } else if (bundle_num == 3) {
            fprintf(outFile, "%ld,%ld,%ld,%ld,%ld,%ld,%ld\n",
                    (long)cd_arr_e0[i], (long)cd_arr_e1[i], (long)cd_arr_e2[i], (long)cd_arr_e3[i],
                    (long)cd_arr_e4[i], (long)cd_arr_e5[i], (long)cd_arr_e6[i]);
        } else if (bundle_num == 4) {
            fprintf(outFile, "%ld,%ld,%ld\n",
                    (long)cd_arr_e0[i], (long)cd_arr_e1[i], (long)cd_arr_e2[i]);
        } else if (bundle_num == 5) {
            fprintf(outFile, "%ld,%ld,%ld\n",
                    (long)cd_arr_e0[i], (long)cd_arr_e1[i], (long)cd_arr_e2[i]);
        } else if (bundle_num == 6) {
            fprintf(outFile, "%ld,%ld,%ld,%ld,%ld,%ld,%ld\n",
                    (long)cd_arr_e0[i], (long)cd_arr_e1[i], (long)cd_arr_e2[i], (long)cd_arr_e3[i],
                    (long)cd_arr_e4[i], (long)cd_arr_e5[i], (long)cd_arr_e6[i]);
        } else if (bundle_num == 7) {
            fprintf(outFile, "%ld,%ld,%ld,%ld\n",
                    (long)cd_arr_e0[i], (long)cd_arr_e1[i], (long)cd_arr_e2[i], (long)cd_arr_e3[i]);
        } else if (bundle_num == 8) {
            fprintf(outFile, "%ld,%ld,%ld,%ld,%ld,%ld,%ld\n",
                    (long)cd_arr_e0[i], (long)cd_arr_e1[i], (long)cd_arr_e2[i], (long)cd_arr_e3[i],
                    (long)cd_arr_e4[i], (long)cd_arr_e5[i], (long)cd_arr_e6[i]);
        } else if (bundle_num == 9) {
            fprintf(outFile, "%ld,%ld,%ld,%ld,%ld,%ld,%ld\n",
                    (long)cd_arr_e0[i], (long)cd_arr_e1[i], (long)cd_arr_e2[i], (long)cd_arr_e3[i],
                    (long)cd_arr_e4[i], (long)cd_arr_e5[i], (long)cd_arr_e6[i]);
        } else if (bundle_num == 10) {
            fprintf(outFile, "%ld,%ld,%ld,%ld,%ld\n",
                    (long)cd_arr_e0[i], (long)cd_arr_e1[i], (long)cd_arr_e2[i], (long)cd_arr_e3[i], (long)cd_arr_e4[i]);
        } else if (bundle_num == 11) {
            fprintf(outFile, "%ld,%ld,%ld,%ld,%ld,%ld\n",
                    (long)cd_arr_e0[i], (long)cd_arr_e1[i], (long)cd_arr_e2[i], (long)cd_arr_e3[i],
                    (long)cd_arr_e4[i], (long)cd_arr_e5[i]);
        } else if (bundle_num == 12) {
            fprintf(outFile, "%ld,%ld,%ld,%ld,%ld\n",
                    (long)cd_arr_e0[i], (long)cd_arr_e1[i], (long)cd_arr_e2[i], (long)cd_arr_e3[i],
                    (long)cd_arr_e4[i]);
        } else if (bundle_num == 13) {
            fprintf(outFile, "%ld,%ld,%ld,%ld,%ld,%ld,%ld\n",
                    (long)cd_arr_e0[i], (long)cd_arr_e1[i], (long)cd_arr_e2[i], (long)cd_arr_e3[i],
                    (long)cd_arr_e4[i], (long)cd_arr_e5[i], (long)cd_arr_e6[i]);
        } else if (bundle_num == 14) {
            fprintf(outFile, "%ld,%ld,%ld,%ld,%ld,%ld,%ld\n",
                    (long)cd_arr_e0[i], (long)cd_arr_e1[i], (long)cd_arr_e2[i], (long)cd_arr_e3[i],
                    (long)cd_arr_e4[i], (long)cd_arr_e5[i], (long)cd_arr_e6[i]);
        } else {
            printf("Invalid bundle number: %d\n", bundle_num);
        }
    }
}

void write_to_csv(int bundle_num, FILE* outFile) {

    for (uint64_t i = 0; i < arr_size0; ++i) {
        if (bundle_num == 0) {
            fprintf(outFile, "%s,%ld,%ld,%ld,%ld,%ld,%ld\n", context_arr[i],
                (long)cd_arr0[i], (long)cd_arr1[i], (long)cd_arr2[i],
                (long)cd_arr3[i], (long)cd_arr4[i], (long)cd_arr5[i]);
        } else if (bundle_num == 1) {
            fprintf(outFile, "%s,%ld,%ld,%ld,%ld,%ld\n", context_arr[i],
                    (long)cd_arr0[i], (long)cd_arr1[i], (long)cd_arr2[i],
                    (long)cd_arr3[i], (long)cd_arr4[i]);
        } else if (bundle_num == 2) {
            fprintf(outFile, "%s,%ld,%ld,%ld,%ld\n", context_arr[i],
                    (long)cd_arr0[i], (long)cd_arr1[i], (long)cd_arr2[i], (long)cd_arr3[i]);
        } else if (bundle_num == 3) {
            fprintf(outFile, "%s,%ld,%ld,%ld,%ld,%ld,%ld,%ld\n", context_arr[i],
                    (long)cd_arr0[i], (long)cd_arr1[i], (long)cd_arr2[i], (long)cd_arr3[i],
                    (long)cd_arr4[i], (long)cd_arr5[i], (long)cd_arr6[i]);
        } else if (bundle_num == 4) {
            fprintf(outFile, "%s,%ld,%ld,%ld\n", context_arr[i],
                    (long)cd_arr0[i], (long)cd_arr1[i], (long)cd_arr2[i]);
        } else if (bundle_num == 5) {
            fprintf(outFile, "%s,%ld,%ld,%ld\n", context_arr[i],
                    (long)cd_arr0[i], (long)cd_arr1[i], (long)cd_arr2[i]);
        } else if (bundle_num == 6) {
            fprintf(outFile, "%s,%ld,%ld,%ld,%ld,%ld,%ld,%ld\n", context_arr[i],
                    (long)cd_arr0[i], (long)cd_arr1[i], (long)cd_arr2[i], (long)cd_arr3[i],
                    (long)cd_arr4[i], (long)cd_arr5[i], (long)cd_arr6[i]);
        } else if (bundle_num == 7) {
            fprintf(outFile, "%s,%ld,%ld,%ld,%ld\n", context_arr[i],
                    (long)cd_arr0[i], (long)cd_arr1[i], (long)cd_arr2[i], (long)cd_arr3[i]);
        } else if (bundle_num == 8) {
            fprintf(outFile, "%s,%ld,%ld,%ld,%ld,%ld,%ld,%ld\n", context_arr[i],
                    (long)cd_arr0[i], (long)cd_arr1[i], (long)cd_arr2[i], (long)cd_arr3[i],
                    (long)cd_arr4[i], (long)cd_arr5[i], (long)cd_arr6[i]);
        } else if (bundle_num == 9) {
            fprintf(outFile, "%s,%ld,%ld,%ld,%ld,%ld,%ld,%ld\n", context_arr[i],
                    (long)cd_arr0[i], (long)cd_arr1[i], (long)cd_arr2[i], (long)cd_arr3[i],
                    (long)cd_arr4[i], (long)cd_arr5[i], (long)cd_arr6[i]);
        } else if (bundle_num == 10) {
            fprintf(outFile, "%s,%ld,%ld,%ld,%ld,%ld\n", context_arr[i],
                    (long)cd_arr0[i], (long)cd_arr1[i], (long)cd_arr2[i], (long)cd_arr3[i], (long)cd_arr4[i]);
        } else if (bundle_num == 11) {
            fprintf(outFile, "%s,%ld,%ld,%ld,%ld,%ld,%ld\n", context_arr[i],
                    (long)cd_arr0[i], (long)cd_arr1[i], (long)cd_arr2[i], (long)cd_arr3[i],
                    (long)cd_arr4[i], (long)cd_arr5[i]);
        } else if (bundle_num == 12) {
            fprintf(outFile, "%s,%ld,%ld,%ld,%ld,%ld\n", context_arr[i],
                    (long)cd_arr0[i], (long)cd_arr1[i], (long)cd_arr2[i], (long)cd_arr3[i],
                    (long)cd_arr4[i]);
        } else if (bundle_num == 13) {
            fprintf(outFile, "%s,%ld,%ld,%ld,%ld,%ld,%ld,%ld\n", context_arr[i],
                    (long)cd_arr0[i], (long)cd_arr1[i], (long)cd_arr2[i], (long)cd_arr3[i],
                    (long)cd_arr4[i], (long)cd_arr5[i], (long)cd_arr6[i]);
        } else if (bundle_num == 14) {
            fprintf(outFile, "%s,%ld,%ld,%ld,%ld,%ld,%ld,%ld\n", context_arr[i],
                    (long)cd_arr0[i], (long)cd_arr1[i], (long)cd_arr2[i], (long)cd_arr3[i],
                    (long)cd_arr4[i], (long)cd_arr5[i], (long)cd_arr6[i]);
        } else {
            printf("Invalid bundle number: %d\n", bundle_num);
        }
    }
}

void cycle_diff(int num_events) {
    for(int k = 0; k < num_events; ++k) {
        uint64_t diff = event_counts[0].end_cnt[k] - event_counts[0].start_cnt[k];
        printf("End is %ld, Start is %ld\n", event_counts[0].end_cnt[k], event_counts[0].start_cnt[k]);
        if (diff < 0){
            printf("End is %ld, Start is %ld\n", event_counts[0].end_cnt[k], event_counts[0].start_cnt[k]);
        }
        switch (k) {
            case 0:
                pushback(&cd_arr_e0, &arr_size0, diff);
                break;
            case 1:
                pushback(&cd_arr_e1, &arr_size1, diff);
                break;
            case 2:
                pushback(&cd_arr_e2, &arr_size2, diff);
                break;
            case 3:
                pushback(&cd_arr_e3, &arr_size3, diff);
                break;
            case 4:
                pushback(&cd_arr_e4, &arr_size4, diff);
                break;
            case 5:
                pushback(&cd_arr_e5, &arr_size5, diff);
                break;
            case 6:
                pushback(&cd_arr_e6, &arr_size6, diff);
                break;
            default:
                fprintf(stderr, "Invalid index: %d\n", k);
                break;
        }
    }
}

void generate_cycle_diff(int num_events) {
    for (int i = 0; i < global_index; ++i) {
        push_context(event_counts[i].context);
        for(int k = 0; k < num_events; ++k) {
            uint64_t diff = event_counts[i].end_cnt[k] - event_counts[i].start_cnt[k];
	    printf("End is %ld, Start is %ld, diff is %ld\n", event_counts[i].end_cnt[k], event_counts[i].start_cnt[k], diff);
            if (diff < 0){
                printf("End is %ld, Start is %ld\n", event_counts[i].end_cnt[k], event_counts[i].start_cnt[k]);
            }
            switch (k) {
                case 0:
                    pushback(&cd_arr0, &arr_size0, diff);
                    break;
                case 1:
                    pushback(&cd_arr1, &arr_size1, diff);
                    break;
                case 2:
                    pushback(&cd_arr2, &arr_size2, diff);
                    break;
                case 3:
                    pushback(&cd_arr3, &arr_size3, diff);
                    break;
                case 4:
                    pushback(&cd_arr4, &arr_size4, diff);
                    break;
                case 5:
                    pushback(&cd_arr5, &arr_size5, diff);
                    break;
                case 6:
                    pushback(&cd_arr6, &arr_size6, diff);
                    break;
                default:
                    fprintf(stderr, "Invalid index: %d\n", k);
                    break;
            }
        }
    }
}

/*
 * MIT License
 * Copyright (c) [Year] ARM-software
 *
 * Performance Monitoring accessing PMUv3 counters.
 */

#include "pmuv3_plugin_bundle.h"

int num_bundles;
uint64_t num_events;
uint32_t *event_values;
char **event_names;

bundles bundle0[] = {
    {"CPU_CYCLES", 0x11},
    {"L1D_TLB_REFILL", 0x05},
    {"L1D_TLB", 0x25},
    {"L2D_TLB_REFILL", 0x2D},
    {"L2D_TLB", 0x2F},
    {"DTLB_WALK", 0x34},
};

bundles bundle1[] = {
    {"CPU_CYCLES", 0x11},
    {"L2D_TLB_REFILL_RD", 0x5C},
    {"L2D_TLB_REFILL_WR", 0x5D},
    {"L2D_TLB_RD", 0x5E},
    {"L2D_TLB_WR", 0x5F},
};

bundles bundle2[] = {
    {"CPU_CYCLES", 0x11},
    {"MEM_ACCESS", 0x13},
    {"BUS_ACCESS", 0x19},
    {"MEMORY_ERROR", 0x1A},
};

bundles bundle3[] = {
    {"CPU_CYCLES", 0x11},
    {"BR_MIS_PRED", 0x10},
    {"BR_PRED", 0x12},
    {"BR_RETIRED", 0x21},
    {"BR_MIS_PRED_RETIRED", 0x22},
    {"BR_IMMED_SPEC", 0x78},
    {"BR_INDIRECT_SPEC", 0x7A},
};

bundles bundle4[] = {
    {"CPU_CYCLES", 0x11},
    {"STALL_FRONTEND", 0x23},
    {"STALL_BACKEND", 0x24},
    {"STALL_SLOT_FRONTEND", 0x3E},
    {"STALL_SLOT_BACKEND", 0x3D},
    {"STALL_BACKEND_MEM", 0x4005},
};

bundles bundle5[] = {
    {"CPU_CYCLES", 0x11},
    {"L1I_CACHE_REFILL", 0x01},
    {"L1I_CACHE", 0x14},
};

bundles bundle6[] = {
    {"CPU_CYCLES", 0x11},
    {"L1D_CACHE_REFILL", 0x03},
    {"L1D_CACHE", 0x04},
    {"L2D_CACHE", 0x16},
    {"L2D_CACHE_REFILL", 0x17},
    {"L3D_CACHE_REFILL", 0x2A},
    {"L3D_CACHE", 0x2B},
};

bundles bundle7[] = {
    {"CPU_CYCLES", 0x11},
    {"L1I_TLB_REFILL", 0x02},
    {"L1I_TLB", 0x26},
    {"ITLB_WALK", 0x35},
};

bundles bundle8[] = {
    {"CPU_CYCLES", 0x11},
    {"INST_RETIRED", 0x08},
    {"INST_SPEC", 0x1B},
    {"EXC_TAKEN", 0x09},
    {"ST_SPEC", 0x71},
    {"ASE_SPEC", 0x74},
    {"PC_WRITE_SPEC", 0x76},
};

bundles bundle9[] = {
    {"CPU_CYCLES", 0x11},
    {"BR_RETURN_SPEC", 0x79},
    {"BR_IMMED_SPEC", 0x78},
    {"BR_INDIRECT_SPEC", 0x7A},
    {"INST_SPEC", 0x1B},
    {"LD_SPEC", 0x70},
    {"DSB_SPEC", 0x7D},
};

bundles bundle10[] = {
    {"CPU_CYCLES", 0x11},
    {"L1D_TLB_REFILL_RD", 0x4C},
    {"L1D_TLB_REFILL_WR", 0x4D},
    {"L1D_TLB_RD", 0x4E},
    {"L1D_TLB_WR", 0x4F},
};

bundles bundle11[] = {
    {"CPU_CYCLES", 0x11},
    {"INST_RETIRED", 0x08},
    {"LL_CACHE_MISS_RD", 0x37},
    {"L1D_CACHE_REFILL", 0x03},
    {"ITLB_WALK", 0x35},
    {"L1I_CACHE_REFILL", 0x01},
};

bundles bundle12[] = {
    {"CPU_CYCLES", 0x11},
    {"INST_RETIRED", 0x08},
    {"DTLB_WALK", 0x34},
    {"BR_MIS_PRED_RETIRED", 0x22},
    {"L2D_CACHE_REFILL", 0x17},
};

bundles bundle13[] = {
    {"CPU_CYCLES", 0x11},
    {"L1D_CACHE_REFILL_OUTER", 0x45},
    {"L1D_CACHE_REFILL", 0x03},
    {"L1D_CACHE_REFILL_RD", 0x42},
    {"L1D_CACHE_RD", 0x40},
    {"L1D_CACHE_REFILL_WR", 0x43},
    {"L1D_CACHE_WR", 0x41},
};

bundles bundle14[] = {
    {"CPU_CYCLES", 0x11},
    {"CRYPTO_SPEC", 0x77},
    {"ISB_SPEC", 0x7C},
    {"DP_SPEC", 0x73},
    {"DMB_SPEC", 0x7E},
    {"VFP_SPEC", 0x75},
    {"INST_SPEC", 0x1B},
};

bundles bundle15[] = {
    {"CPU_CYCLES", 0x11},
    {"SVE_INST_SPEC", 0x8006},
    {"SVE_PRED_SPEC", 0x8074},
    {"SVE_PRED_EMPTY_SPEC", 0x8075},
    {"SVE_PRED_FULL_SPEC", 0x8076},
    {"SVE_PRED_PARTIAL_SPEC", 0x8077},
};

bundles bundle16[] = {
    {"CPU_CYCLES", 0x11},
    {"BUS_ACCESS_RD", 0x60},
    {"BUS_ACCESS_WR", 0x61},
    {"MEM_ACCESS_RD", 0x66},
    {"MEM_ACCESS_WR", 0x67},
};

bundles bundle17[] = {
    {"CPU_CYCLES", 0x11},
    {"LL_CACHE_RD", 0x36},
    {"LL_CACHE_MISS_RD", 0x37},
    {"FP_SCALE_OPS_SPEC", 0x80C0},
    {"FP_FIXED_OPS_SPEC", 0x80C1},
};

bundles bundle18[] = {
    {"OP_RETIRED", 0x3A},
    {"OP_SPEC", 0x3B},
    {"STALL_SLOT", 0x3F},
    {"CPU_CYCLES", 0x11},
    {"STALL_SLOT_FRONTEND", 0x3E},
    {"STALL_SLOT_BACKEND", 0x3D},
    {"BR_MIS_PRED", 0x10},
};

struct BundleDefinition {
    bundles *events;
    size_t count;
};

#define BUNDLE_DEFINITION(number) \
    { bundle##number, sizeof(bundle##number) / sizeof(bundle##number[0]) }

static const struct BundleDefinition bundle_definitions[TOTAL_BUNDLE_NUM] = {
    BUNDLE_DEFINITION(0),
    BUNDLE_DEFINITION(1),
    BUNDLE_DEFINITION(2),
    BUNDLE_DEFINITION(3),
    BUNDLE_DEFINITION(4),
    BUNDLE_DEFINITION(5),
    BUNDLE_DEFINITION(6),
    BUNDLE_DEFINITION(7),
    BUNDLE_DEFINITION(8),
    BUNDLE_DEFINITION(9),
    BUNDLE_DEFINITION(10),
    BUNDLE_DEFINITION(11),
    BUNDLE_DEFINITION(12),
    BUNDLE_DEFINITION(13),
    BUNDLE_DEFINITION(14),
    BUNDLE_DEFINITION(15),
    BUNDLE_DEFINITION(16),
    BUNDLE_DEFINITION(17),
    BUNDLE_DEFINITION(18),
};

void free_bundle_memory(void)
{
    if (event_names != NULL) {
        for (uint64_t i = 0; i < num_events; i++)
            free(event_names[i]);
        free(event_names);
        event_names = NULL;
    }
    free(event_values);
    event_values = NULL;
}

void init_bundle(bundles *bundle)
{
    event_values = calloc(num_events, sizeof(*event_values));
    event_names = calloc(num_events, sizeof(*event_names));
    if (event_values == NULL || event_names == NULL) {
        free_bundle_memory();
        return;
    }

    for (uint64_t i = 0; i < num_events; i++) {
        event_values[i] = bundle[i].event_value;
        event_names[i] = strdup(bundle[i].name);
        if (event_names[i] == NULL) {
            free_bundle_memory();
            return;
        }
    }
}

int pmuv3_bundle_init(int bundle_num)
{
    if (bundle_num < 0 || bundle_num >= TOTAL_BUNDLE_NUM) {
        fprintf(stderr, "PMUv3: bundle number must be in [0,%d]\n",
                TOTAL_BUNDLE_NUM - 1);
        return -1;
    }

    num_bundles = bundle_num;
    num_events = bundle_definitions[bundle_num].count;
    if (num_events > MAX_EVENTS)
        return -1;

    init_bundle(bundle_definitions[bundle_num].events);
    if (event_values == NULL || event_names == NULL)
        return -1;
    if (init_api(0, NULL, event_values) != 0) {
        free_bundle_memory();
        return -1;
    }
    return 0;
}

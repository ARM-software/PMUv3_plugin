#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "pmuv3_plugin_bundle.h"
#include "pmuv3_processing.h"

static volatile uint64_t g_sink;

static void branch_region(unsigned long iters)
{
    uint64_t acc = 0x517cc1b727220a95ULL;

    for (unsigned long i = 0; i < iters; i++) {
        if ((acc ^ i) & 1)
            acc = acc * 0x9e3779b97f4a7c15ULL + i;
        else
            acc ^= (i << 7) | (i >> 3);
    }

    g_sink ^= acc;
}

static void memory_region(unsigned long iters)
{
    enum { N = 4096 };
    uint64_t *buf = calloc(N, sizeof(*buf));
    uint64_t acc = 0;

    if (buf == NULL)
        return;

    for (int i = 0; i < N; i++)
        buf[i] = (uint64_t)i * 1315423911ULL;

    for (unsigned long i = 0; i < iters; i++) {
        unsigned idx = (unsigned)((i * 257UL) & (N - 1));
        acc += buf[idx];
        buf[idx] ^= acc + i;
    }

    g_sink ^= acc;
    free(buf);
}

static void measure_region(const char *name, void (*fn)(unsigned long),
                           unsigned long iters)
{
    uint64_t idx = get_next_index();

    get_start_count(&count_data, name, idx);
    fn(iters);
    get_end_count(&count_data, name, idx);

    for (uint64_t i = 0; i < num_events; i++) {
        printf("C_MULTI,%s,%s,%" PRIu64 "\n", name, event_names[i],
               event_counts[idx].end_cnt[i] - event_counts[idx].start_cnt[i]);
    }
}

int main(int argc, char **argv)
{
    int bundle = 4;
    unsigned long iters = 50000000UL;

    if (argc > 1)
        bundle = atoi(argv[1]);
    if (argc > 2)
        iters = strtoul(argv[2], NULL, 0);

    if (pmuv3_bundle_init(bundle) != 0)
        return 1;

    measure_region("c_branch_region", branch_region, iters);
    measure_region("c_memory_region", memory_region, iters / 4);

    process_data(bundle);
    shutdown_resources();

    printf("C_MULTI_DONE bundle=%d sink=%" PRIu64 "\n", bundle, g_sink);
    return 0;
}

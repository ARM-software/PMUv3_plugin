#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "pmuv3_plugin_bundle.h"
#include "processing.h"

static volatile uint64_t g_sink;

static void work_region(unsigned long iters)
{
    uint64_t acc = 0x12345678ULL;

    for (unsigned long i = 0; i < iters; i++) {
        acc += i;
        acc ^= acc << 7;
        acc ^= acc >> 3;
    }

    g_sink = acc;
}

int main(int argc, char **argv)
{
    int bundle = 0;
    unsigned long iters = 100000000UL;

    if (argc > 1)
        bundle = atoi(argv[1]);
    if (argc > 2)
        iters = strtoul(argv[2], NULL, 0);

    pmuv3_bundle_init(bundle);

    uint64_t idx = get_next_index();
    get_start_count(&count_data, "work_region", idx);
    work_region(iters);
    get_end_count(&count_data, "work_region", idx);

    process_data(bundle);
    shutdown_resources();

    printf("bundle=%d iters=%lu sink=%lu\n", bundle, iters, (unsigned long)g_sink);
    return 0;
}

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cinttypes>

#include "pmuv3_plugin_bundle.h"
#include "pmuv3_processing.hpp"

static volatile std::uint64_t g_sink;

static void compute_region(unsigned long iters)
{
    std::uint64_t acc = 0x123456789abcdef0ULL;

    for (unsigned long i = 0; i < iters; i++) {
        acc += i;
        acc ^= acc << 11;
        acc ^= acc >> 7;
    }

    g_sink = acc;
}

int main(int argc, char **argv)
{
    int cur_bundle_no = 4;
    unsigned long iters = 50000000UL;

    if (argc > 1)
        cur_bundle_no = std::atoi(argv[1]);
    if (argc > 2)
        iters = std::strtoul(argv[2], nullptr, 0);

    if (pmuv3_bundle_init(cur_bundle_no) != 0)
        return 1;

    process_start_count(&count_data);
    compute_region(iters);
    process_end_count(&count_data);

    for (std::uint64_t i = 0; i < num_events; i++) {
        std::printf("CPP_SINGLE,%s,%" PRIu64 "\n", event_names[i],
                    event_counts[0].end_cnt[i] -
                    event_counts[0].start_cnt[i]);
    }

    process_single_chunk(cur_bundle_no);
    shutdown_resources();

    std::printf("CPP_SINGLE_DONE bundle=%d sink=%" PRIu64 "\n",
                cur_bundle_no, static_cast<std::uint64_t>(g_sink));
    return 0;
}

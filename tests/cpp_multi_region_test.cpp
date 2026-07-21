#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "pmuv3_plugin_bundle.h"
#include "pmuv3_processing.hpp"

static volatile std::uint64_t g_sink;

static void vector_region(unsigned long iters)
{
    std::vector<std::uint64_t> data(8192);
    std::uint64_t acc = 0xabcdef1234567890ULL;

    for (std::size_t i = 0; i < data.size(); i++)
        data[i] = i * 0x9e3779b97f4a7c15ULL;

    for (unsigned long i = 0; i < iters; i++) {
        std::size_t idx = (i * 131UL) & (data.size() - 1);
        acc ^= data[idx];
        data[idx] += acc + i;
    }

    g_sink ^= acc;
}

static void compute_region(unsigned long iters)
{
    std::uint64_t acc = 0x12345678ULL;

    for (unsigned long i = 0; i < iters; i++) {
        acc += i;
        acc = (acc << 13) | (acc >> 51);
        acc *= 0xd1b54a32d192ed03ULL;
    }

    g_sink ^= acc;
}

static void measure_region(const char *name, void (*fn)(unsigned long),
                           unsigned long iters)
{
    std::uint64_t idx = get_next_index();

    get_start_count(&count_data, name, idx);
    fn(iters);
    get_end_count(&count_data, name, idx);

    for (std::uint64_t i = 0; i < num_events; i++) {
        std::printf("CPP_MULTI,%s,%s,%" PRIu64 "\n", name, event_names[i],
                    event_counts[idx].end_cnt[i] -
                    event_counts[idx].start_cnt[i]);
    }
}

int main(int argc, char **argv)
{
    int bundle = 4;
    unsigned long iters = 50000000UL;

    if (argc > 1)
        bundle = std::atoi(argv[1]);
    if (argc > 2)
        iters = std::strtoul(argv[2], nullptr, 0);

    if (pmuv3_bundle_init(bundle) != 0)
        return 1;

    measure_region("cpp_vector_region", vector_region, iters / 5);
    measure_region("cpp_compute_region", compute_region, iters);

    process_data(bundle);
    shutdown_resources();

    std::printf("CPP_MULTI_DONE bundle=%d sink=%" PRIu64 "\n", bundle,
                static_cast<std::uint64_t>(g_sink));
    return 0;
}

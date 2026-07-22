#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "pmuv3_plugin_bundle.h"

int main(int argc, char **argv)
{
    int bundle = argc > 1 ? std::atoi(argv[1]) : 4;
    std::uint64_t iterations = argc > 2 ? std::strtoull(argv[2], nullptr, 0)
                                        : 500000000ULL;
    if (pmuv3_bundle_init(bundle) != 0)
        return 1;

    volatile std::uint64_t k = 0;
    process_start_count(&count_data);
    while (k < iterations)
        k++;
    process_end_count(&count_data);

    std::uint64_t cycles = event_counts[0].end_cnt[0] -
                           event_counts[0].start_cnt[0];
    std::printf("CPP_VOLATILE,iterations,%" PRIu64 ",cycles,%" PRIu64 "\n",
                iterations, cycles);
    int status = shutdown_resources();
    return status != 0 || cycles == 0;
}

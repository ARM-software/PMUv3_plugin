#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <thread>
#include <vector>
#include <pthread.h>

#include "pmuv3_plugin_bundle.h"
#include "pmuv3_processing.hpp"

static void volatile_loop(std::uint64_t iterations)
{
    volatile std::uint64_t k = 0;
    while (k < iterations)
        k++;
}

static pthread_barrier_t workers_created;
static pthread_barrier_t counters_initialized;

int main(int argc, char **argv)
{
    int bundle = argc > 1 ? std::atoi(argv[1]) : 4;
    int thread_count = argc > 2 ? std::atoi(argv[2]) : 4;
    std::uint64_t iterations = argc > 3 ? std::strtoull(argv[3], nullptr, 0)
                                        : 10000000ULL;

    if (thread_count <= 0)
        return 1;

    std::vector<std::thread> threads;
    std::vector<int> status(static_cast<std::size_t>(thread_count));
    std::vector<std::string> contexts(static_cast<std::size_t>(thread_count) * 2);
    pthread_barrier_init(&workers_created, nullptr,
                         static_cast<unsigned>(thread_count + 1));
    pthread_barrier_init(&counters_initialized, nullptr,
                         static_cast<unsigned>(thread_count + 1));
    for (int i = 0; i < thread_count; i++) {
        contexts[static_cast<std::size_t>(i) * 2] =
            "cpp_thread_" + std::to_string(i) + "_region_0";
        contexts[static_cast<std::size_t>(i) * 2 + 1] =
            "cpp_thread_" + std::to_string(i) + "_region_1";
        threads.emplace_back([&, i]() {
            pthread_barrier_wait(&workers_created);
            pthread_barrier_wait(&counters_initialized);
            if (pmuv3_register_current_thread() != 0) {
                status[static_cast<std::size_t>(i)] = 1;
                return;
            }

            process_start_count(&count_data);
            volatile_loop(iterations);
            process_end_count(&count_data);

            for (int region = 0; region < 2; region++) {
                std::uint64_t index = get_next_index();
                const char *name = contexts[static_cast<std::size_t>(i) * 2 + region].c_str();
                get_start_count(&count_data, name, index);
                volatile_loop(iterations / static_cast<std::uint64_t>(region + 2));
                get_end_count(&count_data, name, index);
            }
            status[static_cast<std::size_t>(i)] =
                pmuv3_unregister_current_thread() != 0;
        });
    }
    pthread_barrier_wait(&workers_created);
    int failed = pmuv3_bundle_init(bundle) != 0;
    pthread_barrier_wait(&counters_initialized);
    for (auto &thread : threads)
        thread.join();

    for (int value : status)
        failed |= value;
    std::printf("CPP_MT,threads,%d,rows,%" PRIu64 "\n", thread_count, global_index);
    for (std::uint64_t row = 0; row < global_index; row++)
        std::printf("CPP_MT_ROW,%" PRIu64 ",%s,cycles,%" PRIu64 "\n", row,
                    event_counts[row].context,
                    event_counts[row].end_cnt[0] - event_counts[row].start_cnt[0]);

    process_data(bundle);
    failed |= shutdown_resources() != 0;
    pthread_barrier_destroy(&counters_initialized);
    pthread_barrier_destroy(&workers_created);
    return failed;
}

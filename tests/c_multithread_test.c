#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "pmuv3_plugin_bundle.h"
#include "pmuv3_processing.h"

struct WorkerArg {
    int id;
    uint64_t iterations;
    char contexts[2][64];
};

static pthread_barrier_t workers_created;
static pthread_barrier_t counters_initialized;

static void volatile_loop(uint64_t iterations)
{
    volatile uint64_t k = 0;
    while (k < iterations)
        k++;
}

static void *worker(void *opaque)
{
    struct WorkerArg *arg = opaque;
    pthread_barrier_wait(&workers_created);
    pthread_barrier_wait(&counters_initialized);
    if (pmuv3_register_current_thread() != 0)
        return (void *)(uintptr_t)1;

    process_start_count(&count_data);
    volatile_loop(arg->iterations);
    process_end_count(&count_data);

    for (int region = 0; region < 2; region++) {
        uint64_t index = get_next_index();
        get_start_count(&count_data, arg->contexts[region], index);
        volatile_loop(arg->iterations / (uint64_t)(region + 2));
        get_end_count(&count_data, arg->contexts[region], index);
    }

    return (void *)(uintptr_t)(pmuv3_unregister_current_thread() != 0);
}

int main(int argc, char **argv)
{
    int bundle = argc > 1 ? atoi(argv[1]) : 4;
    int thread_count = argc > 2 ? atoi(argv[2]) : 4;
    uint64_t iterations = argc > 3 ? strtoull(argv[3], NULL, 0) : 10000000ULL;

    if (thread_count <= 0)
        return 1;

    pthread_t *threads = calloc((size_t)thread_count, sizeof(*threads));
    struct WorkerArg *args = calloc((size_t)thread_count, sizeof(*args));
    if (threads == NULL || args == NULL)
        return 1;

    pthread_barrier_init(&workers_created, NULL, (unsigned)thread_count + 1);
    pthread_barrier_init(&counters_initialized, NULL, (unsigned)thread_count + 1);

    for (int i = 0; i < thread_count; i++) {
        args[i].id = i;
        args[i].iterations = iterations;
        snprintf(args[i].contexts[0], sizeof(args[i].contexts[0]),
                 "c_thread_%d_region_0", i);
        snprintf(args[i].contexts[1], sizeof(args[i].contexts[1]),
                 "c_thread_%d_region_1", i);
        pthread_create(&threads[i], NULL, worker, &args[i]);
    }

    pthread_barrier_wait(&workers_created);
    int failed = pmuv3_bundle_init(bundle) != 0;
    pthread_barrier_wait(&counters_initialized);

    for (int i = 0; i < thread_count; i++) {
        void *result = NULL;
        pthread_join(threads[i], &result);
        failed |= result != NULL;
    }

    printf("C_MT,threads,%d,rows,%" PRIu64 "\n", thread_count, global_index);
    for (uint64_t row = 0; row < global_index; row++)
        printf("C_MT_ROW,%" PRIu64 ",%s,cycles,%" PRIu64 "\n", row,
               event_counts[row].context,
               event_counts[row].end_cnt[0] - event_counts[row].start_cnt[0]);

    process_data(bundle);
    failed |= shutdown_resources() != 0;
    pthread_barrier_destroy(&counters_initialized);
    pthread_barrier_destroy(&workers_created);
    free(args);
    free(threads);
    return failed;
}

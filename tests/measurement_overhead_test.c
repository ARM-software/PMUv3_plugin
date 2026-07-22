#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "pmuv3_plugin_bundle.h"

static volatile uint64_t sink;

static inline uint64_t xrand64(uint64_t *state)
{
    *state ^= *state << 13;
    *state ^= *state >> 7;
    *state ^= *state << 17;
    return *state;
}

static inline uint64_t read_cntvct(void)
{
#if defined(__aarch64__)
    uint64_t value;
    asm volatile("mrs %0, cntvct_el0" : "=r"(value));
    return value;
#else
    return 0;
#endif
}

static __attribute__((noinline)) void empty_section(uint64_t *state)
{
    sink += *state;
}

static __attribute__((noinline)) void tiny_branch_section(uint64_t *state)
{
    uint64_t payload = xrand64(state);
    uint64_t value = sink;
    for (int i = 0; i < 8; i++) {
        if ((payload >> (i * 7)) & 1) {
            value ^= payload;
            value *= 0x9e3779b97f4a7c15ULL;
        } else {
            value += payload;
            value ^= 0xd1b54a32d192ed03ULL;
        }
        payload = (payload >> 1) | (payload << 63);
    }
    sink = value;
}

static __attribute__((noinline)) void branch64_section(uint64_t *state)
{
    uint64_t payload = xrand64(state);
    uint64_t value = sink;
    for (int i = 0; i < 64; i++) {
        if ((payload >> (i & 63)) & 1) {
            value ^= payload;
            value *= 0x9e3779b97f4a7c15ULL;
        } else {
            value += payload;
            value ^= 0xd1b54a32d192ed03ULL;
        }
        payload = (payload >> 1) | (payload << 63);
    }
    sink = value;
}

typedef void (*section_fn)(uint64_t *);

static void run_case(const char *name, section_fn function, int repetitions)
{
    uint64_t state = 0x517cc1b727220a95ULL;
    uint64_t pmu_sum = 0, pmu_min = UINT64_MAX, pmu_max = 0;
    uint64_t timer_sum = 0, timer_min = UINT64_MAX, timer_max = 0;

    for (int i = 0; i < 1000; i++)
        function(&state);
    for (int i = 0; i < repetitions; i++) {
        uint64_t index = (uint64_t)i;
        uint64_t before = read_cntvct();
        get_start_count(&count_data, name, index);
        function(&state);
        get_end_count(&count_data, name, index);
        uint64_t after = read_cntvct();

        uint64_t pmu = event_counts[index].end_cnt[0] -
                       event_counts[index].start_cnt[0];
        uint64_t timer = after - before;
        pmu_sum += pmu;
        timer_sum += timer;
        if (pmu < pmu_min) pmu_min = pmu;
        if (pmu > pmu_max) pmu_max = pmu;
        if (timer < timer_min) timer_min = timer;
        if (timer > timer_max) timer_max = timer;
    }

    printf("OVERHEAD,%s,reps,%d,pmu_cycles_avg,%.2f,pmu_min,%" PRIu64
           ",pmu_max,%" PRIu64 ",cntvct_avg,%.2f,cntvct_min,%" PRIu64
           ",cntvct_max,%" PRIu64 "\n", name, repetitions,
           (double)pmu_sum / repetitions, pmu_min, pmu_max,
           (double)timer_sum / repetitions, timer_min, timer_max);
}

int main(int argc, char **argv)
{
    int repetitions = argc > 1 ? atoi(argv[1]) : 5000;
    if (repetitions <= 0 || repetitions > PMUV3_MAX_REGIONS)
        return 1;
    if (pmuv3_bundle_init(4) != 0)
        return 1;

    run_case("empty_section", empty_section, repetitions);
    run_case("tiny_branch_section", tiny_branch_section, repetitions);
    run_case("branch64_section", branch64_section, repetitions);
    return shutdown_resources() != 0;
}

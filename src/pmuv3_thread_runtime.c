/*
 * Thread-aware PMUv3 runtime used by the existing bundle instrumentation API.
 */

#include "pmuv3_plugin_bundle.h"
#include "pmuv3_thread_runtime.h"

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

struct PMUv3ThreadContext {
    struct CountData local_counts;
    struct CountData *counts;
    struct PMUv3_Bundle_Data *records;
    unsigned char *started;
    unsigned char *completed;
    uint64_t next_index;
    uint64_t max_used;
    pid_t tid;
    int thread_index;
    bool is_main;
    bool owns_records;
    int debug_direct_printed[MAX_EVENTS];
    int debug_fallback_printed[MAX_EVENTS];
    struct PMUv3ThreadContext *next;
};

struct PMUv3Runtime {
    pthread_mutex_t lock;
    struct PMUv3ThreadContext *contexts;
    struct PMUv3ThreadContext *main_context;
    unsigned active_workers;
    bool initialized;
};

static struct PMUv3Runtime runtime = {
    .lock = PTHREAD_MUTEX_INITIALIZER,
};
static _Thread_local struct PMUv3ThreadContext *current_thread;
static char *owned_contexts[PMUV3_MAX_REGIONS];

int tests_failed;
int tests_verbose;
struct PerfData *perf_data;
struct CountData count_data;
struct perf_thread_map *global_threads;
struct PMUv3_Bundle_Data event_counts[PMUV3_MAX_REGIONS];
uint64_t global_index;

static pid_t current_tid(void)
{
    return (pid_t)syscall(SYS_gettid);
}

static int thread_index_from_tid(const struct perf_thread_map *threads, pid_t tid)
{
    int count = perf_thread_map__nr((struct perf_thread_map *)threads);
    for (int i = 0; i < count; i++) {
        if (perf_thread_map__pid((struct perf_thread_map *)threads, i) == tid)
            return i;
    }
    return -1;
}

static struct perf_thread_map *build_process_thread_map(void)
{
    DIR *directory = opendir("/proc/self/task");
    if (directory == NULL)
        return NULL;

    size_t capacity = 16;
    size_t count = 0;
    pid_t *tids = malloc(capacity * sizeof(*tids));
    if (tids == NULL) {
        closedir(directory);
        return NULL;
    }

    struct dirent *entry;
    while ((entry = readdir(directory)) != NULL) {
        if (!isdigit((unsigned char)entry->d_name[0]))
            continue;
        if (count == capacity) {
            capacity *= 2;
            pid_t *updated = realloc(tids, capacity * sizeof(*tids));
            if (updated == NULL) {
                free(tids);
                closedir(directory);
                return NULL;
            }
            tids = updated;
        }
        tids[count++] = (pid_t)strtol(entry->d_name, NULL, 10);
    }
    closedir(directory);

    struct perf_thread_map *threads =
        perf_thread_map__new_array((int)count, tids);
    free(tids);
    return threads;
}

static uint32_t raw_pmu_type(void)
{
#if !defined(__aarch64__)
    return PERF_TYPE_RAW;
#else
    const char *devices_path = "/sys/bus/event_source/devices";
    DIR *devices = opendir(devices_path);
    if (devices == NULL)
        return PERF_TYPE_RAW;

    uint32_t type = PERF_TYPE_RAW;
    struct dirent *entry;
    while ((entry = readdir(devices)) != NULL) {
        if (strncmp(entry->d_name, "armv8_pmuv3", 11) != 0)
            continue;
        char path[512];
        snprintf(path, sizeof(path), "%s/%s/type", devices_path, entry->d_name);
        FILE *type_file = fopen(path, "r");
        if (type_file == NULL)
            continue;
        if (fscanf(type_file, "%" SCNu32, &type) != 1)
            type = PERF_TYPE_RAW;
        fclose(type_file);
        break;
    }
    closedir(devices);
    return type;
#endif
}

static int pmuv3_read_direct(struct perf_event_mmap_page *pc, uint64_t *value)
{
#if !defined(__aarch64__)
    (void)pc;
    (void)value;
    return -1;
#else
    if (getenv("PMUV3_FORCE_FALLBACK") != NULL)
        return -1;
    if (pc == NULL || pc->cap_user_rdpmc == 0)
        return -1;

    uint32_t seq;
    uint32_t index;
    uint64_t offset;
    uint16_t width;
    uint64_t reg_value = 0;

    do {
        seq = pc->lock;
        __atomic_thread_fence(__ATOMIC_ACQUIRE);
        index = pc->index;
        offset = pc->offset;
        width = pc->pmc_width;
        if (index == 0)
            return -1;

        uint32_t pmc_index = index - 1;
        if (pmc_index == 31) {
            asm volatile("mrs %0, pmccntr_el0" : "=r"(reg_value));
        } else {
            asm volatile("msr pmselr_el0, %0" : : "r"((uint64_t)pmc_index));
            asm volatile("isb");
            asm volatile("mrs %0, pmxevcntr_el0" : "=r"(reg_value));
        }
        __atomic_thread_fence(__ATOMIC_ACQUIRE);
    } while (pc->lock != seq);

    int64_t signed_count = (int64_t)reg_value;
    if (width > 0 && width < 64) {
        signed_count <<= (64 - width);
        signed_count >>= (64 - width);
    }
    *value = (uint64_t)((int64_t)offset + signed_count);
    return 0;
#endif
}

static struct perf_event_mmap_page *thread_event_page(
    const struct PMUv3ThreadContext *thread, uint64_t event_index)
{
    size_t offset = (size_t)thread->thread_index * (size_t)num_events +
                    (size_t)event_index;
    return perf_data->pc[offset];
}

static int read_event(struct PMUv3ThreadContext *thread, uint64_t event_index,
                      struct perf_counts_values *count)
{
    uint64_t value;
    const char *debug = getenv("PMUV3_DEBUG_READ_PATH");
    if (thread == NULL || perf_data == NULL || count == NULL ||
        event_index >= num_events)
        return -1;

    if (pmuv3_read_direct(thread_event_page(thread, event_index), &value) == 0) {
        if (debug != NULL && debug[0] != '\0' &&
            thread->debug_direct_printed[event_index] == 0) {
            fprintf(stderr,
                    "PMUv3: tid %d event %" PRIu64 " using direct EL0 PMU read path\n",
                    thread->tid, event_index);
            thread->debug_direct_printed[event_index] = 1;
        }
        count->val = value;
        return 0;
    }

    if (debug != NULL && debug[0] != '\0' &&
        thread->debug_fallback_printed[event_index] == 0) {
        fprintf(stderr,
                "PMUv3: tid %d event %" PRIu64 " using perf_evsel__read fallback path\n",
                thread->tid, event_index);
        thread->debug_fallback_printed[event_index] = 1;
    }
    return perf_evsel__read(perf_data->global_evsel[event_index], 0,
                            thread->thread_index, count);
}

static void free_thread_context(struct PMUv3ThreadContext *thread)
{
    if (thread == NULL)
        return;
    if (thread->owns_records)
        free(thread->records);
    free(thread->started);
    free(thread->completed);
    free(thread);
}

static struct PMUv3ThreadContext *create_thread_context(bool is_main)
{
    if (perf_data == NULL || perf_data->threads == NULL)
        return NULL;

    struct PMUv3ThreadContext *thread = calloc(1, sizeof(*thread));
    if (thread == NULL)
        return NULL;
    thread->tid = current_tid();
    thread->thread_index = thread_index_from_tid(perf_data->threads, thread->tid);
    if (thread->thread_index < 0) {
        free(thread);
        return NULL;
    }

    thread->is_main = is_main;
    thread->counts = is_main ? &count_data : &thread->local_counts;
    thread->records = is_main ? event_counts :
        calloc(PMUV3_MAX_REGIONS, sizeof(*thread->records));
    thread->owns_records = !is_main;
    thread->started = calloc(PMUV3_MAX_REGIONS, sizeof(*thread->started));
    thread->completed = calloc(PMUV3_MAX_REGIONS, sizeof(*thread->completed));
    if (thread->records == NULL || thread->started == NULL ||
        thread->completed == NULL) {
        free_thread_context(thread);
        return NULL;
    }
    return thread;
}

static void close_perf_resources(void)
{
    if (perf_data == NULL)
        return;
    for (uint64_t i = 0; i < num_events; i++) {
        if (perf_data->global_evsel[i] != NULL) {
            perf_evsel__munmap(perf_data->global_evsel[i]);
            perf_evsel__close(perf_data->global_evsel[i]);
            perf_evsel__delete(perf_data->global_evsel[i]);
        }
    }
    if (perf_data->threads != NULL)
        perf_thread_map__put(perf_data->threads);
    free(perf_data->pc);
    free(perf_data);
    perf_data = NULL;
    global_threads = NULL;
}

static int open_process_events(const uint32_t events[])
{
    bool include_kernel = getenv("PMUV3_INCLUDE_KERNEL") != NULL;
    bool disable_user_access = getenv("PMUV3_DISABLE_USER_ACCESS") != NULL;
    uint32_t pmu_type = raw_pmu_type();
    struct perf_thread_map *threads = build_process_thread_map();
    if (threads == NULL)
        return -1;

    perf_data = calloc(1, sizeof(*perf_data));
    if (perf_data == NULL) {
        perf_thread_map__put(threads);
        return -1;
    }
    perf_data->threads = threads;
    perf_data->nthreads = perf_thread_map__nr(threads);
    global_threads = threads;

    for (uint64_t i = 0; i < num_events; i++) {
        struct perf_event_attr attr = {
            .size = sizeof(attr),
            .type = pmu_type,
            .config = (uint64_t)events[i],
#if defined(__aarch64__)
            .config1 = disable_user_access ? 0 : 0x2,
#endif
            .exclude_user = 0,
            .exclude_kernel = include_kernel ? 0 : 1,
            .exclude_hv = include_kernel ? 0 : 1,
            .disabled = 0,
        };
        struct perf_evsel *evsel = perf_evsel__new(&attr);
        if (evsel == NULL)
            goto fail;
        perf_data->global_evsel[i] = evsel;
        if (perf_evsel__open(evsel, NULL, threads) != 0)
            goto fail;
        if (perf_evsel__mmap(evsel, 0) != 0)
            goto fail;
    }

    size_t page_count = (size_t)perf_data->nthreads * (size_t)num_events;
    perf_data->pc = calloc(page_count, sizeof(*perf_data->pc));
    if (perf_data->pc == NULL)
        goto fail;
    for (int thread = 0; thread < perf_data->nthreads; thread++) {
        for (uint64_t event = 0; event < num_events; event++) {
            size_t offset = (size_t)thread * (size_t)num_events + event;
            perf_data->pc[offset] = perf_evsel__mmap_base(
                perf_data->global_evsel[event], 0, thread);
            if (perf_data->pc[offset] == NULL)
                goto fail;
        }
    }

    if (getenv("PMUV3_FORCE_SYSCALL") != NULL) {
        for (uint64_t event = 0; event < num_events; event++)
            perf_evsel__munmap(perf_data->global_evsel[event]);
        memset(perf_data->pc, 0, page_count * sizeof(*perf_data->pc));
    }
    return 0;

fail:
    fprintf(stderr, "PMUv3: failed to open process thread counters: %s\n",
            strerror(errno));
    close_perf_resources();
    return -1;
}

static int capture_counts(bool start, const char *context, uint64_t index)
{
    struct PMUv3ThreadContext *thread = current_thread;
    if (thread == NULL || index >= PMUV3_MAX_REGIONS)
        return -1;

    struct PMUv3_Bundle_Data *record = &thread->records[index];
    if (context != NULL)
        record->context = context;
    else if (record->context == NULL)
        record->context = "single_region";

    for (uint64_t i = 0; i < num_events; i++) {
        struct perf_counts_values *count = &thread->counts->global_count[i];
        if (read_event(thread, i, count) != 0)
            return -1;
        if (start)
            record->start_cnt[i] = count->val;
        else
            record->end_cnt[i] = count->val;
    }

    if (start)
        thread->started[index] = 1;
    else
        thread->completed[index] = thread->started[index];
    if (thread->max_used <= index)
        thread->max_used = index + 1;
    if (thread->next_index <= index)
        thread->next_index = index + 1;
    if (thread->is_main && global_index < thread->next_index)
        global_index = thread->next_index;
    return 0;
}

int custom_print(enum libperf_print_level level, const char *fmt, va_list ap)
{
    (void)level;
    return vfprintf(stderr, fmt, ap);
}

int pmuv3_runtime_init(const uint32_t events[])
{
    pthread_mutex_lock(&runtime.lock);
    if (runtime.initialized) {
        pthread_mutex_unlock(&runtime.lock);
        return -1;
    }
    runtime.initialized = true;
    pthread_mutex_unlock(&runtime.lock);

    for (uint64_t i = 0; i < PMUV3_MAX_REGIONS; i++) {
        free(owned_contexts[i]);
        owned_contexts[i] = NULL;
    }
    memset(event_counts, 0, sizeof(event_counts));
    memset(&count_data, 0, sizeof(count_data));
    global_index = 0;
    if (open_process_events(events) != 0)
        goto fail;

    struct PMUv3ThreadContext *thread = create_thread_context(true);
    if (thread == NULL)
        goto fail;
    runtime.contexts = thread;
    runtime.main_context = thread;
    current_thread = thread;
    return 0;

fail:
    close_perf_resources();
    pthread_mutex_lock(&runtime.lock);
    runtime.initialized = false;
    pthread_mutex_unlock(&runtime.lock);
    return -1;
}

int pmu_counter_read(const uint32_t events[])
{
    return pmuv3_runtime_init(events);
}

int init_api(int argc, char **argv, const uint32_t event_vals[])
{
    (void)argc;
    (void)argv;
    libperf_init(custom_print);
    return pmu_counter_read(event_vals);
}

int pmuv3_register_current_thread(void)
{
    if (current_thread != NULL)
        return 0;

    pthread_mutex_lock(&runtime.lock);
    bool initialized = runtime.initialized;
    pthread_mutex_unlock(&runtime.lock);
    if (!initialized)
        return -1;

    struct PMUv3ThreadContext *thread = create_thread_context(false);
    if (thread == NULL)
        return -1;
    pthread_mutex_lock(&runtime.lock);
    thread->next = runtime.contexts;
    runtime.contexts = thread;
    runtime.active_workers++;
    pthread_mutex_unlock(&runtime.lock);
    current_thread = thread;
    return 0;
}

int pmuv3_unregister_current_thread(void)
{
    struct PMUv3ThreadContext *thread = current_thread;
    if (thread == NULL || thread->is_main)
        return -1;

    int status = 0;
    pthread_mutex_lock(&runtime.lock);
    for (uint64_t i = 0; i < thread->max_used; i++) {
        if (thread->started[i] && !thread->completed[i]) {
            status = -1;
            continue;
        }
        if (!thread->completed[i])
            continue;
        if (global_index >= PMUV3_MAX_REGIONS) {
            status = -1;
            break;
        }
        event_counts[global_index] = thread->records[i];
        owned_contexts[global_index] = strdup(
            thread->records[i].context != NULL ?
            thread->records[i].context : "unnamed_region");
        if (owned_contexts[global_index] == NULL) {
            status = -1;
            event_counts[global_index].context = "unnamed_region";
        } else {
            event_counts[global_index].context = owned_contexts[global_index];
        }
        global_index++;
    }

    struct PMUv3ThreadContext **link = &runtime.contexts;
    while (*link != NULL && *link != thread)
        link = &(*link)->next;
    if (*link == thread)
        *link = thread->next;
    if (runtime.active_workers > 0)
        runtime.active_workers--;
    pthread_mutex_unlock(&runtime.lock);

    current_thread = NULL;
    free_thread_context(thread);
    return status;
}

uint64_t get_next_index(void)
{
    struct PMUv3ThreadContext *thread = current_thread;
    if (thread == NULL || thread->next_index >= PMUV3_MAX_REGIONS)
        return UINT64_MAX;
    uint64_t index = thread->next_index++;
    if (thread->is_main)
        global_index = thread->next_index;
    return index;
}

uint64_t process_start_count(struct CountData *counts)
{
    (void)counts;
    return capture_counts(true, "single_region", 0) == 0 ? 0 : UINT64_MAX;
}

uint64_t process_end_count(struct CountData *counts)
{
    (void)counts;
    return capture_counts(false, "single_region", 0) == 0 ? 0 : UINT64_MAX;
}

uint64_t get_start_count(struct CountData *counts, const char *context,
                         uint64_t index)
{
    (void)counts;
    return capture_counts(true, context, index) == 0 ? 0 : UINT64_MAX;
}

uint64_t get_end_count(struct CountData *counts, const char *context,
                       uint64_t index)
{
    (void)counts;
    return capture_counts(false, context, index) == 0 ? 0 : UINT64_MAX;
}

int shutdown_resources(void)
{
    pthread_mutex_lock(&runtime.lock);
    if (!runtime.initialized) {
        pthread_mutex_unlock(&runtime.lock);
        return -1;
    }
    if (runtime.active_workers != 0) {
        fprintf(stderr, "PMUv3: %u worker threads are still registered\n",
                runtime.active_workers);
        pthread_mutex_unlock(&runtime.lock);
        return -1;
    }

    struct PMUv3ThreadContext *main_context = runtime.main_context;
    runtime.contexts = NULL;
    runtime.main_context = NULL;
    runtime.initialized = false;
    pthread_mutex_unlock(&runtime.lock);

    if (current_thread == main_context)
        current_thread = NULL;
    free_thread_context(main_context);
    close_perf_resources();
    free_bundle_memory();
    for (uint64_t i = 0; i < PMUV3_MAX_REGIONS; i++) {
        free(owned_contexts[i]);
        owned_contexts[i] = NULL;
    }
    return 0;
}

# Scenarios

This guide shows the common PMUv3 Plugin instrumentation patterns. Pick the
scenario that matches the codebase and the number of code regions you want to
measure.

All examples use `cur_bundle_no` as the bundle selected by the user at runtime.
The context string, such as `"CONTEXT_1"`, is written to the CSV output and
should describe the measured region.

## Common Setup

Choose the bundle at runtime and initialize it once before entering the
instrumented regions:

```c
int cur_bundle_no = atoi(argv[1]);

pmuv3_bundle_init(cur_bundle_no);
```

Call the matching post-processing API after the measured region or regions, then
call `shutdown_resources()` once.

The CSV output is written as `bundle<N>.csv` in the directory where the
instrumented program runs.

## Scenario I: Different Code Blocks In A C++ Codebase

Use this when a C++ program has multiple functions, phases, or nested regions to
measure in one run.

Include the plugin headers:

```cpp
#include "pmuv3_plugin_bundle.h"
#include "pmuv3_processing.hpp"
```

Initialize once:

```cpp
int cur_bundle_no = atoi(argv[1]);
pmuv3_bundle_init(cur_bundle_no);
```

Use a unique index for every instrumented region. The same context and index
used in `get_start_count()` must be passed to the matching `get_end_count()`.

```cpp
uint64_t local_1 = get_next_index();
get_start_count(&count_data, "CONTEXT_1", local_1);

/* first code block to be instrumented */

get_end_count(&count_data, "CONTEXT_1", local_1);

uint64_t local_2 = get_next_index();
get_start_count(&count_data, "CONTEXT_2", local_2);

/* second code block to be instrumented */

get_end_count(&count_data, "CONTEXT_2", local_2);
```

After all regions:

```cpp
process_data(cur_bundle_no);
shutdown_resources();
```

## Scenario II: Single Code Block In A C++ Codebase

Use this when a C++ program has only one region to measure.

```cpp
#include "pmuv3_plugin_bundle.h"
#include "pmuv3_processing.hpp"
```

Initialize once:

```cpp
int cur_bundle_no = atoi(argv[1]);
pmuv3_bundle_init(cur_bundle_no);
```

Instrument the single region:

```cpp
process_start_count(&count_data);

/* code block to be instrumented */

process_end_count(&count_data);
```

After instrumentation:

```cpp
process_single_chunk(cur_bundle_no);
shutdown_resources();
```

## Scenario III: Different Code Blocks In A C Codebase

Use this when a C program has multiple functions, phases, or regions to measure.

Include the plugin headers directly:

```c
#include "pmuv3_plugin_bundle.h"
#include "pmuv3_processing.h"
```

Initialize once:

```c
int cur_bundle_no = atoi(argv[1]);
pmuv3_bundle_init(cur_bundle_no);
```

Instrument each region with a unique index:

```c
uint64_t local_1 = get_next_index();
get_start_count(&count_data, "CONTEXT_1", local_1);

/* first code block to be instrumented */

get_end_count(&count_data, "CONTEXT_1", local_1);

uint64_t local_2 = get_next_index();
get_start_count(&count_data, "CONTEXT_2", local_2);

/* second code block to be instrumented */

get_end_count(&count_data, "CONTEXT_2", local_2);
```

After all regions:

```c
process_data(cur_bundle_no);
shutdown_resources();
```

## Scenario IV: Single Code Block In A C Codebase

Use this when a C program has only one region to measure.

```c
#include "pmuv3_plugin_bundle.h"
#include "pmuv3_processing.h"
```

Initialize once:

```c
int cur_bundle_no = atoi(argv[1]);
pmuv3_bundle_init(cur_bundle_no);
```

Instrument the single region:

```c
process_start_count(&count_data);

/* code block to be instrumented */

process_end_count(&count_data);
```

After instrumentation:

```c
process_single_chunk(cur_bundle_no);
shutdown_resources();
```

## Notes

- Use `get_next_index()` before every separate `get_start_count()` call.
- Keep each start/end pair matched with the same context string and index.
- Use a meaningful context string, such as `"PARSER"` or `"LOOKUP_PHASE"`, so
  the CSV is understandable.
- Set `PMUV3_DEBUG_READ_PATH=1` while validating if you want to see whether the
  plugin used direct EL0 PMU reads or the `perf_evsel__read()` fallback path.

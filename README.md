# PMUv3 Plugin

PMUv3 Plugin is a small C/C++ instrumentation library for reading Arm PMUv3
performance counters around selected regions of application code. It is useful
when whole-process profiling is too coarse and a codebase needs function-level
or phase-level PMU measurements.

The repository also contains CSV post-processing helpers and runnable tests that
show how to instrument one or more code regions. This README is the maintained
public guide. See `scenarios/README.md` for copy-paste instrumentation patterns
for C, C++, single-region, multi-region, and multithreaded programs.

## Repository Layout

| Path | Purpose |
| --- | --- |
| `include/` | Public plugin headers. |
| `src/` | Plugin and CSV post-processing implementations. |
| `tests/` | Validation programs and small C/C++ instrumentation samples. |
| `scenarios/` | Detailed instrumentation scenarios and API call sequences. |
| `Makefile` | Main build entry point. |

## Requirements

- AArch64 Linux system with PMUv3 support.
- GCC or Clang toolchain.
- Linux kernel source tree containing `tools/lib/perf` and `tools/lib/api`.
- Development packages needed by Linux perf libraries, typically:
  - `libelf-dev`
  - `zlib1g-dev`
  - `make`
  - `gcc`
  - `g++`

The plugin uses libperf from the Linux source tree. Provide the tree explicitly
with `LINUX_SRC=/path/to/linux`, or place it in one of the locations searched by
the Makefile:

- `./linux-source`
- `/usr/src/linux-source-<version>`
- `/usr/src/linux`

If none of these paths exists and `LINUX_SRC` is not set, `make` clones the
Linux source tree into `./linux-source`. Override the repository with
`LINUX_GIT=<url>` if needed.

## Build

```bash
make LINUX_SRC=/path/to/linux
```

This builds:

- `out/libpmuv3_plugin.a`
- `out/libpmuv3_plugin_bundle.a`
- `out/libpmuv3_plugin_bundle.so`
- `out/basic_bundle_test`
- `out/c_single_region_test`
- `out/c_multi_region_test`
- `out/c_multithread_test`
- `out/cpp_single_region_test`
- `out/cpp_multi_region_test`
- `out/cpp_multithread_test`
- `out/cpp_volatile_cycle_test`
- `out/measurement_overhead_test`

You can also build only a subset:

```bash
make LINUX_SRC=/path/to/linux libs
make LINUX_SRC=/path/to/linux tests
```

`build.sh` is retained as a convenience wrapper around `make`:

```bash
./build.sh LINUX_SRC=/path/to/linux
```

## Use From Another Project

To instrument an external C or C++ application, add the PMUv3 Plugin headers to
that application's compile flags and link the PMUv3 bundle library with libperf,
libapi, and their system dependencies.

Example variables:

```make
PMUV3_DIR := /path/to/PMUv3_plugin
LINUX_SRC := /path/to/linux

CFLAGS   += -I$(PMUV3_DIR)/include \
            -I$(LINUX_SRC)/tools/lib/perf/include
CXXFLAGS += -I$(PMUV3_DIR)/include \
            -I$(LINUX_SRC)/tools/lib/perf/include
LDLIBS   += $(PMUV3_DIR)/out/libpmuv3_plugin_bundle.a \
            $(LINUX_SRC)/tools/lib/perf/libperf.a \
            $(LINUX_SRC)/tools/lib/api/libapi.a \
            -lelf -lz -lpthread
```

Then include the plugin headers in the source files that contain the
instrumented regions:

```c
#include "pmuv3_plugin_bundle.h"
#include "pmuv3_processing.h"
```

For C++ post-processing helpers, include:

```cpp
#include "pmuv3_plugin_bundle.h"
#include "pmuv3_processing.hpp"
```

The test programs in `tests/` are also buildable examples of the required
include and link flags.

## Direct PMU Read Setup

On AArch64, the bundle API requests userspace PMU access and reads counters
through the perf mmap page with `mrs pmccntr_el0` / `mrs pmxevcntr_el0` when
the kernel permits it. If direct access is unavailable, the plugin falls back to
`perf_evsel__read()`.

Common Linux setup:

```bash
sudo sysctl kernel.perf_event_paranoid=1
sudo sysctl kernel.perf_user_access=1
```

Some kernels may not expose `kernel.perf_user_access`; in that case the plugin
will use the fallback path.

To verify the read path during testing:

```bash
PMUV3_DEBUG_READ_PATH=1 ./out/c_single_region_test 4 1000000
```

On AArch64, the plugin discovers the PMU device type from
`/sys/bus/event_source/devices/armv8_pmuv3*/type`. Raw events must use this
device type; `PERF_TYPE_RAW` is only a fallback when no Arm PMU device can be
found.

Validation-only read modes are also available:

```bash
PMUV3_FORCE_FALLBACK=1 ./out/measurement_overhead_test
PMUV3_FORCE_SYSCALL=1 ./out/measurement_overhead_test
```

The first bypasses the plugin's direct helper but permits libperf's mmap read.
The second unmaps the perf pages and forces actual file-descriptor reads.

## Instrumenting A Code Region

Include the bundle API and optional post-processing API:

```c
#include "pmuv3_plugin_bundle.h"
#include "pmuv3_processing.h"
```

Initialize a bundle, mark the region, then post-process:

```c
int cur_bundle_no = atoi(argv[1]);

pmuv3_bundle_init(cur_bundle_no);

uint64_t idx = get_next_index();
get_start_count(&count_data, "work_region", idx);
work_region();
get_end_count(&count_data, "work_region", idx);

process_data(cur_bundle_no);
shutdown_resources();
```

The CSV output is written as `bundle<N>.csv` in the current directory. For
full examples, use `scenarios/README.md`; it shows the exact API sequence for:

- C++ multi-region instrumentation.
- C++ single-region instrumentation.
- C multi-region instrumentation.
- C single-region instrumentation.
- C or C++ multithreaded instrumentation.

## Multithreaded Programs

Multithreaded programs keep the same start/end APIs. The only additional calls
are `pmuv3_register_current_thread()` and
`pmuv3_unregister_current_thread()`.

The initialization order matters. PMUv3 opens counters centrally for a
snapshot of the process threads, so workers must exist before
`pmuv3_bundle_init()` and remain blocked until initialization finishes:

1. Create worker threads and block them at an application barrier.
2. Call `pmuv3_bundle_init()` from the main thread.
3. Release the workers.
4. Each worker registers once, measures one or more regions, and unregisters.
5. Join every worker.
6. Call `process_data()` and `shutdown_resources()` from the main thread.

Worker instrumentation remains unchanged:

```c
pmuv3_register_current_thread();

uint64_t idx = get_next_index();
get_start_count(&count_data, "worker_region", idx);
worker_region();
get_end_count(&count_data, "worker_region", idx);

pmuv3_unregister_current_thread();
```

Registration performs the TID lookup and allocates per-thread result storage.
The start/end path performs no allocation, lock, TID lookup, or registration.
Threads created after `pmuv3_bundle_init()` are not part of the PMU thread map
and cannot register in that session.

## Tests

```bash
make LINUX_SRC=/path/to/linux tests
./out/basic_bundle_test 4
./out/c_single_region_test 4 100000000
./out/c_multi_region_test 4 50000000
./out/cpp_single_region_test 4 50000000
./out/cpp_multi_region_test 4 50000000
./out/c_multithread_test 4 4 10000000
./out/cpp_multithread_test 4 4 10000000
./out/cpp_volatile_cycle_test 4 500000000
./out/measurement_overhead_test 5000
```

The first command is a smoke test. The remaining commands are small
instrumentation samples covering C/C++ single-region, multi-region,
multithreaded, volatile-loop, and measurement-overhead usage. See
`tests/README.md` for details.

## Event Bundles

The bundle API currently provides **19 bundles**, numbered `0` through `18`.
The bundle table uses raw PMU event encodings. This is intentional because raw
encodings allow custom, implementation-defined, or hidden events that may not
have portable perf event names.

Portability caveat: raw event encodings can differ by CPU generation. Treat the
current bundles as Arm PMUv3/Neoverse-oriented examples and verify event
availability on the target CPU before publishing cross-platform conclusions.

The source of truth for bundle contents is `src/pmuv3_plugin_bundle.c`.

| Bundle | Focus | Events |
| --- | --- | --- |
| `0` | D-side TLB and page walks | `CPU_CYCLES`, `L1D_TLB_REFILL`, `L1D_TLB`, `L2D_TLB_REFILL`, `L2D_TLB`, `DTLB_WALK` |
| `1` | L2 TLB read/write behavior | `CPU_CYCLES`, `L2D_TLB_REFILL_RD`, `L2D_TLB_REFILL_WR`, `L2D_TLB_RD`, `L2D_TLB_WR` |
| `2` | Memory and bus access | `CPU_CYCLES`, `MEM_ACCESS`, `BUS_ACCESS`, `MEMORY_ERROR` |
| `3` | Branch prediction | `CPU_CYCLES`, `BR_MIS_PRED`, `BR_PRED`, `BR_RETIRED`, `BR_MIS_PRED_RETIRED`, `BR_IMMED_SPEC`, `BR_INDIRECT_SPEC` |
| `4` | Stall cycles and stall slots | `CPU_CYCLES`, `STALL_FRONTEND`, `STALL_BACKEND`, `STALL_SLOT_FRONTEND`, `STALL_SLOT_BACKEND`, `STALL_BACKEND_MEM` |
| `5` | L1 instruction cache | `CPU_CYCLES`, `L1I_CACHE_REFILL`, `L1I_CACHE` |
| `6` | L1/L2/L3 data cache | `CPU_CYCLES`, `L1D_CACHE_REFILL`, `L1D_CACHE`, `L2D_CACHE`, `L2D_CACHE_REFILL`, `L3D_CACHE_REFILL`, `L3D_CACHE` |
| `7` | Instruction TLB | `CPU_CYCLES`, `L1I_TLB_REFILL`, `L1I_TLB`, `ITLB_WALK` |
| `8` | Retired/speculative instruction mix | `CPU_CYCLES`, `INST_RETIRED`, `INST_SPEC`, `EXC_TAKEN`, `ST_SPEC`, `ASE_SPEC`, `PC_WRITE_SPEC` |
| `9` | Branch/load/barrier instruction mix | `CPU_CYCLES`, `BR_RETURN_SPEC`, `BR_IMMED_SPEC`, `BR_INDIRECT_SPEC`, `INST_SPEC`, `LD_SPEC`, `DSB_SPEC` |
| `10` | L1D TLB read/write behavior | `CPU_CYCLES`, `L1D_TLB_REFILL_RD`, `L1D_TLB_REFILL_WR`, `L1D_TLB_RD`, `L1D_TLB_WR` |
| `11` | MPKI-oriented cache/ITLB view | `CPU_CYCLES`, `INST_RETIRED`, `LL_CACHE_MISS_RD`, `L1D_CACHE_REFILL`, `ITLB_WALK`, `L1I_CACHE_REFILL` |
| `12` | MPKI-oriented DTLB/L2/branch view | `CPU_CYCLES`, `INST_RETIRED`, `DTLB_WALK`, `BR_MIS_PRED_RETIRED`, `L2D_CACHE_REFILL` |
| `13` | L1D read/write miss rates | `CPU_CYCLES`, `L1D_CACHE_REFILL_OUTER`, `L1D_CACHE_REFILL`, `L1D_CACHE_REFILL_RD`, `L1D_CACHE_RD`, `L1D_CACHE_REFILL_WR`, `L1D_CACHE_WR` |
| `14` | Speculative operation mix | `CPU_CYCLES`, `CRYPTO_SPEC`, `ISB_SPEC`, `DP_SPEC`, `DMB_SPEC`, `VFP_SPEC`, `INST_SPEC` |
| `15` | SVE activity | `CPU_CYCLES`, `SVE_INST_SPEC`, `SVE_PRED_SPEC`, `SVE_PRED_EMPTY_SPEC`, `SVE_PRED_FULL_SPEC`, `SVE_PRED_PARTIAL_SPEC` |
| `16` | Bus and memory read/write split | `CPU_CYCLES`, `BUS_ACCESS_RD`, `BUS_ACCESS_WR`, `MEM_ACCESS_RD`, `MEM_ACCESS_WR` |
| `17` | Last-level cache reads and FP operation class | `CPU_CYCLES`, `LL_CACHE_RD`, `LL_CACHE_MISS_RD`, `FP_SCALE_OPS_SPEC`, `FP_FIXED_OPS_SPEC` |
| `18` | Topdown-style slot metrics | `OP_RETIRED`, `OP_SPEC`, `STALL_SLOT`, `CPU_CYCLES`, `STALL_SLOT_FRONTEND`, `STALL_SLOT_BACKEND`, `BR_MIS_PRED` |

Bundles `15` through `18` add SVE, bus/memory, LL-cache/FP, and topdown-style
slot events used during Grace and Neoverse V2 validation. Several encodings are
implementation-defined; zero counts can mean the workload did not execute the
operation or that the event is not implemented on the target CPU.

Future work can add richer event descriptors containing perf event type,
config, CPU family, and event descriptions.

## Scenarios

Scenario notes are in `scenarios/README.md`.

## Cleaning

```bash
make clean
make distclean
```

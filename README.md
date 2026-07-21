# PMUv3 Plugin

PMUv3 Plugin is a small C/C++ instrumentation library for reading Arm PMUv3
performance counters around selected regions of application code. It is useful
when whole-process profiling is too coarse and a codebase needs function-level
or phase-level PMU measurements.

The repository also contains CSV post-processing helpers and runnable tests that
show how to instrument one or more code regions.

Detailed historical documentation is still available in
`Documentation-PMUV3_plugin.pdf`. The sections below are the current build and
usage guide.

## Repository Layout

| Path | Purpose |
| --- | --- |
| `include/` | Public plugin headers. |
| `src/` | Plugin and CSV post-processing implementations. |
| `tests/` | Validation programs and small C/C++ instrumentation samples. |
| `scenarios/` | Scenario-oriented usage notes. |
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
- `out/cpp_single_region_test`
- `out/cpp_multi_region_test`

You can also build only a subset:

```bash
make LINUX_SRC=/path/to/linux libs
make LINUX_SRC=/path/to/linux tests
```

`build.sh` is retained as a convenience wrapper around `make`:

```bash
./build.sh LINUX_SRC=/path/to/linux
```

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

The CSV output is written as `bundle<N>.csv` in the current directory.

## Tests

```bash
make LINUX_SRC=/path/to/linux tests
./out/basic_bundle_test 4
./out/c_single_region_test 4 100000000
./out/c_multi_region_test 4 50000000
./out/cpp_single_region_test 4 50000000
./out/cpp_multi_region_test 4 50000000
```

The first command is a smoke test. The remaining commands are small instrumentation
samples covering C/C++ single-region and multi-region usage. See
`tests/README.md` for details.

## Event Bundles

The current bundle table uses raw PMU event encodings. This is intentional
because raw encodings allow custom, implementation-defined, or hidden events
that may not have portable perf event names.

Portability caveat: raw event encodings can differ by CPU generation. Treat the
current bundles as Arm PMUv3/Neoverse-oriented examples and verify event
availability on the target CPU before publishing cross-platform conclusions.

Future work can add richer event descriptors containing perf event type,
config, CPU family, and descriptions.

## Scenarios

Scenario notes are in `scenarios/README.md`.

## Cleaning

```bash
make clean
make distclean
```

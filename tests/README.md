# Tests

This directory contains buildable validation programs and small instrumentation
samples for the PMUv3 plugin. Keeping them together avoids maintaining a
separate example path that duplicates the tests.

## Programs

### `basic_bundle_test.c`

Initializes a PMUv3 event bundle, measures a simple busy loop, prints the
`CPU_CYCLES` delta, and shuts down the plugin. This is the smallest smoke test.

### `c_single_region_test.c`

Shows the common C usage pattern for measuring one named code region and
generating `bundle<N>.csv` with `process_single_chunk()`.

### `c_multi_region_test.c`

Shows C instrumentation around two different code regions in one process. This
validates `get_next_index()` and multiple CSV rows.

### `cpp_multi_region_test.cpp`

Shows the same multi-region pattern from C++ code using
`pmuv3_processing.hpp`.

### `cpp_single_region_test.cpp`

Shows the C++ single-region path using `pmuv3_processing.hpp` and
`process_single_chunk()`.

### `c_multithread_test.c`

Creates multiple pthread workers before PMUv3 initialization. Every worker
registers, measures one single block and two indexed blocks, unregisters, and
contributes rows to the shared CSV.

### `cpp_multithread_test.cpp`

Validates the same lifecycle using `std::thread` and the unchanged C start/end
instrumentation API.

### `cpp_volatile_cycle_test.cpp`

Measures a 500-million-iteration volatile C++ loop. Together with
`basic_bundle_test.c`, this checks that C and C++ both report approximately the
expected CPU-cycle count.

### `measurement_overhead_test.c`

Measures the start/end cost around empty, tiny-branch, and 64-branch regions.
Run it normally, with `PMUV3_FORCE_FALLBACK=1`, and with
`PMUV3_FORCE_SYSCALL=1` to distinguish direct, libperf mmap, and syscall paths.

Build:

```bash
make LINUX_SRC=/path/to/linux tests
```

Run:

```bash
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

For all programs, the first argument is the bundle number. For the region tests,
the optional second argument is the loop iteration count. Bundle `4` is a
compact cycles/stall bundle and is a convenient smoke test.

Before running on systems with restricted perf access, set:

```bash
sudo sysctl kernel.perf_event_paranoid=1
sudo sysctl kernel.perf_user_access=1
```

If direct userspace PMU access is unavailable, the plugin falls back to
`perf_evsel__read()`.

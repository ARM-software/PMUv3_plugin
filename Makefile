CC ?= gcc
CXX ?= g++
AR ?= ar

BUILD_DIR ?= out
SRC_DIR := src
INC_DIR := include
TEST_DIR := tests

LINUX_SRC_CANDIDATES := linux-source $(wildcard /usr/src/linux-source-*) /usr/src/linux
LINUX_SRC ?= $(firstword $(wildcard $(LINUX_SRC_CANDIDATES)))

ifeq ($(strip $(LINUX_SRC)),)
LINUX_SRC := linux-source
NEED_LINUX_CLONE := 1
endif

LINUX_GIT ?= https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git

PERF_DIR := $(LINUX_SRC)/tools/lib/perf
API_DIR := $(LINUX_SRC)/tools/lib/api
PERF_INC := $(PERF_DIR)/include
LIBPERF_A := $(PERF_DIR)/libperf.a
LIBAPI_A := $(API_DIR)/libapi.a

CPPFLAGS += -I$(INC_DIR) -I$(PERF_INC)
CFLAGS ?= -O2 -g -Wall -Wextra
CXXFLAGS ?= -O2 -g -Wall -Wextra
LDLIBS += $(LIBPERF_A) $(LIBAPI_A) -lelf -lz -lpthread

C_CORE_OBJ := $(BUILD_DIR)/pmuv3_plugin.o
C_BUNDLE_OBJ := $(BUILD_DIR)/pmuv3_plugin_bundle.o
C_THREAD_RUNTIME_OBJ := $(BUILD_DIR)/pmuv3_thread_runtime.o
C_PROCESSING_OBJ := $(BUILD_DIR)/pmuv3_processing.o
CXX_PROCESSING_OBJ := $(BUILD_DIR)/pmuv3_processing_cpp.o

.PHONY: all linux-libs libs tests clean distclean help

all: libs tests

help:
	@echo "Targets:"
	@echo "  make libs                 Build PMUv3 static/shared libraries"
	@echo "  make tests                Build validation and instrumentation samples"
	@echo "  make clean                Remove generated build outputs"
	@echo ""
	@echo "Variables:"
	@echo "  LINUX_SRC=/path/to/linux  Linux source tree containing tools/lib/perf"
	@echo "  BUILD_DIR=out             Build output directory"

linux-libs: $(LIBPERF_A) $(LIBAPI_A)

ifeq ($(NEED_LINUX_CLONE),1)
$(LINUX_SRC)/.git:
	git clone --depth 1 $(LINUX_GIT) $(LINUX_SRC)

$(LIBPERF_A): $(LINUX_SRC)/.git
else
$(LIBPERF_A):
endif
	$(MAKE) -C $(PERF_DIR)

ifeq ($(NEED_LINUX_CLONE),1)
$(LIBAPI_A): $(LINUX_SRC)/.git
else
$(LIBAPI_A):
endif
	$(MAKE) -C $(API_DIR)

libs: $(BUILD_DIR)/libpmuv3_plugin.a \
      $(BUILD_DIR)/libpmuv3_plugin_bundle.a \
      $(BUILD_DIR)/libpmuv3_plugin_bundle.so

tests: $(BUILD_DIR)/basic_bundle_test \
       $(BUILD_DIR)/c_single_region_test \
       $(BUILD_DIR)/c_multi_region_test \
       $(BUILD_DIR)/c_multithread_test \
       $(BUILD_DIR)/cpp_single_region_test \
       $(BUILD_DIR)/cpp_multi_region_test \
       $(BUILD_DIR)/cpp_multithread_test \
       $(BUILD_DIR)/cpp_volatile_cycle_test \
       $(BUILD_DIR)/measurement_overhead_test

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR) linux-libs
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/pmuv3_processing_cpp.o: $(SRC_DIR)/pmuv3_processing.cpp | $(BUILD_DIR) linux-libs
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/libpmuv3_plugin.a: $(C_CORE_OBJ) | $(BUILD_DIR)
	$(AR) rcs $@ $^

$(BUILD_DIR)/libpmuv3_plugin_bundle.a: $(C_BUNDLE_OBJ) $(C_THREAD_RUNTIME_OBJ) $(C_PROCESSING_OBJ) | $(BUILD_DIR)
	$(AR) rcs $@ $^

$(BUILD_DIR)/libpmuv3_plugin_bundle.so: $(SRC_DIR)/pmuv3_plugin_bundle.c $(SRC_DIR)/pmuv3_thread_runtime.c $(SRC_DIR)/pmuv3_processing.c | $(BUILD_DIR) linux-libs
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -c $(SRC_DIR)/pmuv3_plugin_bundle.c -o $(BUILD_DIR)/pmuv3_plugin_bundle.pic.o
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -c $(SRC_DIR)/pmuv3_thread_runtime.c -o $(BUILD_DIR)/pmuv3_thread_runtime.pic.o
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -c $(SRC_DIR)/pmuv3_processing.c -o $(BUILD_DIR)/pmuv3_processing.pic.o
	$(CC) -shared -o $@ $(BUILD_DIR)/pmuv3_plugin_bundle.pic.o $(BUILD_DIR)/pmuv3_thread_runtime.pic.o $(BUILD_DIR)/pmuv3_processing.pic.o $(LDLIBS)

$(BUILD_DIR)/basic_bundle_test: $(TEST_DIR)/basic_bundle_test.c $(C_BUNDLE_OBJ) $(C_THREAD_RUNTIME_OBJ) | $(BUILD_DIR) linux-libs
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(C_BUNDLE_OBJ) $(C_THREAD_RUNTIME_OBJ) $(LDLIBS) -o $@

$(BUILD_DIR)/c_single_region_test: $(TEST_DIR)/c_single_region_test.c $(C_BUNDLE_OBJ) $(C_THREAD_RUNTIME_OBJ) $(C_PROCESSING_OBJ) | $(BUILD_DIR) linux-libs
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(C_BUNDLE_OBJ) $(C_THREAD_RUNTIME_OBJ) $(C_PROCESSING_OBJ) $(LDLIBS) -o $@

$(BUILD_DIR)/c_multi_region_test: $(TEST_DIR)/c_multi_region_test.c $(C_BUNDLE_OBJ) $(C_THREAD_RUNTIME_OBJ) $(C_PROCESSING_OBJ) | $(BUILD_DIR) linux-libs
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(C_BUNDLE_OBJ) $(C_THREAD_RUNTIME_OBJ) $(C_PROCESSING_OBJ) $(LDLIBS) -o $@

$(BUILD_DIR)/c_multithread_test: $(TEST_DIR)/c_multithread_test.c $(C_BUNDLE_OBJ) $(C_THREAD_RUNTIME_OBJ) $(C_PROCESSING_OBJ) | $(BUILD_DIR) linux-libs
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(C_BUNDLE_OBJ) $(C_THREAD_RUNTIME_OBJ) $(C_PROCESSING_OBJ) $(LDLIBS) -o $@

$(BUILD_DIR)/cpp_single_region_test: $(TEST_DIR)/cpp_single_region_test.cpp $(C_BUNDLE_OBJ) $(C_THREAD_RUNTIME_OBJ) $(CXX_PROCESSING_OBJ) | $(BUILD_DIR) linux-libs
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< $(C_BUNDLE_OBJ) $(C_THREAD_RUNTIME_OBJ) $(CXX_PROCESSING_OBJ) $(LDLIBS) -o $@

$(BUILD_DIR)/cpp_multi_region_test: $(TEST_DIR)/cpp_multi_region_test.cpp $(C_BUNDLE_OBJ) $(C_THREAD_RUNTIME_OBJ) $(CXX_PROCESSING_OBJ) | $(BUILD_DIR) linux-libs
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< $(C_BUNDLE_OBJ) $(C_THREAD_RUNTIME_OBJ) $(CXX_PROCESSING_OBJ) $(LDLIBS) -o $@

$(BUILD_DIR)/cpp_multithread_test: $(TEST_DIR)/cpp_multithread_test.cpp $(C_BUNDLE_OBJ) $(C_THREAD_RUNTIME_OBJ) $(CXX_PROCESSING_OBJ) | $(BUILD_DIR) linux-libs
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< $(C_BUNDLE_OBJ) $(C_THREAD_RUNTIME_OBJ) $(CXX_PROCESSING_OBJ) $(LDLIBS) -o $@

$(BUILD_DIR)/cpp_volatile_cycle_test: $(TEST_DIR)/cpp_volatile_cycle_test.cpp $(C_BUNDLE_OBJ) $(C_THREAD_RUNTIME_OBJ) | $(BUILD_DIR) linux-libs
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< $(C_BUNDLE_OBJ) $(C_THREAD_RUNTIME_OBJ) $(LDLIBS) -o $@

$(BUILD_DIR)/measurement_overhead_test: $(TEST_DIR)/measurement_overhead_test.c $(C_BUNDLE_OBJ) $(C_THREAD_RUNTIME_OBJ) | $(BUILD_DIR) linux-libs
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(C_BUNDLE_OBJ) $(C_THREAD_RUNTIME_OBJ) $(LDLIBS) -o $@

clean:
	rm -rf $(BUILD_DIR)

distclean: clean
	rm -f bundle*.csv

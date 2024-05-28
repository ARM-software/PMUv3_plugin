# Compiler
CC = gcc

# Compiler flags
CFLAGS = -g -I/home/ubuntu/ut_integration/linux/tools/lib/perf/include/

# Libraries
LIBS = /home/ubuntu/ut_integration/PMUv3_plugin/libperf.a /home/ubuntu/ut_integration/PMUv3_plugin/libapi.a

# Source files
SRCS = /home/ubuntu/ut_integration/PMUv3_plugin/test.c /home/ubuntu/ut_integration/PMUv3_plugin/pmuv3_plugin_bundle.c

# Object files
OBJS = $(SRCS:.c=.o)

# Output executable
TARGET = test

# Default rule
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove the executable and object files
clean:
	rm -f $(TARGET) $(OBJS)


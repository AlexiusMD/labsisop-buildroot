BUILDROOT_DIR := ../..
COMPILER := $(BUILDROOT_DIR)/output/host/bin/i686-buildroot-linux-gnu-gcc
FLAGS := -Wall -O3 -g -lpthread

all:
	$(COMPILER) -o thread_profiler thread_profiler.c $(FLAGS) -I .
	cp thread_profiler $(BUILDROOT_DIR)/output/target/bin/
	cp profiler_test.sh $(BUILDROOT_DIR)/output/target/bin/
clean:
	rm thread_profiler

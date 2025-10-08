BUILDROOT_DIR := ../..
COMPILER := $(BUILDROOT_DIR)/output/host/bin/i686-buildroot-linux-gnu-gcc

all:
	$(COMPILER) -o setpriority setpriority.c -lpthread -Wall
	$(COMPILER) -o deadline deadline.c -lpthread -Wall
	cp setpriority $(BUILDROOT_DIR)/output/target/bin/

clean:
	rm setpriority deadline

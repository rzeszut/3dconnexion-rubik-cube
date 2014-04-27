# This makefile is a wrapper for cmake-generated makefile.
# Run `./configure` before using it.

RM = rm -rf

all:
		@ (mkdir -p build && cd build)
		@ $(MAKE) -C build

clean:
		@- $(MAKE) -C build clean || true
		@- $(RM) ./build/*

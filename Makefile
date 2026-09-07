CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude

BUILD_DIR = build

.PHONY: all cpu cache trace clean help

all: cpu cache trace

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# RISC-V CPU Simulator
# main.cpp includes Executor.cpp, which includes DataControl.cpp.
cpu: | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) src/main.cpp -o $(BUILD_DIR)/riscv_cpu

# Cache Simulator
cache: | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) src/CacheController.cpp -o $(BUILD_DIR)/cache_simulator

# Trace Generator
trace: | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) src/TraceGenerator.cpp -o $(BUILD_DIR)/trace_generator

clean:
	rm -rf $(BUILD_DIR)

help:
	@echo "Available targets:"
	@echo "  make        Build all executables"
	@echo "  make cpu    Build RISC-V CPU simulator"
	@echo "  make cache  Build cache simulator"
	@echo "  make trace  Build trace generator"
	@echo "  make clean  Remove build files"
	@echo "  make help   Show this help"
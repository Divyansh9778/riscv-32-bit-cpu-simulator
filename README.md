# 32-bit RISC-V CPU Simulator

A C++-based simulator for a 32-bit RISC-V processor. The project models instruction execution through a pipelined CPU datapath and also includes a configurable cache simulator and memory-trace generator.

## Features

- 32-bit RISC-V instruction simulation
- Instruction fetch, decode, and execution
- Register file simulation
- ALU operations
- Pipeline-based execution
- Branch and jump handling
- Pipeline stalls and flushing
- Memory access simulation
- Configurable cache simulation
- Direct-mapped, fully associative, and set-associative caches
- Write-through and write-back cache policies
- Memory-access trace generation
- Makefile-based build system

## Architecture

The CPU simulator models the major stages of a pipelined processor:

```text
Instruction Fetch (IF)
        ↓
Instruction Decode (ID)
        ↓
Execute (EX)
        ↓
Memory Access (MEM)
        ↓
Write Back (WB)
````

The project also includes:

* Register File
* ALU
* Pipeline Registers
* Control Unit
* Data/Memory Control
* Cache Controller
* Trace Generator

## Project Structure

```text
32-bit-RISC-V-CPU-Simulator/
├── build/                 # Generated executables
├── config/                # Simulator configuration
│   └── config.cfg
├── docs/                  # Documentation
│   └── how_to_run.md
├── examples/              # Example input files
│   ├── input.txt
│   └── example_trace.txt
├── include/               # Header files
│   ├── Assembler.hpp
│   ├── Controller.hpp
│   ├── Pipeline.hpp
│   └── Utils.hpp
├── src/                   # Source files
│   ├── main.cpp
│   ├── Executor.cpp
│   ├── DataControl.cpp
│   ├── CacheController.cpp
│   └── TraceGenerator.cpp
├── Makefile
└── README.md
```

## Build

Make sure `g++`, `make`, and a C++17-compatible compiler are installed.

From the project root:

```bash
make
```

This builds the following executables:

```text
build/riscv_cpu
build/cache_simulator
build/trace_generator
```

## Running the Project

### RISC-V CPU Simulator

Run the CPU simulator using the included example program:

```bash
./build/riscv_cpu
```

The simulator displays pipeline activity and final register values.

The provided example performs a factorial computation:

```text
6! = 720
```

with the final result stored in `x2`.

### Trace Generator

Generate a memory-access trace:

```bash
./build/trace_generator 64 64 1 4 examples/example_trace.txt
```

This generates a matrix-multiplication-style trace and stores it in:

```text
examples/example_trace.txt
```

### Cache Simulator

Run the cache simulator using the generated trace:

```bash
./build/cache_simulator config/config.cfg examples/example_trace.txt
```

The cache simulator evaluates multiple cache organizations and write policies and reports statistics including:

* Cache hits and misses
* Miss rate
* Writebacks
* Memory traffic
* Execution cycles
* AMAT
* Bandwidth

For detailed build and execution instructions, see:

[How to Build and Run](docs/how_to_run)

## Supported Cache Configurations

The cache simulator supports:

* Direct-Mapped Cache
* Fully Associative Cache
* Set-Associative Cache
* Write-Through Policy
* Write-Back Policy

## Example Workflow

```bash
# Build everything
make

# Run the RISC-V CPU simulator
./build/riscv_cpu

# Generate a memory trace
./build/trace_generator 64 64 1 4 examples/example_trace.txt

# Run the cache simulator
./build/cache_simulator config/config.cfg examples/example_trace.txt
```

## Makefile Commands

Build all components:

```bash
make
```

Build only the CPU simulator:

```bash
make cpu
```

Build only the cache simulator:

```bash
make cache
```

Build only the trace generator:

```bash
make trace
```

Remove generated executables:

```bash
make clean
```

Show available commands:

```bash
make help
```

## Example Result

For the included factorial program, the CPU simulator produces:

```text
x1 = 6
x2 = 720
x3 = 0
```

The cache simulator can then be used independently with generated or user-provided memory traces.

## Documentation

For detailed instructions on installation, building, running each component, and the complete workflow:

**[docs/how_to_run](docs/how_to_run)**

## Author

**Divyansh Sharma**
# 32-bit RISC-V CPU Simulator

A C++ based simulator for a 32-bit RISC-V processor implementing instruction
fetch, decode, execution, memory access, and pipeline simulation.

## Features

- 32-bit RISC-V instruction simulation
- Instruction decoding and execution
- Register file simulation
- Memory/cache simulation
- Pipeline execution
- Trace generation
- Configurable simulator parameters
- Makefile-based build system

## Architecture

The simulator models the following major components:

- Instruction Fetch
- Instruction Decode
- Execute
- Memory Access
- Write Back
- Register File
- Cache Controller
- Pipeline Controller

## Build

```bash
make
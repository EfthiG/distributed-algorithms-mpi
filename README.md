# Distributed Algorithms with MPI

A collaborative university project for a **Parallel Systems** course, exploring distributed-memory parallel programming in **C**.

The repository contains two implementations: Conway's Game of Life and matrix–vector multiplication. It also includes scripts for running experiments and generating performance visualizations.

## Projects

### 1. Distributed Conway's Game of Life

**Source:** `src/game_of_life/`

A distributed implementation of Conway's Game of Life on a two-dimensional grid. Each MPI process updates an assigned range of rows for each generation.

- The root process initializes the grid.
- `MPI_Bcast` distributes the grid to all processes.
- Each process computes updates for its assigned rows.
- `MPI_Gather` collects the updated rows at the root, followed by another broadcast for the next generation.
- `MPI_Wtime` measures elapsed execution time.

**Implementation note:** The current `MPI_Gather` call uses a fixed row count per process. Consequently, the implementation is intended for grid sizes divisible by the number of MPI processes; uneven partitions are not correctly handled by the current gather logic.

### 2. Distributed Matrix–Vector Multiplication

**Source:** `src/multiply/`

A column-partitioned implementation of dense matrix–vector multiplication.

- Matrix columns are distributed among MPI processes.
- `MPI_Type_vector` defines a derived datatype for noncontiguous matrix columns.
- Each process calculates partial contributions to the result vector.
- The root process receives and sums the partial results.
- The program requires the matrix dimension to be divisible by the number of processes.

**Implementation note:** The supplied implementation has not been independently validated for numerical correctness. In particular, its root-process vector initialization and column distribution should be reviewed before using it for correctness or performance claims.

## Technologies

- C
- MPI (e.g. MPICH or Open MPI)
- GNU Make
- Python (`graphs.py`, for plotting experimental results)
- Bash (benchmark scripts)

## Repository Structure

```text
.
├── include/
│   ├── common.h
│   ├── game_of_life.h
│   └── multiply.h
├── src/
│   ├── common.c
│   ├── game_of_life/
│   │   ├── grid_funcs.c
│   │   └── main.c
│   └── multiply/
│       ├── funcs.c
│       └── main.c
├── Makefile
├── graphs.py
└── tests.sh
```

Some copies of the original assignment also contain `setup.sh` and saved experimental plots. The setup script is specific to the original university computing environment and should not be run blindly on a personal machine: it changes SSH-related files.

## Build

Install an MPI implementation with its C compiler wrapper (`mpicc`) and launcher (`mpirun` or `mpiexec`), then run:

```bash
make all
```

The Makefile builds:

- `bin/game_of_life`
- `bin/multiply`

To remove generated build files and results:

```bash
make clean
```

## Run

### Game of Life

```bash
mpirun -np 4 ./bin/game_of_life 1000 64
```

Arguments: `<generations> <grid_size>`.

For the current implementation, choose a grid size divisible by the process count (64 is divisible by 4).

### Matrix–Vector Multiplication

```bash
mpirun -np 4 ./bin/multiply 240
```

Argument: `<matrix_size>`. The matrix size must be divisible by the number of MPI processes.

Both programs print a row of timing information suitable for collecting experimental measurements.

## Benchmarking

The repository includes `tests.sh`, which defines experiments using 1, 2, 3, and 4 MPI processes and multiple problem sizes, with repeated runs. In the supplied script, **only the matrix–vector multiplication benchmark is enabled by default**; the Game of Life benchmark and graph-generation calls are commented out.

```bash
bash tests.sh
```

Review the script and adjust problem sizes and process counts to your system before running large experiments. The original assignment archive also includes saved charts and timing data under `tmp_final/`.

## Scope and Limitations

This repository documents a university coursework implementation and its parallelization strategies. Compilation, execution, numerical correctness, and scaling behavior should be verified on the target MPI environment. The presence of benchmark scripts or archived results does not, by itself, establish correctness or speedup.

## Acknowledgments

Developed collaboratively as part of a university **Parallel Systems** course.

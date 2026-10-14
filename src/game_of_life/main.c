#include "game_of_life.h"

void parse_arguments(int argc, char** argv, int* num_generations, int* grid_size) {
    if (argc != 3)
        custom_error("Usage: ./main <grid_size> <num_generations>");

    *num_generations = atoi(argv[1]);
    *grid_size = atoi(argv[2]);

    if (*grid_size <= 0 || *num_generations <= 0)
        custom_error("Error: grid_size and num_generations must be positive integers.");
}

int main(int argc, char** argv) {
    int num_generations, grid_size;
    parse_arguments(argc, argv, &num_generations, &grid_size);

    int rank, size;
    initialize_mpi(&argc, &argv, &rank, &size);

    bool* old_grid = create_contiguous_grid(grid_size);
    bool* new_grid = create_contiguous_grid(grid_size);

    if (rank == 0)
        initialize_grid(old_grid, grid_size);

    // Broadcast the initial old grid to all processes
    MPI_Bcast(old_grid, grid_size * grid_size, MPI_C_BOOL, 0, MPI_COMM_WORLD);

    int local_rows = grid_size / size;
    int start_row = rank * local_rows;
    int end_row = (rank == size - 1) ? grid_size : start_row + local_rows;

    // Get starting time
    double start_time = MPI_Wtime();

    for (int gen = 0; gen < num_generations; gen++) {
        // Process each row assigned to the current process
        upgrade_grid(&old_grid, &new_grid, grid_size, start_row, end_row);

        // Gather the updated grid into the root process
        MPI_Gather(&new_grid[start_row * grid_size], local_rows * grid_size, MPI_C_BOOL,
                   new_grid, local_rows * grid_size, MPI_C_BOOL, 0, MPI_COMM_WORLD);

        // Broadcast the new grid back to all processes
        MPI_Bcast(new_grid, grid_size * grid_size, MPI_C_BOOL, 0, MPI_COMM_WORLD);

        // Swap grids
        swap_grids(&old_grid, &new_grid);
    }

    // Get ending time
    double end_time = MPI_Wtime();

    // Print the stats in the root process
    if (rank == 0)
        // print_grid(old_grid, rows, cols);
        printf("%-15d %-15d %-15d %-15f\n", size, num_generations, grid_size, end_time - start_time);

    free(old_grid);
    free(new_grid);

    MPI_Finalize();
    return 0;
}
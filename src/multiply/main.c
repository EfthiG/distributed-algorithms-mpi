#include "multiply.h"

void parse_arguments(int argc, char* argv[], int* size) {
    if (argc != 2)
        custom_error("Usage: ./main <size>");

    *size = atoi(argv[1]);

    if (*size <= 0)
        custom_error("Error: size must be a positive integer.");
}

int main(int argc, char* argv[]) {
    int size;
    parse_arguments(argc, argv, &size);

    int rank, comm_sz;
    initialize_mpi(&argc, &argv, &rank, &comm_sz);

    // check that comm_sz divides size evenly
    if (size % comm_sz != 0)
        custom_mpi_error(rank, "Error: size must be divisible by the number of processes.");

    int cols_per_process = size / comm_sz;

    double* matrix = NULL;
    double* vector = NULL;
    double* local_columns = NULL;
    double* partial_result = NULL;
    double* result = NULL;

    allocate_memory(rank, size, cols_per_process, &vector, &local_columns, &partial_result);

    if (rank == 0)
        initialize_data(size, &matrix, &vector, &result);

    float start_time = MPI_Wtime();

    // Broadcast the vector to all processes
    MPI_Bcast(vector, size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Scatter blocks of columns to all processes
    scatter_matrix_columns(rank, comm_sz, size, cols_per_process, matrix, local_columns);

    // Perform local computation
    perform_local_computation(rank, size, cols_per_process, local_columns, vector, partial_result);

    // Gather results at the main process
    gather_results(rank, comm_sz, size, partial_result, result);

    float end_time = MPI_Wtime();

    if (rank == 0)
        printf("%-15d %-15d %-15f\n", comm_sz, size, end_time - start_time);

    if (rank == 0) {
        free(matrix);
        free(result);
    }
    free(vector);
    free(local_columns);
    free(partial_result);

    MPI_Finalize();
    return 0;
}
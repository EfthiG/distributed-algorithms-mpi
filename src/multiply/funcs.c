#include "multiply.h"

void initialize_matrix(double* matrix, int n) {
    srand(time(NULL));
    for (int i = 0; i < n * n; i++)
        matrix[i] = rand() % 10 + 1;
}

void initialize_vector(double* vector, int n) {
    srand(time(NULL));
    for (int i = 0; i < n; i++)
        vector[i] = rand() % 10 + 1;
}

void print_matrix(double* matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%6.2f ", matrix[i * n + j]);
        printf("\n");
    }
}

void print_vector(double* vector, int n) {
    for (int i = 0; i < n; i++)
        printf("%6.2f\n", vector[i]);
}

void initialize_data(int size, double** matrix, double** vector, double** result) {
    *matrix = malloc(size * size * sizeof(double));
    *result = malloc(size * sizeof(double));
    if (!*matrix || !*result)
        custom_mpi_error(0, "Error: unable to allocate memory.");

    initialize_matrix(*matrix, size);
    initialize_vector(*vector, size);

    // printf("Matrix:\n");
    // print_matrix(*matrix, size);
    // printf("\nVector:\n");
    // print_vector(*vector, size);
    // printf("\n");
}

void allocate_memory(int rank, int size, int cols_per_process, double** vector, double** local_columns, double** partial_result) {
    *vector = malloc(size * sizeof(double));
    *local_columns = malloc(size * cols_per_process * sizeof(double));
    *partial_result = calloc(size, sizeof(double));

    if (!*vector || !*local_columns || !*partial_result)
        custom_mpi_error(rank, "Error: unable to allocate memory.");
}

void scatter_matrix_columns(int rank, int comm_sz, int size, int cols_per_process, double* matrix, double* local_columns) {
    // If there is only one process, copy the matrix directly to local_columns
    if (comm_sz == 1) {
        for (int i = 0; i < size * cols_per_process; i++)
            local_columns[i] = matrix[i];
        return;
    }

    MPI_Datatype column_block;
    MPI_Type_vector(size, cols_per_process, size, MPI_DOUBLE, &column_block);
    MPI_Type_commit(&column_block);

    if (rank == 0) {
        for (int i = 1; i < comm_sz; i++)
            MPI_Send(&matrix[i * cols_per_process], 1, column_block, i, 0, MPI_COMM_WORLD);

        // Copy the local columns for rank 0
        for (int i = 0; i < size; i++)
            for (int j = 0; j < cols_per_process; j++)
                local_columns[i * cols_per_process + j] = matrix[i * size + j];
    }
    else
        MPI_Recv(local_columns, size * cols_per_process, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    MPI_Type_free(&column_block);
}

void perform_local_computation(int rank, int size, int cols_per_process, double* local_columns, double* vector, double* partial_result) {
    for (int j = 0; j < cols_per_process; j++) {
        int global_col = rank * cols_per_process + j;
        for (int i = 0; i < size; i++)
            partial_result[i] += local_columns[i * cols_per_process + j] * vector[global_col];
    }
}

void gather_results(int rank, int comm_sz, int size, double* partial_result, double* result) {
    if (rank != 0) {
        MPI_Send(partial_result, size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        return;
    }

    for (int i = 0; i < size; i++)
        result[i] = partial_result[i];

    for (int i = 1; i < comm_sz; i++) {
        MPI_Recv(partial_result, size, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int j = 0; j < size; j++)
            result[j] += partial_result[j];
    }

    // print_vector(result, size);

}
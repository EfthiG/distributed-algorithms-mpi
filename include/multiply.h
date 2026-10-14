#ifndef MULTIPLY_H
#define MULTIPLY_H
#include "common.h"

void initialize_data(int size, double** matrix, double** vector, double** result);

void allocate_memory(int rank, int size, int cols_per_process, double** vector, double** local_columns, double** partial_result);

void scatter_matrix_columns(int rank, int comm_sz, int size, int cols_per_process, double* matrix, double* local_columns);

void perform_local_computation(int rank, int size, int cols_per_process, double* local_columns, double* vector, double* partial_result);

void gather_results(int rank, int comm_sz, int size, double* partial_result, double* result);

#endif
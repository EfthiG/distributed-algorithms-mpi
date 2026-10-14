#include "common.h"

void custom_error(char* message){
    fprintf(stderr, "%s\n", message);
    exit(EXIT_FAILURE);
}

void perror_exit(char* message){
    perror(message);
    exit(EXIT_FAILURE);
}


void custom_mpi_error(int rank, char* message) {
    if(rank == 0)
        fprintf(stderr, "%s\n", message);
    MPI_Finalize();
    exit(EXIT_FAILURE);
}

void perror_mpi_exit(int rank, char* message){
    if(rank == 0)
        perror(message);
    MPI_Finalize();
    exit(EXIT_FAILURE);
}

void initialize_mpi(int* argc, char*** argv, int* rank, int* size) {
    MPI_Init(argc, argv);
    MPI_Comm_rank(MPI_COMM_WORLD, rank);
    MPI_Comm_size(MPI_COMM_WORLD, size);
}
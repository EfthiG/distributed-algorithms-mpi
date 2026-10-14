#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <mpi.h>

void custom_error(char* message);
void perror_exit(char* message);

void custom_mpi_error(int rank, char* message);
void perror_mpi_exit(int rank, char* message);

void initialize_mpi(int* argc, char*** argv, int* rank, int* size);

#endif
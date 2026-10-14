#ifndef game_of_life_H
#define game_of_life_H
#include "common.h"

#define DEAD false
#define ALIVE true


bool* create_contiguous_grid(int grid_size);
void initialize_grid(bool* grid, int grid_size);
void print_grid(bool* grid, int grid_size);
int count_alive_neighbours(bool* grid, int grid_size, int row, int col);
void upgrade_grid(bool** old_grid, bool** new_grid, int grid_size, int start_row, int end_row);
void swap_grids(bool** old_grid, bool** new_grid);

#endif
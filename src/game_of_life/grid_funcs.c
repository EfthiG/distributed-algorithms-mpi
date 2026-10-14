#include "game_of_life.h"

bool* create_contiguous_grid(int grid_size) {
    bool* grid = malloc(grid_size * grid_size * sizeof(bool));
    if (!grid)
        custom_error("Error: Unable to allocate memory for the grid.");

    for (int i = 0; i < grid_size * grid_size; i++)
        grid[i] = DEAD;
    return grid;
}

void initialize_grid(bool* grid, int grid_size) {
    srand(time(NULL));
    for (int i = 0; i < grid_size; i++)
        for (int j = 0; j < grid_size; j++)
            grid[i * grid_size + j] = rand() % 2;
}

void print_grid(bool* grid, int grid_size) {
    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++)
            printf("%c", grid[i * grid_size + j] ? 'O' : '.');
        printf("\n");
    }
}

int count_alive_neighbours(bool* grid, int grid_size, int row, int col) {
    const int neighbors[8][2] = {
        {-1, -1}, {-1,  0}, {-1,  1},
        { 0, -1},           { 0,  1},
        { 1, -1}, { 1,  0}, { 1,  1}
    };

    int count = 0;
    for (int i = 0; i < 8; i++) {
        int nrow = row + neighbors[i][0];
        int ncol = col + neighbors[i][1];
        if (nrow >= 0 && nrow < grid_size && ncol >= 0 && ncol < grid_size)
            count += grid[nrow * grid_size + ncol];
    }

    return count;
}

void upgrade_grid(bool** old_grid, bool** new_grid, int grid_size, int start_row, int end_row) {
    for (int i = start_row; i < end_row; i++)
        for (int j = 0; j < grid_size; j++) {
            int alive_neighbours = count_alive_neighbours(*old_grid, grid_size, i, j);
            if ((*old_grid)[i * grid_size + j] == ALIVE)
                (*new_grid)[i * grid_size + j] = (alive_neighbours == 2 || alive_neighbours == 3) ? ALIVE : DEAD;
            else
                (*new_grid)[i * grid_size + j] = (alive_neighbours == 3) ? ALIVE : DEAD;
        }
}

void swap_grids(bool** old_grid, bool** new_grid) {
    bool* temp = *old_grid;
    *old_grid = *new_grid;
    *new_grid = temp;
}
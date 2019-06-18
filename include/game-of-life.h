#ifndef __UTILS_H__   
#define __UTILS_H__

#define Board(x,y) board[(x)*N + (y)]
#define NewBoard(x,y) newboard[(x)*N + (y)]
#define PosMod(a, b) ((a)%(b)>=0)?(a)%(b):(a)%(b) + (b)
#define pBoard(x, y)  (*board)[(x)*N + (y)] 
#define pNewBoard(x, y) (*newboard)[(x)*N + (y)]

#include <mpi.h>
#include <omp.h>

/* set everthing to zero */

void initialize_board (int *board, int N, int comm_size);

/* return the number of on cells adjacent to the i,j cell */

int adjacent_to (int *board, int i, int j, int N);

/* return the number of cells adjacent to the i,j cell for boundary cases */

int barrier_adjacent_to (int *board, int i, int j, int N, int comm_size, int* up, int* down);

/* play the game through one generation */

void play (int **board, int **newboard, int N, int comm_size, int this_rank);

/* print the life board */

void print (int *board, int N);

/* generate random table */

void generate_table (int *board, int N, int comm_size, int this_rank, float threshold);

/* display the table with delay and clear console */

void display_table(int *board, int N);

/* count_adjacent cells and return appropriate value */

int count_adj(int cur_tile, int count);

#ifdef PRINT
/* print sequentially */
void print_sequentially (int* board, int N, int comm_size, int this_rank);
#endif //PRINT

#endif // __FOO_H__ 

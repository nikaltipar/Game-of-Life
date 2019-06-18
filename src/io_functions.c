#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <game-of-life.h>

/* print the life board */

void print (int *board, int N) {
  int   i, j, comm_size;
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

  for (i = 0; i < N/comm_size; i++)
  {
    for (j = 0; j < N; j++)
    {
      printf("%c", Board(i, j) ? 'x' : '.');
    }
    printf("\n");
  }
}



/* display the table with delay and clear console */

void display_table(int *board, int N) 
{
  print (board, N);
  usleep(100000);  
  /* clear the screen using VT100 escape codes */
  //puts ("\033[H\033[J");
}

#ifdef PRINT
void print_sequentially (int* board, int N, int comm_size, int this_rank)
{
  int handshake = 0;

  MPI_Request req;
  MPI_Status stat;

  if (this_rank == 0)
  {
    printf("Printing Board.\n");
  }
  else if (this_rank > 0 && this_rank < comm_size)
  {
    MPI_Recv(&handshake, 1, MPI_INT, PosMod(this_rank - 1, comm_size),
        10, MPI_COMM_WORLD, &stat);
  }

  //printf("%d\n", this_rank);
  display_table(board, N);

  if (this_rank < comm_size - 1)
  {
    MPI_Isend(&handshake, 1, MPI_INT, PosMod(this_rank + 1, comm_size),
        10, MPI_COMM_WORLD, &req);
  }
}
#endif //PRINT

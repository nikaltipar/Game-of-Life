#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <game-of-life.h>

void play (int **board, int **newboard, int N, int comm_size, int this_rank)
{
  int i, j, a;

  int* temp_board;
  int* up = (int *) malloc(N * sizeof(int)); 
  int* down = (int *) malloc(N * sizeof(int)); 
  MPI_Request req;
  MPI_Status stat;

  MPI_Isend(&pBoard(0, 0),
      N, 
      MPI_INT, 
      PosMod(this_rank - 1, comm_size),
      1,
      MPI_COMM_WORLD,
      &req
      );

  MPI_Isend(&pBoard(N/comm_size - 1, 0),
      N, 
      MPI_INT, 
      PosMod(this_rank + 1, comm_size),
      2, 
      MPI_COMM_WORLD,
      &req
      );



#pragma omp parallel for schedule(static)
  for (i = 1; i < N/comm_size - 1; i++)
  {
    for (j = 0; j < N; j++)
    {
      a = adjacent_to (*board, i, j, N);
      pNewBoard(i, j) = count_adj(pBoard(i, j), a);
    }
  }

  MPI_Irecv(down, 
      N, 
      MPI_INT, 
      PosMod(this_rank + 1, comm_size),
      1,
      MPI_COMM_WORLD,
      &req
      );

  MPI_Recv(up,
      N, 
      MPI_INT, 
      PosMod(this_rank - 1, comm_size),
      2,
      MPI_COMM_WORLD,
      &stat
      );

  MPI_Barrier(MPI_COMM_WORLD);

  i = 0;

#pragma omp parallel for schedule(static)
  for (j = 0; j < N; j++)
  {
    a = barrier_adjacent_to (*board, i, j, N, comm_size, up, down);
    pNewBoard(i, j) = count_adj(pBoard(i, j), a);
  }

  i = N/comm_size - 1;
  if (i != 0)
  {

    MPI_Wait(&req, &stat);

#pragma omp parallel for schedule(static)
    for (j = 0; j < N; j++)
    {
      a = barrier_adjacent_to (*board, i, j, N, comm_size, up, down);
      pNewBoard(i, j) = count_adj(pBoard(i, j), a);
    }
  }

  temp_board = *board;
  *board = *newboard;
  *newboard = temp_board;

  MPI_Barrier(MPI_COMM_WORLD);
  free(up);
  free(down);
}

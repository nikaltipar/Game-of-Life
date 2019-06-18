#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <omp.h>
#include <mpi.h>
#include <game-of-life.h>


int main (int argc, char *argv[])
{
  int *board, *newboard, i;

  if (argc != 4)
  { // Check if the command line arguments are correct 
    printf("Usage: %s N thres t x\n"
        "where\n"
        "  N     : size of table (N x N)\n"
        "  thres : propability of alive cell\n"
        "  t     : number of generations\n"
        , argv[0]);
    return (1);
  }

  int comm_size;
  int this_rank;


  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &this_rank);



  // Input command line arguments
  int N = atoi(argv[1]);        // Array size
  double thres = atof(argv[2]); // Propability of life cell
  int t = atoi(argv[3]);        // Number of generations 

  if (this_rank == 0)
  {
    printf("Size %dx%d with propability: %0.1f%%\n", N, N, thres*100);
  }

  board = NULL;
  newboard = NULL;

  //assume p | N
  board = (int *) malloc(N * N/comm_size * sizeof(int));

  if (board == NULL){
    printf("\nERROR: Memory allocation did not complete successfully!\n");
    return (1);
  }

  newboard = (int *)malloc(N * N/comm_size * sizeof(int));

  if (newboard == NULL){
    printf("\nERROR: Memory allocation did not complete successfully!\n");
    return (1);
  }


  initialize_board (board, N, comm_size);
  generate_table (board, N, comm_size, this_rank, thres);

#ifdef PRINT
  MPI_Barrier(MPI_COMM_WORLD);
  print_sequentially(board, N, comm_size, this_rank);
#endif //PRINT

  MPI_Barrier(MPI_COMM_WORLD);

  for (i = 0; i < t; i++)
  {
    play (&board, &newboard, N, comm_size, this_rank);    
  }

#ifdef PRINT
  MPI_Barrier(MPI_COMM_WORLD);
  print_sequentially(board, N, comm_size, this_rank);
#endif //PRINT

  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Finalize();
  return 0;
}

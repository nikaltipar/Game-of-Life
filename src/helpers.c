#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <game-of-life.h>


int adjacent_to (int *board, int i, int j, int N)
{
  int   k, l, count;

  count = 0;


  for (k = -1; k <= 1; k++) 
  {
    for (l = -1; l <= 1; l++)
    {
      if (k || l)
      {
        if (Board(PosMod(i + k, N), PosMod(j + l, N)))
        {
          count++;
        }
      }
    }
  }

  return count;
}

int barrier_adjacent_to (int *board, int i, int j, int N, int comm_size, int* up, int* down)
{
  int k, l, count;

  count = 0;

  for (k = -1; k <= 1; k++) 
  {
    if ((i == 0) && (k == -1))
    {
      count += up[PosMod(j - 1, N)] + up[PosMod(j, N)] + up[PosMod(j + 1, N)];
      continue;
    }
    else if ((i + 1 == N/comm_size) && (k == 1))
    {
      count += down[PosMod(j - 1, N)] + down[PosMod(j, N)] + down[PosMod(j + 1, N)];
      continue;
    }


    for (l =  -1; l <= 1; l++)
    {
      if (k || l)
      {
        if (Board(PosMod(i + k, N), PosMod(j + l, N)))
        {
          count++;
        }
      }
    }
  }
  return count;
}


int count_adj(int cur_tile, int count)
{
  switch (count)
  {
    case 2:
      return cur_tile;
    case 3:
      return 1;
    default:
      return 0;
  }
}


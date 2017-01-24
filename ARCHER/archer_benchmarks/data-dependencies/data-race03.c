/**
 * data-race03.c
 *
 * Description: There may be or may be not a race, depending on the
 * content of array a. In the example the a contains the same values
 * as the index i, so no race will occur, however if a contains the
 * same value in different location a race may occur.
 *
 * Solution: Add reduction(+:sum) to omp pragma.
 *
 */

#include <stdio.h>
#include <math.h>

#define N 1000

int main (int argc, char **argv)
{
  int i;
  int a[N], b[N], sum;

  sum = 0;

  // Vector initialization
#pragma omp parallel for
  for (i = 0; i < N; ++i) {
    a[i] = i;
    b[i] = 0;
  }
  
#pragma omp parallel for
  for (i = 1; i < N; i++) {
      b[a[i]] = b[a[i]] + 1;
  }
}

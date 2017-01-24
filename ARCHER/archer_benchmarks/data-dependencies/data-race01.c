/**
 * data-race01.c
 *
 * Description: Race on array a, multiple location of the array might
 * be accessed by different threads simultaneously.
 *
 * Solution: Include accesses to arrays within a critical section or
 * recompute the second expression
 *
 */

#include <stdio.h>

#define N 1000

int main (int argc, char **argv)
{
  int i;
  int a[N], b[N];
  
  a[0] = 0;
  
#pragma omp parallel for
  for (i = 1; i < N; i++) {
    a[i] = 2.0 * i * (i - 1);
    b[i] = a[i] - a[i - 1];
  } /* end of omp parallel */
}

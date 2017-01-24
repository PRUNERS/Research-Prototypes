In order to run the following examples set the number of threads >= 2
with the following command:

     export OMP_NUM_THREADS=2

Programs:

* data_race01.c
  Description: Race on array a, multiple location of the
  array might be accessed by different threads simultaneously.
  Compile with:
          clang -g -fopenmp -fsanitize=thread data-race01.c -o data-race01

* data_race02.c
  Description: Race on vector a, because of data dependency.
  Compile with:
          clang -g -fopenmp -fsanitize=thread data-race02.c -o data-race02

* data_race03.c
  Description: There may be or may be not a race, depending on the
  content of array a. In the example the a contains the same values as
  the index i, so no race will occur, however if a contains the same
  value in different location a race may occur.
  Compile with:
          clang -g -fopenmp -fsanitize=thread data-race03.c -o data-race03

/*
Copyright (c) 2015
Lawrence Livermore National Security, LLC.

Produced at the Lawrence Livermore National Laboratory.
Written by Anh Vo, Sriram Aananthakrishnan, Ganesh Gopalakrishnan,
Bronis R. de Supinski, Martin Schulz, and Greg Bronevetsky
Contact email: schulzm@llnl.gov, LLNL-CODE-647203
All rights reserved - please read information in "LICENSE".

This file is part of DAMPI. For details see
https://github.com/soarlab/DAMPI.

Please also read the file "LICENSE" included in this package for Our
Notice and GNU Lesser General Public License.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License (as published by
the Free Software Foundation) version 2.1 dated February 1999.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the IMPLIED WARRANTY OF
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the terms and
conditions of the GNU General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this program; if not, write to the

Free Software Foundation, Inc., 
59 Temple Place, Suite 330, 
Boston, MA 02111-1307 USA
*/

/* Author: Anh Vo 
 * This basic example presents the most basic situation 
 * of message race: the Recv from P0 has two possible matches coming from
 * P1 and P2
 */

#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
  int data1 = 0;
  int size, rank;
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
  if (size < 3) {
    if (rank == 0)
      fprintf(stderr, "Need at least three processes\n");
    MPI_Abort(MPI_COMM_WORLD,1);
  }


  if (rank == 0) {
    MPI_Recv(&data1, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
    if (status.MPI_SOURCE == 1) {
      fprintf(stderr, "Possible Matching 1\n");
    } else if (status.MPI_SOURCE == 2) {
      fprintf(stderr, "Possible Matching 2\n");
    }
    MPI_Recv(&data1, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
  } else if (rank == 1) {
    MPI_Send(&data1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  } else if (rank == 2) {
    MPI_Send(&data1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
  
  fprintf(stdout, "rank %d finished\n", rank);
  MPI_Finalize();
  return 0;
}

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
 * This example presents the crooked barrier situation
 * The Ir* can cross the barrier and can match either Isend from P1 or P2
 * (Of course this is also due to the fact that the Isend can also cross
 * the barrier. If we are to change the P1's Isend into an Ssend, then only
 * P1's Ssend can match with the Irecv).
 */

#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
  int data1,data2 = 0;
  int size, rank;
  MPI_Request request[2];
  MPI_Status status[2];
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
  if (size < 3) {
    if (rank == 0)
      fprintf(stderr, "Need at least three processes\n");
    MPI_Abort(MPI_COMM_WORLD,1);
  }

  if (rank == 0) {
    MPI_Irecv(&data1, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &request[0]);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Wait(&request[0], &status[0]);
    if (status[0].MPI_SOURCE == 1){
      fprintf(stderr, "Possible match 1\n");
    } else if (status[0].MPI_SOURCE == 2) {
      fprintf(stderr, "Possible match 2\n");
    }
    MPI_Recv(&data2, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status[1]);
  } else if (rank == 1) {
    MPI_Isend(&data1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request[0]);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Wait(&request[0], &status[0]);    
  } else if (rank == 2) {
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Isend(&data1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request[0]);
    MPI_Wait(&request[0], &status[0]);    
  }
  
  fprintf(stdout, "rank %d finished\n", rank);
  MPI_Finalize();
  return 0;
}
